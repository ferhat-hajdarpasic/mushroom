
/******************************************************************************

 @file       air_i2c.c

 *  Created on: Apr 1, 2018
 *      Author: MR

 Group: CMCU, SCS
 Target Device: CC2640R2

 ******************************************************************************

 Copyright (c) 2018, AirTracker
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: simplelink_cc2640r2_sdk_1_40_00_45
 Release Date:
 *****************************************************************************/
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <ti/devices/cc26x0r2/driverlib/ioc.h>
#include <ti/devices/cc26x0r2/driverlib/udma.h>
#include <ti/devices/cc26x0r2/inc/hw_ints.h>
#include <ti/devices/cc26x0r2/inc/hw_memmap.h>

#include <ti/ble5stack/boards/SABLEXR2_DEV_BOARD/SABLEXR2_DEV_BOARD.h>
#include <icall.h>
#include "util.h"
/* This Header file contains all BLE API and icall structure definition */
#include "icall_ble_api.h"

#include "devinfoservice.h"

#include "Board.h"
#include "board.h"
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>
#include "air_i2c.h"

#include <stdio.h>
#include <measurement.h>
#include <frame.h>

static void AirTrackerBoard_init( void );
static void AirTrackerBoard_taskFxn(UArg a0, UArg a1);
static void AirTrackerBoard_performPeriodicTask(void);
static Clock_Struct periodicClock;

static ICall_EntityID selfEntity;

// Event globally used to post local events and pend on system and
// local events.
static ICall_SyncHandle syncEvent;
#define Board_I2C0              CC2640R2_SABLEXR2_I2C0
#define AIR_TASK_STACK_SIZE                   644
#define AIR_TASK_PRIORITY                     1

#define AIR_PERIODIC_EVT                      Event_Id_01
#define AIR_ICALL_EVT                         Event_Id_28 // Event_Id_31
#define AIR_QUEUE_EVT                         Event_Id_27 // Event_Id_30

#define AIR_ALL_EVENTS                        (AIR_ICALL_EVT        | \
                                               AIR_QUEUE_EVT        | \
                                               AIR_PERIODIC_EVT)
Task_Struct airTask;
Char airTaskStack[AIR_TASK_STACK_SIZE];

I2C_Handle        i2c;
I2C_Params        i2cParams;
I2C_Transaction   i2cTransaction;
uint8_t       txBuffer[1];
uint8_t       rxBuffer[2];
uint16_t        temperature;
uint16_t        humudity;



/*********************************************************************
 * PUBLIC FUNCTIONS
**********************************************************************/


  void air_hdm1080init( void)
{

 I2C_init();
 I2C_Params_init(&i2cParams);
 i2cParams.bitRate = I2C_400kHz;
 i2c = I2C_open(Board_I2C0, & i2cParams);

 if (i2c == NULL)
 {
     while(1);
 }

 txBuffer[0] = hdm1080_temp;
 i2cTransaction.slaveAddress = hdm1080_addr;
 i2cTransaction.writeBuf = txBuffer;
 i2cTransaction.writeCount = 1;
 i2cTransaction.readBuf =  rxBuffer;
 i2cTransaction.readCount = 2;

 if (I2C_transfer(i2c, &i2cTransaction))
 {
     temperature = (rxBuffer[0]<<6) | (rxBuffer[1] >> 2);
 }
 temperature /=32;

 I2C_close(i2c);
}


  void AirTrackerBoard_init( void)
{
      air_hdm1080init();
}
  /*********************************************************************
   * @fn      SimpleBLEPeripheral_createTask
   *
   * @brief   Task creation function for the Simple Peripheral.
   *
   * @param   None.
   *
   * @return  None.
   */
  void AirTrackerBoard_performPeriodicTask (void)
  {

  }

  void AirTrackerBoard_createTask(void)
  {
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = airTaskStack;
    taskParams.stackSize = AIR_TASK_STACK_SIZE;
    taskParams.priority = AIR_TASK_PRIORITY;

    Task_construct(&airTask, AirTrackerBoard_taskFxn, &taskParams, NULL);
  }

  void LOG(const char* msg) {
      puts(msg);
  }

  void LOGBUFFER(const unsigned char* buffer, int length) {
      char output[100];
      for(int i = 0; i < length; i++) {
          sprintf(&output[2*i], "%02x", buffer[i]);
      }
      output[2*length] = 0;
      puts(output);
  }

  /*********************************************************************
   * @fn      SimpleBLEPeripheral_taskFxn
   *
   * @brief   Application task entry point for the Simple Peripheral.
   *
   * @param   a0, a1 - not used.
   *
   * @return  None.
   */
  static void AirTrackerBoard_taskFxn(UArg a0, UArg a1)
  {
    // Initialize application
  //    air_hdm1080init( );
      AirTrackerBoard_init();
      puts("Started\n");
        struct Measurement m = {.Pm2_5 = 0x01, .O3 = 0x03, .CO2 = 0x01};
        for(int frameIndex = 0; frameIndex < sizeof(frames); frameIndex++) {
            LOG("Frame:");
            unsigned char* result = frame(frameIndex, &m);
            LOGBUFFER(result, 12);
        }
        LOG("Finished\n");
    // Application main loop
 /*
    for (;;)
    {
      uint32_t events;

      // Waits for an event to be posted associated with the calling thread.
      // Note that an event associated with a thread is posted when a
      // message is queued to the message receive queue of the thread
      events = Event_pend(syncEvent, Event_Id_NONE, AIR_ALL_EVENTS,
                          ICALL_TIMEOUT_FOREVER);

      if (events)
      {
        ICall_EntityID dest;
        ICall_ServiceEnum src;
        ICall_HciExtEvt *pMsg = NULL;



        if (events & AIR_PERIODIC_EVT)
        {
          Util_startClock(&periodicClock);

          // Perform periodic application task
          AirTrackerBoard_performPeriodicTask();
        }


      }

  }
  */

 }


