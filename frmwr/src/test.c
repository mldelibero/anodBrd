// Author:	Michael DeLibero
// Date Created:	10.27.2011

// This file contains all of the unit testing functions for the AnodeV3.0 proj
//-----------------------------------------------------------------------------
// #includes
#include <avr/io.h>
#include "test.h"
#include "init.h"
#include "TC_driver.h"
#include "spi.h"
#include "spi_driver.h"
#include "dac.h"
#include "usart.h"
#include "adc.h"
#include "adc_driver.h"
//-----------------------------------------------------------------------------
// Defines
#define LEDPORT		(PORTF)
//-----------------------------------------------------------------------------
// Variable Definitions and Declarations
volatile testing_flags_t testing_flags;
//dac
	volatile uint16_t dac_cnt    = 0b0011111111;
	volatile uint16_t dac_cntMax = 0b1111111111; // 10 bits ---(73,300,775,185) 
	volatile int ch_num = 1;
	uint8_t receivedData[NUM_SPI_BYTES];
	SPI_DataPacket_t dataPacket;
	uint8_t sendData[NUM_SPI_BYTES];
	volatile uint8_t EN = 1;
	volatile uint8_t low_byte;
	volatile uint8_t hi_byte;
	volatile uint8_t status;
	PORT_t *ss_port = &PORTF;
	volatile unsigned int ss_pin = PIN0_bm;
//usart
	volatile char rxByte = 0x33; // stores the last byte read by the usart rx.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void led_test(void) {
	led0 = 0;
	led1 = 1;
	led2 = 1;
	led3 = 0;
}
//-----------------------------------------------------------------------------
void clk_test(void) {	
	LEDPORT.DIRSET = 0xFF; // Set Port F pins as output
	LEDPORT.OUTSET = 0x80; // Sets the values on port F pins
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc; // Set clock prescaler (14.12.1)
	TCC0.PER = 1563; // Set Clock period to 9.99Hz
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc; // enable timer overflow int and
		// set its priority to low 
	sei(); // enable global interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm; // Enable Low Level interrupts (12.8.3)
	
	while (1)
	{
		LEDPORT.OUTTGL = 0x10; // Toggle led
	}
}
//-----------------------------------------------------------------------------
void usart_test(void) {
	USARTF0.DATA = 0x0F; // necessary to trip the interrupt once the usart
						// flag has been set.
	while(1) {nop();}
}
		
void usartTx_test(void) {		
	USARTF0.DATA = 0x7E
} // end	usartTx_test()
	


void usartRx_test(void) {
	rxByte = USARTF0.DATA;	
}
//-----------------------------------------------------------------------------
void usartRxMsg_test(volatile char uRxByte) {
	static uint8_t ind = 0;
	
	*dat2pc_p[ind] = uRxByte;
	
	ind++;
	if (ind >= 12) {
		ind = 0;
	}
}
//-----------------------------------------------------------------------------
void dac_test(void)  {
	#define sweep	(1) // 0 --- sweep output, 1 --- const output
	
	ch[ch1].vol2dac_u.all = 0x02C1;
	ch[ch2].vol2dac_u.all = 0x02C1;
	
	ch[ch1].cur2dac_u.all = 0x02C1;
	ch[ch2].cur2dac_u.all = 0x02C1;
	
	bool vol_1_sweep = false;
	bool vol_2_sweep = false;
	bool cur_1_sweep = false;
	bool cur_2_sweep = false;
	
	if (vol_1_sweep == false) {
		dac_up(DAC_VOL,GA_2x,SHDN_n,ch1,&ch[0]);
	}
	if (vol_2_sweep == false){
		dac_up(DAC_VOL,GA_2x,SHDN_n,ch2,&ch[0]);
	}		
	if (cur_1_sweep == false){
		dac_up(DAC_CUR,GA_1x,SHDN_n,ch1,&ch[0]);
	}
	if (cur_2_sweep == false) {
		dac_up(DAC_CUR,GA_1x,SHDN_n,ch2,&ch[0]);
	}
} // end	void dac_test(void)  {
//-----------------------------------------------------------------------------
void spiInit_test(void) {
	// This is a stripped down version of dac_up(...)
	uint8_t spiRx[NUM_SPI_BYTES]; 
		//data rx at Xmega SPI port -- var is only a placeholder.
	uint8_t spiTx[NUM_SPI_BYTES];
		 // formatted for ext daqs to be sent from the mega SPI port.
	SPI_DataPacket_t spi_fdata; 
		// data packet formatted for Xmega to be sent from the mega SPI port.
	
	spiTx[0] = 0x32;
	spiTx[1] = 0x87;
	
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
		
	/* Wait for transmission to complete. */
	while (spi_fdata.complete == false) {
	}
} // end    void spiInit_test(void) {
//-----------------------------------------------------------------------------
void adcOff_test(void) {
// This exports the offsets to channel 1 and 2 voltage.
// Look at these on your PC to make sure that they seem reasonable.
// Also make sure that the adc ISR is disabled when this function flag is 
// active or the offsets will be overwritten.
	while(1) {
		ch[ch1].volfadc_u.all = ADC_Offset_Get_Signed(&ADCA, &(ADCA.CH2), true);
		ch[ch2].volfadc_u.all = ADC_Offset_Get_Signed(&ADCB, &(ADCB.CH2), true);
		
		delay_us(500);
	}	
}
void adc_test(void) {
	//TCD0.INTCTRLA = TC_OVFINTLVL_LO_gc; 
	nop();
}
//-----------------------------------------------------------------------------
/*This code use Timer/Counter C0: TCC0 */
/*ISR(TCC0_OVF_vect) {
	if (testing_flags.clk == 1)
	{
		led1 = 1;
		LEDPORT.OUTTGL = 0xC0;             //Toggle leds 2&3.
	} // end	if (testing_flags.clk == 1)
	
	//else if (testing_flags.dac == 1) {
	//	dac_test();
	//} // end	else if (testing_flags.dac == 1) {

	TCC0.INTFLAGS = TC0_OVFIF_bm;
		//Clear the IF by writing a logical 1 to the flag
}
*/	
//-----------------------------------------------------------------------------
void test() {
	testing_flags.leds			= 0;
	testing_flags.clk			= 0;
	testing_flags.usart			= 1;
	testing_flags.usart_rx_msg  = 0;
	testing_flags.dac			= 0;
	testing_flags.spi_init		= 0;
	testing_flags.adc			= 0;
	testing_flags.adc_off		= 0;

	if (testing_flags.leds == 1) {
		led_test();
		testing_flags.leds = 0; //necessary to turn off so that the timer works
	}
	if (testing_flags.clk == 1) {
		clk_test();
		testing_flags.clk = 0;
	}
	if (testing_flags.usart == 1) {
		usart_test();
		testing_flags.usart = 0;
	}
	if (testing_flags.usart_rx_msg == 1) {
		//usartRxMsg_test();
		//testing_flags.usart == 0; // need repeated calls to this function.
	}
	if (testing_flags.dac == 1) {
		dac_test();
		testing_flags.dac = 0;
	}
	if (testing_flags.spi_init == 1) {
		spiInit_test();
		testing_flags.spi_init = 0;
	}
	if (testing_flags.adc == 1) {
		adc_test();
		testing_flags.adc = 0;
	}
	if (testing_flags.adc_off == 1) {
		adcOff_test();
		testing_flags.adc_off = 0;		
	}
} // end	void test() {
//-----------------------------------------------------------------------------
//eof
