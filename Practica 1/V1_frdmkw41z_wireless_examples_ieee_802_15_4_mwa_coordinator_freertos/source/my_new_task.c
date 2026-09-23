/*
 * my_new_task.c
 *
 *  Created on: 22 sep. 2026
 *      Author: Gabriel Gomez
 */

osaEventId_t mMyEvents;

/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;

static void App_NodeCheckTimerCallback(void *param)
{
    uint8_t i;

    for(i = 0; i < mMaxNodes_c; i++)
    {
        if(NodeTable[i].inUse)
        {
            NodeTable[i].missedCount++;

            if(NodeTable[i].missedCount >= mNodeMissThreshold_c)
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
