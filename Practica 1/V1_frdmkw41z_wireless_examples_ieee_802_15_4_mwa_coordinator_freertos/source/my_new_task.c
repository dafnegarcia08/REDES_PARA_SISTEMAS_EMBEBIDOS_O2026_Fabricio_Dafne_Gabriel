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

/* Función para inicializar/pedir el timer al sistema operativo */
void MyTaskTimer_Init(void)
{
    if (myTimerID == gTmrInvalidTimerID_c)
    {
        myTimerID = TMR_AllocateTimer();
    }
}

/* Función para arrancar el timer repitiéndose cada 5 segundos */
void MyTaskTimer_Start(void)
{
    if (myTimerID != gTmrInvalidTimerID_c)
    {
        /* 5000 ms = 5 segundos. Llama a tu función directamente */
        TMR_StartIntervalTimer(myTimerID, 5000, App_NodeCheckTimerCallback, NULL);
    }
}

/* Función de seguridad por si necesitas detenerlo en algún momento */
void MyTaskTimer_Stop(void)
{
    if (myTimerID != gTmrInvalidTimerID_c)
    {
        TMR_StopTimer(myTimerID);
    }
}

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
