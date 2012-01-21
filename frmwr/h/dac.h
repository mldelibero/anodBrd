/*
	File:          DAC.h
	Author:        Michael DeLibero
	Date Created:  11.09.11
	Last Modified: 11.09.11 - Michael DeLibero
	
	Description: 	This is the header file for DAC.c.
					See DAC.c for further information.
					
	References: 	
		Microchip MCP4822 Datasheet
			http://ww1.microchip.com/downloads/en/DeviceDoc/22249A.pdf
*/
//-----------------------------------------------------------------------------
#ifndef DAC_H
	#define DAC_H
	#include "init.h" // location of typedef --- port_t
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Structs
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Unions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// #Defines
//-----------------------------------------------------------------------------
//Output pins to the DACs
	//Synchronization Input. This pin is used to transfer DAC settings
	//(Input Registers) to the output registers (VOUT)
	#define LDAC1	(MYPORTF->bit1)
	#define LDAC2	(MYPORTE->bit4)
	#define LDAC3	(MYPORTD->bit3)
	#define LDAC4	(MYPORTC->bit3)
	//slave select pins (chip select)
	#define SS1		(MYPORTF->bit0)
	#define SS2		(MYPORTE->bit3)
	#define SS3		(MYPORTD->bit5)
	#define SS4		(MYPORTD->bit0)
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// typedef externs
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void dac_init(void);
//-----------------------------------------------------------------------------
void dac_up(uint8_t ch_sel, uint8_t gain, uint8_t shutdwn, uint8_t ch_num, \
            volatile channel_t *ch);
/*
	DAC update
	
	This function commands an external DAC to update its output value.
	
	Parameters:
		ch_sel: channel select --- each DAC has two output channels. (2 total)
		gain:				   --- each DAC can be configured to run at a
									1x or 2x gain
		shutdwn: shutdown	   --- each channel on the DACs can be made inoperable
								   in case you do not want it in be outputting
								   power.
		ch_num:	channel number --- There is a DAC per output channel. This
								   selects between them. (4 total).
		ch:		channel		   --- All of the state information for each chan.
		
	References:
		MCP4802/4812/4822 datasheet:
			http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en547820
*/
//-----------------------------------------------------------------------------
#endif // DAC_H
//eof
