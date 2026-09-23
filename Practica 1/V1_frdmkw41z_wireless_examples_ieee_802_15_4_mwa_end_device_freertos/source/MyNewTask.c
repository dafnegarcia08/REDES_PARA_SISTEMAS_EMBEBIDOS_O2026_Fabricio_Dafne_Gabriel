/*
 * MyNewTask.c
 *
 *  Created on: 16 sep. 2026
 *      Author: fabri
 */


#include "MyNewTask.h"
#include <stdio.h>
#include <string.h>

void My_Task(osaTaskParam_t argument);

/* OSA Task Definition*/
OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );


osaEventId_t mMyEvents;

/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;

/* Local variable to store the current state of the LEDs */
// static uint8_t ledsState = 0;

/* Local variable to store the current state of the LEDs */
 static int8_t counter_x = 0;

/* Function to init the task */
void MyTask_Init(void)
{
	mMyEvents = OSA_EventCreate(TRUE);
	/* The instance of the MAC is passed at task creaton */
	gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);
}


/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback(void *param)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}

/* Public function to send an event to stop the timer */
void MyTaskTimer_Stop(void)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent3_c);
}

/* Public function to send an event to start the timer */
void MyTaskTimer_Start(void)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent1_c);
}

/* Public function to send an event that the SW3 is pressed in mwa_end_device_2.c */
void MyTask_SW3_Pressed(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEvent_SW3_c);
}

/* Public function to send an event that the SW4 is pressed in mwa_end_device_2.c */
void MyTask_SW4_Pressed(void)
{
    OSA_EventSet(mMyEvents, gMyNewTaskEvent_SW4_c);
}

/* Main custom task */
void My_Task(osaTaskParam_t argument)
{
	osaEventFlags_t customEvent;
	myTimerID = TMR_AllocateTimer();
	while(1)
	{
		OSA_EventWait(mMyEvents, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &customEvent);
		if( !gUseRtos_c && !customEvent)
		{
			break;
		}

		/* Depending on the received event */
		switch(customEvent){
		case gMyNewTaskEvent1_c:
			TMR_StartIntervalTimer(myTimerID, /*myTimerID*/
					4000, /* Timer's Timeout */
					myTaskTimerCallback, /* pointer to
					myTaskTimerCallback function */
					NULL
			);
			/* Ensure all LEDs are turned off */
			LED_TurnOffAllLeds();
			break;

		case gMyNewTaskEvent2_c: /* Event called from myTaskTimerCallback */
			if(counter_x == 3) {
				counter_x = 0; /* Reset to 0 if the counter reach the limit */
			}
			else {
				counter_x = counter_x + 1;  /* Otherwise increment by 1 */
			}
			LED_TurnOffAllLeds();

			switch (counter_x){
			case 0: // If counter = 0, turn on GREEN LED
				Led3On();
				break;
			case 1:
				Led2On(); // If counter = 1, turn on RED LED
				break;
			case 2:
				Led4On(); // If counter = 2, turn on BLUE LED
				break;
			case 3:
				Led2On(); // If counter = 3, turn on PURPLE LED = RED LED + BLUE LED
				Led4On();
				break;
			}
			/* Create the base string buffer */
			uint8_t mi_mensaje[] = "Counter: 0";

			/*Convert the current integer state (counter_x) into its ASCII character representation by adding it to the base ASCII value of '0' (0x30)*/
			mi_mensaje[9] = (uint8_t)('0' + counter_x);

			/* Send 10-byte string to the MAC layer for transmission.*/
			App_TransmitCustomData(mi_mensaje, 10);


			break;
		case gMyNewTaskEvent3_c: /* Event to stop the timer */
			counter_x = 0;
			LED_TurnOffAllLeds();
			TMR_StopTimer(myTimerID);
			break;

		case gMyNewTaskEvent_SW3_c: /* Event when the SW3 is pressed */
			counter_x = 0; // Force the counter to 0

			/* Turn off the LEDS and then turn on GREEN LED*/
			LED_TurnOffAllLeds();
			Led3On();

			{
				uint8_t my_message[] = "Counter: 0"; // Create a local string buffer for a message
				my_message[9] = (uint8_t)('0' + counter_x); // Overwrite the 9th index (the '0' character. By adding '0' (0x30 in hex) to the integer 'counter_x' (which is 0), it translates to the ASCII character '0'.
				App_TransmitCustomData(my_message, 10); // Pass the crafted string to the MAC layer function to send it
			}

			/* Reset the timer to 4 seconds*/
			TMR_StartIntervalTimer(myTimerID, 4000, myTaskTimerCallback, NULL);
			break;

		case gMyNewTaskEvent_SW4_c: /* Event when the SW4 is pressed */
			counter_x = 2; // Force the counter to 2

			/* Turn off the LEDS and then turn on BLUE LED*/
			LED_TurnOffAllLeds();
			Led4On();

			{
				uint8_t my_message[] = "Counter: 0";
				my_message[9] = (uint8_t)('0' + counter_x);
				App_TransmitCustomData(my_message, 10);
			}
			TMR_StartIntervalTimer(myTimerID, 4000, myTaskTimerCallback, NULL);
			break;
		default:
			break;
		}
	}
}


