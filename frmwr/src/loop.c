// Author:	Michael DeLibero
// Date Created:	10.27.2011

// This file contains the infinite loop used in Anod V3.0
//-----------------------------------------------------------------------------
#include <avr/io.h>
#include "avr_compiler.h"
#include "loop.h"
#include "init.h"
#include "usart.h"
#include "TC_driver.h"
#include "dac.h"

// States correspond to the format required in the u-PC transmission as well
#define hi_state	(3)
#define me_state	(2)
#define lo_state	(1)

int i = 0; // index for loop
//-----------------------------------------------------------------------------
void loop_init(void) {
	TC0_ConfigClockSource(&TCE0,TC_CLKSEL_DIV1024_gc); // TCC0.CTRLA = TC_CLKSEL_DIV1024_gc; // Set clock prescaler (14.12.1)
	TC_SetPeriod(&TCE0,3125); // TCC0.PER = 1563; // Set Clock period to 10Hz
	TC0_SetOverflowIntLevel(&TCE0,TC_OVFINTLVL_LO_gc);  // TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc; // enable timer overflow int and
	// Make sure to always use |= with PMIC.CTRL so you don't overwrite other
	// settings
	PMIC.CTRL |= PMIC_LOLVLEN_bm; // Enable Low Level interrupts (12.8.3)
} // void loop_init(void) {
//-----------------------------------------------------------------------------
void powerCurve_init(void) {
	TC0_ConfigClockSource(&TCF0,TC_CLKSEL_DIV1024_gc); // Set clock prescaler (14.12.1)
	TC_SetPeriod(&TCF0,1562); // Set Clock period to 19.99Hz
	TC0_SetOverflowIntLevel(&TCF0,TC_OVFINTLVL_LO_gc);  // enable timer overflow int and
	// Make sure to always use |= with PMIC.CTRL so you don't overwrite other
	// settings
	PMIC.CTRL |= PMIC_LOLVLEN_bm; // Enable Low Level interrupts (12.8.3)	
} // end	void powerCurve_init(void) {
//-----------------------------------------------------------------------------
ISR(TCF0_OVF_vect) {
/*
	static uint16_t pwr = 0;
	static uint8_t num = 0;
	
	for (i = 1;i<1;i++) {
		// Pdiss = ((Vset-Imeas)-Vmeas)*Imeas
		// Power dissipation in last pass transistor.
		pwr = ch[i].curfadc_u.all*(ch[i].vol2dac_u.all-ch[i].curfadc_u.all-ch[i].volfadc_u.all);
		if (pwr > ch[i].pwr_max) {
			// Danger Will Robinson
			// Decrease current cmd until a safe power level has been reached
			ch[i].cur2dac_u.all = ch[i].cur2dac_u.all - 10;
			sflags.newDacData = 1;
		} // end	if (pwr > ch[i].pwr_max) {
	} // end	for (i = 1;i<1;i++) {
*/
} // end	ISR(TCE0_OVF_vect) {
//-----------------------------------------------------------------------------
ISR(TCE0_OVF_vect) {
	sflags.hys = 1;
} // end	ISR(TCE0_OVF_vect) {
//-----------------------------------------------------------------------------
void loop() {
	led2 =  ch[ch2].state&1;
	led3 = (ch[ch2].state&2)>>1;
	
	if (sflags.newDacData == 1) {
		//then we need to update the output values on the DACs
		sflags.newDacData = 0;
		if (ch[ch1].en == 1) {
			dac_up(DAC_VOL,GA_2x,SHDN_n,ch1,&ch[0]);
			dac_up(DAC_CUR,GA_1x,SHDN_n,ch1,&ch[0]);
		}
		if (ch[ch2].en == 1) {
			dac_up(DAC_VOL,GA_2x,SHDN_n,ch2,&ch[0]);
			dac_up(DAC_CUR,GA_1x,SHDN_n,ch2,&ch[0]);
		}
	} // end	if (sflags.newDacData == 1) {

	if (sflags.norm == 1) {
		// Change the relays and state if necessary
		// This currently only lets the states decrease.
		// Add in the ability to increase through the states later
		int i = 0;
		if (sflags.hys == 1) { // 10 Hz timer flag
			sflags.hys = 0;
//			led0 = num;
//			num ^=1;
			
			for (i=1;i<2;i++) {	// count through each channel		
				if ((ch[i].en == 1) && ((ch[i].curfadc_u.all <= ch[i].state_change[ch[i].state]) \
                                    || (ch[i].curfadc_u.all > 2047)) && (ch[i].state != lo_state)) {				
					ch[i].hys++;
				
					if (ch[i].hys > ch[i].hys_max) {
						ch[i].hys = 0;
						ch[i].state = ch[i].state - 1;
					
						if (i == 0) {
							CH1_REL_MED_EN = (ch[i].state-1)&1;
							CH1_REL_HI_EN  = (ch[i].state-1)&2>>1;
							state_u.state.chan1 = ch[i].state; 
						}
						else if (i==1) {
							CH2_REL_MED_EN = (ch[i].state-1)&1;
							CH2_REL_HI_EN  = ((ch[i].state-1)&2)>>1;					
							state_u.state.chan2 = ch[i].state;							
						}
					} // end	if (ch[i].hys > ch[i].hys_max) {			
				} // end	if "ch enabled && adc + num>10 && not in lowest state"
				else { // We need to decrement hys cnter
					if (ch[i].hys > 0) {
						ch[i].hys -= 1;
					}
				} // end	else										
			} // end	for (i=0;i<2;i++) {		 
		} // end	if (sflags.hys == 1) { // 10 Hz timer flag
	} // end	if (sflags.norm == 1) {
} // end	void loop() {
//-----------------------------------------------------------------------------
