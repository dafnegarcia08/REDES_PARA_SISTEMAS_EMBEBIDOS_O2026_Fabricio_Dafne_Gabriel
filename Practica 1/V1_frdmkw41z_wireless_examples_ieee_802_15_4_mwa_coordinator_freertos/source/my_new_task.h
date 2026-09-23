/*
 * my_new_task.h
 *
 *  Created on: 22 sep. 2026
 *      Author: Gabriel Gomez
 */

#ifndef MY_NEW_TASK_H_
#define MY_NEW_TASK_H_

/* Fwk */
#include "TimersManager.h"
#include "FunctionLib.h"
#include "LED.h"
#include "SerialManager.h"
/* KSDK */
#include "fsl_common.h"
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"

/* Phase 2: MaxNodes is the limit of simultaneously active associated nodes (5, per practice spec) */
#define MaxNodes 5
#define MaxHistoryNodes 10
#define NodeMissThreshold 2

/* Phase 2 (2 points): structure that stores the info of every node that can associate to the network.
   Declared here so it is visible from both my_new_task.c and mwa_coordinator.c */
typedef struct
{
    bool_t   inUse;
    uint16_t shortAddress;
    uint64_t extAddress;
    bool_t   rxOnWhenIdle;
    bool_t   FFD_not_RFD;
    uint8_t  missedCount;
} nodeInfo_t;

extern uint8_t interfaceId;
extern nodeInfo_t NodeTable[MaxHistoryNodes];

/* Prototipos de funciones para controlar el timer */
void MyTaskTimer_Init(void);
void MyTaskTimer_Start(void);
void MyTaskTimer_Stop(void);

#endif /* MY_NEW_TASK_H_ */
