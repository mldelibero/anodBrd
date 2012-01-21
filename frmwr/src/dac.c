/*
	File:          dac.c
	Author:        Michael DeLibero
	Date Created:  11.09.11
	Last Modified: 11.09.11 - Michael DeLibero
	
	Description: 	This file contains all functions pertaining to the dac.
					
	References: 	
	
*/
//-----------------------------------------------------------------------------
#include <avr/io.h>
#include "spi_driver.h"
#include "dac.h"
#include "init.h"
#include "spi_driver.h"
#include "dac.h"
#include "spi.h"
//-----------------------------------------------------------------------------
//file scope defines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//file scope variables
//-----------------------------------------------------------------------------
uint8_t receivedData[NUM_SPI_BYTES];
//Spi setup -- spi in port C is controlling the dacs
	uint8_t           bytesToTransceive;
	PORT_t           *ssPort;
	uint8_t           ssPinMask;
//-----------------------------------------------------------------------------
// file defined, global scope variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// function pointer externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void dac_init(void) {
	EN1 = 1;
	EN2 = 1;
	
	LDAC1 = 1;
	LDAC2 = 1;
	delay_us(1); // may not be needed.
	
	dac_up(DAC_VOL,GA_2x,SHDN_y,ch1,&ch[0]);
	dac_up(DAC_CUR,GA_1x,SHDN_y,ch1,&ch[0]);
	
	dac_up(DAC_VOL,GA_2x,SHDN_y,ch2,&ch[0]);
	dac_up(DAC_CUR,GA_1x,SHDN_y,ch2,&ch[0]);
}
//-----------------------------------------------------------------------------
void dac_up(uint8_t ch_sel,uint8_t gain, uint8_t shutdwn, uint8_t ch_num, \
            volatile channel_t *ch) {
				
	// 1. Create a formatted packet.
	// 2. Send out packet.

	uint8_t spiRx[NUM_SPI_BYTES]; 
		//data rx at Xmega SPI port -- var is only a placeholder.
	uint8_t spiTx[NUM_SPI_BYTES];
		 // formatted for ext dacs to be sent from the mega SPI port.
	SPI_DataPacket_t spi_fdata; 
		// data packet formatted for Xmega to be sent from the mega SPI port.
    
	//Check DAC data sheet to verify this formatting for the DAC regs
	uint16_t volt0;
	uint16_t volt1;

	if (ch_sel == DAC_VOL) {
		volt0 = ch[ch_num].vol2dac_u.all << 2;
		volt1 = ch[ch_num].vol2dac_u.all >> 6;		
	}
	else { // ch_sel == DAC_CUR
		volt0 = ch[ch_num].cur2dac_u.all << 2;
		volt1 = ch[ch_num].cur2dac_u.all >> 6;
	}
	
	spiTx[0] = 0; // make sure that this byte has been initialized to zero.
	spiTx[1] = 0; // verbose
	spiTx[1] = volt0;
	spiTx[0] = ch_sel | gain | shutdwn | volt1;

	SPI_MasterCreateDataPacket(&spi_fdata,
                                spiTx,
                                spiRx,
                                NUM_SPI_BYTES,
                                ch[ch_num].ss_port,
                                ch[ch_num].ss_pin);
								
	uint8_t status;

	do {
		status = SPI_MasterInterruptTransceivePacket(&spiMasterC,&spi_fdata);	
	} while (status != SPI_OK);
	
	// Wait for transmission to complete.
	while (spi_fdata.complete == false) {
	}

	delay_us(1);
	
	if(ch_num == ch1) {
		LDAC1 = 0;	
		delay_us(1);
		LDAC1 = 1;
	}
	else { // then we have chan 2
		LDAC2 = 0;
		delay_us(1);
		LDAC2 = 1;	
	}
	
	delay_us(1); // may not be necessary.

} // end	void dac_up(...) {										
//-----------------------------------------------------------------------------
// ISRs
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// eof
