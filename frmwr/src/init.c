/*
	File:          init.c
	Author:        Michael DeLibero
	Date Created:  11.04.11
	
	Description: 	This file contains the initialization functions for the 
                    AnodV3 Proj. 
					
	References: 	
*/
//-----------------------------------------------------------------------------
#include <avr/io.h>
#include "avr_compiler.h"
#include "clksys_driver.h"
#include "init.h"
#include "spi.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "loop.h"
//-----------------------------------------------------------------------------
//file scope defines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//file scope variables
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// file defined, global scope variables
//-----------------------------------------------------------------------------
//These port defs will allow you to acces the individual bits of the ports.
//The bits will be referenced in macros throughout this project.
volatile port_t *MYPORTC = (&PORTC.OUT);
volatile port_t *MYPORTD = (&PORTD.OUT);
volatile port_t *MYPORTE = (&PORTE.OUT);
volatile port_t *MYPORTF = (&PORTF.OUT);
//This contains all the data needed to know the complete state of each ch.
volatile channel_t ch[4];
// error flags for the entire program.
volatile eflags_t eflags;
volatile sflags_t sflags;
//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// function pointer externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void flag_init(void) {
    // unset all of the flags used in the program.
	eflags.setEventSource = 0;
	eflags.usartRx.rxlen  = 0;
	eflags.usartRx.badSta = 0;
	eflags.usartRx.badSto = 0;
	eflags.usartRx.noStop = 0;	
	eflags.pwrLimit = 0;
	
	sflags.rxingMsg   = 0;
	sflags.newDacData = 0;
	sflags.calib = 0;
	sflags.norm = 0;
	sflags.hys = 0;
} // end flag_init()

//-----------------------------------------------------------------------------
void port_init(void) {
    // Initialize the port io settings
	PORTA.DIRCLR = 0xFF; // Set ADC pins as inputs
	PORTB.DIRCLR = 0XFF; // Set ADC pins as inputs
	
    PORTC.DIRSET |= 0xFF; // Set the rest of the pins as outputs
    PORTD.DIRSET |= 0xFF;
    PORTE.DIRSET |= 0xFF;
    PORTF.DIRSET |= 0b11111011;
    PORTF.DIRCLR = PIN2_bm; // except usart Tx pin as an input.
} // end	port_init()

