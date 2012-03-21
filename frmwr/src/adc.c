/*
	File:          adc.c
	Author:        Michael DeLibero
	Date Created:  11.17.11
	
	Description: 	This file contains the functions pertaining to the adc code
					for the ADCv3 proj.
					
	References:
		*XmegaA datasheet sect. 36.2.1 -- Errata	 	
		*Atmel app note #1300
		*Anod V2.0
		*adc_driver.c,"".h
*/
//-----------------------------------------------------------------------------
#include <avr/io.h>
#include "avr_compiler.h"
#include "adc.h"
#include "adc_driver.h"
#include "event_system_driver.h"
#include "TC_driver.h"
#include "init.h"
#include "test.h"
//-----------------------------------------------------------------------------
//file scope defines
//-----------------------------------------------------------------------------
#define signed_y	(true)
#define signed_n	(false)
//-----------------------------------------------------------------------------
//file scope variables
//-----------------------------------------------------------------------------
ADC_CH_MUXPOS_t ADCA_mux,ADCB_mux; // used to select input pins.
volatile int8_t offset_A = 0x12;
volatile int8_t offset_B = 0x34;
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
void adc_init(void) {
/*
	Note: port_init() must be run before this function, so that the inputs are
	set correctly.
	
	We are using both ADCs. So everything will be set up for ADCA && ADCB.
*/

// Load the production calibration data into each ADC.
// This data was taken by Atmel and is stored in the micro.
// This function takes care of the whole process for you.
	ADC_CalibrationValues_Load(&ADCA);
	ADC_CalibrationValues_Load(&ADCB);
	
// Set the mode of operation for each ADC
// Signed operation mode is required for the differential configuration.
	ADC_ConvMode_and_Resolution_Config(&ADCA,signed_y,ADC_RESOLUTION_12BIT_gc);
	ADC_ConvMode_and_Resolution_Config(&ADCB,signed_y,ADC_RESOLUTION_12BIT_gc);
	
// Set ADC clocks
// 32MHz / 128 = 250KHz
// I currently have no explanation for this choice
// Atmel documentation states that you need to stay within the recommended
// ADC frequencies, but I cannot find the specific numbers.
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV128_gc);
	ADC_Prescaler_Config(&ADCB, ADC_PRESCALER_DIV128_gc);
	
// Select reference to be external reference on PIN0 for A and B
	ADC_Reference_Config(&ADCA, ADC_REFSEL_AREFA_gc);
	ADC_Reference_Config(&ADCB, ADC_REFSEL_AREFB_gc);

// Setup all channels to have differential input and 1X gain
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0,ADC_CH_INPUTMODE_DIFF_gc,
	                                  ADC_CH_GAIN_1X_gc); // V1
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH1,ADC_CH_INPUTMODE_DIFF_gc,
	                                  ADC_CH_GAIN_1X_gc); // V2
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH2,ADC_CH_INPUTMODE_DIFF_gc,
	                                  ADC_CH_GAIN_1X_gc); // reference
									  
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH0,ADC_CH_INPUTMODE_DIFF_gc,
	                                  ADC_CH_GAIN_1X_gc); // I1
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH1,ADC_CH_INPUTMODE_DIFF_gc,
	                                  ADC_CH_GAIN_1X_gc); // I2
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH2,ADC_CH_INPUTMODE_DIFF_gc,
	                                  ADC_CH_GAIN_1X_gc); // reference								  									 								  

