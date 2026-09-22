/*
 * MyNewTask.h
 *
 *  Created on: 16 sep. 2026
 *      Author: fabri
 */

#ifndef MYNEWTASK_H_
#define MYNEWTASK_H_

/* Fwk */
#include "TimersManager.h"
#include "FunctionLib.h"
#include "LED.h"
/* KSDK */
#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"

#include <stdio.h>
#include <string.h>

/* Define the available Task's Events */
#define gMyNewTaskEvent1_c (1 << 0)
#define gMyNewTaskEvent2_c (1 << 1)
#define gMyNewTaskEvent3_c (1 << 2)

#define gMyNewTaskEvent_SW3_c (1 << 3) // Async hardware event, SW3 was pressed. Forces counter to 0
#define gMyNewTaskEvent_SW4_c (1 << 4) // Async hardware event, SW4 was pressed. Forces counter to 2

#define gMyTaskPriority_c 3
#define gMyTaskStackSize_c 400

void MyTaskTimer_Start(void);
void MyTaskTimer_Stop(void);
void MyTask_Init(void);

void MyTask_SW3_Pressed(void); // Bridge function, called from mwa_end_device.c to notify the custom task about SW3
void MyTask_SW4_Pressed(void); // Bridge function, called from mwa_end_device.c to notify the custom task about SW4

void App_TransmitCustomData(uint8_t *payload, uint8_t length); // Bridge function, called from MyNewTask.c to inject data into the MAC layer

#endif /* MYNEWTASK_H_ */