//-----------------------------------------------------------------------------
void chan_init(void) {
	
	ch[ch1].ss_port = &PORTF;
	ch[ch2].ss_port = &PORTE;
	ch[ch3].ss_port = &PORTE;
	ch[ch4].ss_port = &PORTD;
	
	ch[ch1].ss_pin = PIN0_bm;		
	ch[ch2].ss_pin = PIN3_bm;
	ch[ch3].ss_pin = PIN3_bm;	
	ch[ch4].ss_pin = PIN0_bm;
	
	ch[ch1].vol2dac_u.all = 0x0000;
	ch[ch2].vol2dac_u.all = 0x0000;
	ch[ch3].vol2dac_u.all = 0x0000;
	ch[ch4].vol2dac_u.all = 0x0000;
	
	ch[ch1].cur2dac_u.all = 0x0000;
	ch[ch2].cur2dac_u.all = 0x0000;
	ch[ch3].cur2dac_u.all = 0x0000;
	ch[ch4].cur2dac_u.all = 0x0000;
	
	ch[ch1].volfadc_u.all = 0x1737; // voltages from adc
	ch[ch2].volfadc_u.all = 0x1838;
	ch[ch3].volfadc_u.all = 0x1939;
	ch[ch4].volfadc_u.all = 0x2A4A;
	ch[ch1].curfadc_u.all = 0xCCDD; // currents from adc
	ch[ch2].curfadc_u.all = 0xEEFF;
	ch[ch3].curfadc_u.all = 0x2D4D;
	ch[ch4].curfadc_u.all = 0x2E4E;
		
	ch[ch1].LDAC = LDAC1;
	ch[ch2].LDAC = LDAC2;
	ch[ch3].LDAC = LDAC3;
	ch[ch4].LDAC = LDAC4;
	
	ch[ch1].en = 0;
	ch[ch2].en = 0;
	ch[ch3].en = 0;
	ch[ch4].en = 0;
	
	ch[ch1].state = hi_state;
	ch[ch2].state = hi_state;
	ch[ch3].state = hi_state;
	ch[ch4].state = hi_state;
	
	ch[ch1].state_change[1] = 1; // lo
	ch[ch1].state_change[2] = 1; // med
	ch[ch1].state_change[3] = 1; // hi
	
	ch[ch2].state_change[1] = 10; // lo
	ch[ch2].state_change[2] = 10; // med
	ch[ch2].state_change[3] = 10; // hi
	
	ch[ch3].state_change[1] = 1; // lo
	ch[ch3].state_change[2] = 1; // med
	ch[ch3].state_change[3] = 1; // hi
	
	ch[ch4].state_change[1] = 1; // lo
	ch[ch4].state_change[2] = 1; // med
	ch[ch4].state_change[3] = 1; // hi
	
	ch[ch1].hys = 0;
	ch[ch2].hys = 0;
	ch[ch3].hys = 0;
	ch[ch4].hys = 0;
	
	ch[ch1].hys_max = 10;
	ch[ch2].hys_max = 10;
	ch[ch3].hys_max = 100;
	ch[ch4].hys_max = 100;
	
	ch[ch1].pwr_max = 1000;
	ch[ch2].pwr_max = 1000;
	ch[ch3].pwr_max = 1000;
	ch[ch4].pwr_max = 1000;
}
//-----------------------------------------------------------------------------
void clk_init(void) {
	// Sets up clocks, external 16MHz Clock Source, &&
	// use PLL to double to 32MHz for CPU clock	
	CLKSYS_XOSC_Config( OSC_FRQRANGE_12TO16_gc, false, OSC_XOSCSEL_EXTCLK_gc );
	CLKSYS_Enable( OSC_XOSCEN_bm );
	do {} while ( CLKSYS_IsReady( OSC_XOSCRDY_bm ) == 0 );
	CLKSYS_PLL_Config(  OSC_PLLSRC_XOSC_gc, 2);
	CLKSYS_Enable( OSC_PLLEN_bm );
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );
	do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );	
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );
	CLKSYS_Disable( OSC_XOSCEN_bm );
}
//-----------------------------------------------------------------------------
void init(void) {
	cli();
	
	CH1_REL_MED_EN = 1;
	CH1_REL_HI_EN = 1;
	CH2_REL_MED_EN = 1;
	CH2_REL_HI_EN = 1;
	
	flag_init();
	port_init();
	clk_init();
	chan_init();
	spi_init();
	usart_init();
	adc_init();
    loop_init();
//	powerCurve_init();

	sei();
	dac_init(); // SPI uses an interrupt so it must come after here.
	

	//Blink some LEDs to show we're done initializing everything	
	int i;
	for(i = 0; i < 10; i++){
		led0 ^= 1;
		delay_us(20000);
		led1 ^= 1;
		delay_us(20000);
		led2 ^= 1;
		delay_us(20000);
		led3 ^= 1;
		delay_us(20000);
	}
	
	// Leds reflect the states of channels 1&&2
/*	led0 = ch[ch1].state&&1;
	led1 = ch[ch1].state&&2>>1;
	led2 = ch[ch2].state&&1;
	led3 = ch[ch2].state&&2>>1;
*/	
	USARTF0.DATA = 0x04;
	
}
//-----------------------------------------------------------------------------
void ledup(int num) {
	led0 = ((num & 1) >> 0);
	led1 = ((num & 2) >> 1);
	led2 = ((num & 4) >> 2);
	led3 = ((num & 8) >> 3);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ISRs
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// eof
