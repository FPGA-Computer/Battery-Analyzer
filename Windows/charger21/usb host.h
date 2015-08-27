/*
 * Created: July-23-13, 7:58:33 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 *
 * The software is under Mozilla Public License
 * The full detail of this license is available at: 
 * https://www.mozilla.org/MPL/
 */ 

#ifndef USB_HOST_H_
#define USB_HOST_H_

#define USB_POLL_FREQ		500
#define USER_USB_POLL_DISABLE	0x01

// Transfer timeout
#define CMD_TIMEOUT		1000
#define BLOCK_TIMEOUT		5000

// Device time out
#define SleepTime		100			// in milliseconds
#define TIMEOUT			((READ_TIMEOUT)/SleepTime)

// Power up wait time
#define POWER_ON_DELAY		100
#define POWER_OFF_DELAY		100

#define USB_CONFIG_STR		32

usb_dev_handle *Open_USB_Dev(void);
extern usb_dev_handle *USB_Dev_Handle;
int8_t Poll_USB(void);
void USB_ErrorMsg(char *Func,int Error);

extern DWORD USB_ThreadID;
DWORD WINAPI USBTask( LPVOID Dummy );
void USB_Cmd(uint8_t Cmd, uint16_t Value, uint16_t Index);
#endif
