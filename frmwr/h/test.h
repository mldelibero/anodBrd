// Author:	Michael DeLibero
// Date Created:	10.27.2011
// Last Modified:	10.27.2011	--	Michael DeLibero

// This is the header file for test.c.
//----------------------------------------------------------------------------- 
#ifndef test_H_
	#define test_H_
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// structs
typedef struct {
	unsigned leds:		1;
	unsigned clk:		1;	
	unsigned usart:		1;
	unsigned usart_rx_msg:	1; // test incoming message for correctness
	unsigned dac:		1;
    unsigned spi_init:  1;
	unsigned adc_off:	1; // test adc offsets
	unsigned adc:		1;
} testing_flags_t;
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------- 
//Function Prototypes
//-----------------------------------------------------------------------------
void led_test(void);
/*
	This function tests the LEDs by turning them all on.
	The four leds in question are located to the left of the micro and pshbut
	
	Dependencies:
		init.c
			init_leds()
			
	Notes:
		There are only four leds on the circuit board connected directly to the
		micro. The rest cannot be tested here.
*/
//-----------------------------------------------------------------------------
void clk_test(void);
/*
	This function will test the system clock for accuracy. An ISR will be set
	up that will toggle an led at some frequency. Probe the anode of the led
	with an oscilloscope capable of measuring that frequency.
	
	Dependencies:
		ISR(TCC0_OVF_vect)
		test_leds
			The leds must be able to be commanded on by the program.
		
	Test:
		Was the system set to 32MHz properly?
	
	Analysis:
		The following criteria must be met for the test to pass.
		
		ch 0:	4 MHz pulse (32Mhz / 2 istr per period / 4 inst per toggle
		ch 1:	Always on
		ch 2,3:	
*/
//-----------------------------------------------------------------------------
void usartTx_test(void);
	/*
		This function tests the usart transmission setup by repeatedly sending
		0xC4 at a given frequency.
		
		Note:
			This function has not yet been written.
	*/
//-----------------------------------------------------------------------------
void dac_test(void);
/*
	This function tests the external dacs on the Anod V2.1 circuit board.
	All DACs should have identical output on both channels.`
	
	Modes:
		1. Output a specific, constant command.
		2. Create a periodic, full scale ramp.
		
*/
//-----------------------------------------------------------------------------
void spiInit_test(void);
/*
 * This function tests if the spi has been initialized properly.
 * 
 * Mode:
 *      A sequence of 4 5 6 7 in binary should be seen at output of the
 *      spi pin. Hit the reset button to send the output again.
 
		Also, it is recommended that this be the only test function run when
		it is run.
 *
 *      Probe: (check to see if current pinout is accurate)	
 *          spi clk        --- Pin 23
 *          spi out (MOSI) --- Pin 21
 *       Referenced from AnodV2.sch
 */
//-----------------------------------------------------------------------------
void usart_test(void);
/*
	This function does nothing. Refer to usartTx_test() and usartRX_test() for
	description of test
*/
//-----------------------------------------------------------------------------
void usartTx_test(void);
/*
	This function tests the usart Tx setup.
	
	Modes:
		1. Output a specific byte continuously.
		2. Output the last byte received by the USARTrx continuously.
*/
//-----------------------------------------------------------------------------
void usartRxMsg_test(volatile char uRxByte);
/*
	This function test the incoming message from the PC.
	
	It copies the message to the tx buffer. You should see this msg repeated
	on the PC side.
	
	uRxByte --- USART RX BYTE
	
	Error Causes:
		1. You are not displaying the data correctly on your PC.
			Check for ASCII issues.
		2. The rx msg (PC->micro) is too long. As of this comment, the micro
			only accept data for the first two channels. Don't send data for
			channel 3 or 4.
	
*/
void usartRx_test(void);
/*
	This function tests the USART Rx setup.
	
	It stores a last byte received into a variable. This byte can be echoed
	back along the usart tx by selecting mode 2 in the usart tx	
*/
//-----------------------------------------------------------------------------
void test(void);
/*
	This function calls various testing functions based upon flags in set in
	the testing_flags bit-field.
*/
//----------------------------------------------------------------------------- 
// Typedef Externs
extern volatile testing_flags_t testing_flags;
//----------------------------------------------------------------------------- 

#endif //test_H_

