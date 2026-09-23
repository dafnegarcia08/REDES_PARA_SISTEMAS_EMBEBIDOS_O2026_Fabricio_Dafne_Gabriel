/*
 * my_new_task.c
 *
 *  Created on: 22 sep. 2026
 *      Author: Gabriel Gomez
 */

#include "my_new_task.h"

osaEventId_t mMyEvents;

/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;

static void App_NodeCheckTimerCallback(void *param);

/* Phase 2 (extra): allocates the periodic timer used for node keep-alive tracking */
void MyTaskTimer_Init(void)
{
    if (myTimerID == gTmrInvalidTimerID_c)
    {
        myTimerID = TMR_AllocateTimer();
    }
}

/* Starts the keep-alive timer, repeating every 5 seconds */
void MyTaskTimer_Start(void)
{
    if (myTimerID != gTmrInvalidTimerID_c)
    {
        /* 5000 ms = 5 segundos. Llama a tu función directamente */
        TMR_StartIntervalTimer(myTimerID, 5000, App_NodeCheckTimerCallback, NULL);
    }
}

/* Safety function in case the timer needs to be stopped at some point */
void MyTaskTimer_Stop(void)
{
    if (myTimerID != gTmrInvalidTimerID_c)
    {
        TMR_StopTimer(myTimerID);
    }
}
/* Phase 2 : periodically checks every active node and frees stale ones that stopped transmitting */
static void App_NodeCheckTimerCallback(void *param)
{
    uint8_t i;

    for(i = 0; i < MaxHistoryNodes; i++)
    {
        if(NodeTable[i].inUse)
        {
            NodeTable[i].missedCount++;

            if(NodeTable[i].missedCount >= NodeMissThreshold)
            {
                /* Node considered disconnected: free the slot */
                Serial_Print(interfaceId, "\n\rNode timed out, removing from table:\n\r  Extended Address: 0x", gAllowToBlock_d);
                Serial_PrintHex(interfaceId, (uint8_t *)&NodeTable[i].extAddress, 8, gPrtHexNoFormat_c);
                Serial_Print(interfaceId, "\n\r\n\r", gAllowToBlock_d);

                NodeTable[i].inUse = FALSE;
            }
        }
    }
}