// Select the input pins for each ADC.
/*
	See AnodV2.1.sch eagle file:
	V1 - A1
	V2 - A2
	I1 - B1
	I2 - B2
	Ref - A0,A3,B0,B3
*/
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN1_gc, \
									  ADC_CH_MUXNEG_PIN3_gc); // V1
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_MUXPOS_PIN2_gc, \
									  ADC_CH_MUXNEG_PIN3_gc); // V2
	ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_MUXPOS_PIN3_gc, \
									  ADC_CH_MUXNEG_PIN3_gc); // Offset calib
			
	ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_MUXPOS_PIN1_gc, \
									  ADC_CH_MUXNEG_PIN3_gc); // V1
	ADC_Ch_InputMux_Config(&ADCB.CH1, ADC_CH_MUXPOS_PIN2_gc, 
									  ADC_CH_MUXNEG_PIN3_gc); // V2
	ADC_Ch_InputMux_Config(&ADCB.CH2, ADC_CH_MUXPOS_PIN3_gc,
									  ADC_CH_MUXNEG_PIN3_gc); // Offset calib

// Configure the ADCA.CH2 interrupt.
// This will trip once a reading on channel 2 has been completely resolved.
// I am assuming the chan 0 and 1 of both ADCs will have their results completed
// when chan 2 is done. This is based from the Xmega A manual (sect 25)									  
	ADC_Ch_Interrupts_Config(&ADCA.CH1, ADC_CH_INTMODE_COMPLETE_gc, \
									    ADC_CH_INTLVL_LO_gc);

//Enable ADCs
	ADC_Enable(&ADCA);
	ADC_Enable(&ADCB);								

// Wait until common mode voltage is stable so tha bypass transients are not passed.
// What is the difference between the 32 and 8 MHz versions, 
// and which one do I want?
	ADC_Wait_32MHz(&ADCA);
	ADC_Wait_32MHz(&ADCB);									  

// The TCD0 timer will periodically trigger an event that will  create an event
// on channel 0. (Xmega A manual sect 6)
	eflags.setEventSource = EVSYS_SetEventSource(0, EVSYS_CHMUX_TCD0_OVF_gc);
	TC0_ConfigClockSource(&TCD0, TC_CLKSEL_DIV8_gc);
	TCD0.PER = 200; // 1/f = 1/(32MHz/DIVx/PER) --- 200---50us---20KHz

// This is moved to the adc_test() fun. May want to enable it here later.
	// enable timer overflow int and set priority to low.
	//TCD0.INTCTRLA = TC_OVFINTLVL_LO_gc; 

// An event on eventChan 0 will trigger a sweep of chan 0,1 in ADCA && ADCB.
// I do not know what would happen if an event happened on eventChan 1,2,3.
	ADC_Events_Config(&ADCA, ADC_EVSEL_0123_gc, ADC_EVACT_SWEEP_gc);	
	ADC_Events_Config(&ADCB, ADC_EVSEL_0123_gc, ADC_EVACT_SWEEP_gc);
	ADC_SweepChannels_Config(&ADCA, ADC_SWEEP_01_gc);
	ADC_SweepChannels_Config(&ADCB, ADC_SWEEP_01_gc);
	
	
// Calibration routine for the ADCs
// Find offset with two pins shorted together.
// Steve also found the offset for the current with 0 current flowing into them,
// but did not do an equivalent for voltage. I will leave this out for now.
	offset_A = ADC_Offset_Get_Signed(&ADCA, &(ADCA.CH2), true);
	offset_B = ADC_Offset_Get_Signed(&ADCB, &(ADCB.CH2), true);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ISRs
//-----------------------------------------------------------------------------

ISR(ADCA_CH1_vect) {
	if ((testing_flags.adc_off == 0) && (testing_flags.usart_rx_msg == 0)) {
		ch[ch1].volfadc_u.all = ADCA_CH0RES - offset_A;
		ch[ch1].curfadc_u.all = ADCB_CH0RES - offset_B;				
		ch[ch2].volfadc_u.all = ADCA_CH1RES - offset_A;
		ch[ch2].curfadc_u.all = ADCB_CH1RES - offset_B;
		// (25.16.6) The int flag gets auto cleared when the ISR is executed
	}
} // end	ISR(ADCA_CH1_vect) {
//-----------------------------------------------------------------------------
ISR(TCD0_OVF_vect){
	// This isr will only execute when it is enabled within the adc_test fun.
}
//-----------------------------------------------------------------------------
// eof
