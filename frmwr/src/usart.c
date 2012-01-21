/*
	File:          usart.c
	Author:        Michael DeLibero
	Date Created:  11-02-11
	
	Description:	USART functions for the Anod v3.0 project.
					The USRT is to be used for comm with the FTDI chip to
					perform comm with the computer.
					
	References: 	
	
*/
//-----------------------------------------------------------------------------
#include <avr/io.h>
#include "usart.h"
#include "usart_driver.h"
#include "init.h"
#include "test.h"
//-----------------------------------------------------------------------------
//file scope defines
//-----------------------------------------------------------------------------
#ifndef F_CPU
/*! \brief Define default CPU frequency, if this is not already defined. */
#define F_CPU 23000000UL
#endif

//-----------------------------------------------------------------------------
//file scope variables
//-----------------------------------------------------------------------------
volatile char packet_rx_flag;						// Set to 1 when full packet has been received
volatile char waiting_for_ack;
volatile char tx_flag;								// Set to 1 when full packet is ready to be sent 
volatile char tx_sending_packet;					//Tx buffer sending
volatile int tx_current_byte;
volatile char waiting_for_ack;
volatile int8_t tx_buffer[PACKET_SIZE];

USART_data_t USART_data;
volatile unsigned int rx_ind = 0; // index of current rx byte in rx buffer
volatile unsigned char rx_buff[RX_BUFFER_SIZE];		// Buffer for Receive
volatile char rxByte;							// Most recent received byte

char rx_max_ind = rx_max_ind_def; // Maximum expected index for a usart rx message.
					 // set in usart rx 
					 // Initialize to > 2 initially or you will never parse
					 // data.

// This data will be continuously set to the PC. The vol and curr values
// be updated when new values are resolved by the adcs. The state values
// will be updated when a new command is received from the PC.
volatile uint8_t *dat2pc_p[12]; // pointer to data 2B sent to the PC.
volatile uint8_t tempDat2Pc[12]; // snapshot of dat2pc_p needed so that none of
							// the vars change while sending out a msg. The vol
							// and cur are of special concern, as they are sent
							// out as two 8 bit msgs, while occupying 16bits
							// in the micro

// Using Unions like this is bad practice, as there is no guarantee of order
// with different compilers. Change later.


volatile chEnSz_u_t chEn_size_u;
volatile state_u_t state_u;
	
volatile uint8_t cnt2pc = 1; // Counter for tx data to the PC via usart 2 FTDI
volatile uint8_t cnt2pc_max = 11; // Max length of msg to Tx to PC

// Defined so that we can use these values in pointers
uint8_t start = START_BYTE;
uint8_t stop = STOP_BYTE;


//-----------------------------------------------------------------------------
// file defined, global scope variables
//-----------------------------------------------------------------------------
// function pointer externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void usart_init(void) {
// Initialize vars that will be sent to the PC
	chEn_size_u.all = 0; // Initialize all chEN bits to zero.
	
	chEn_size_u.msgSize.sz = 6; // Message length of 6 alls (16bits/all)
	state_u.all = 0XFF; // Initialize all states to high current for protection
						// From high initial currents.
												
	// Initialize data to PC pointer array.
	dat2pc_p[0]  = &start;
	dat2pc_p[1]  = &chEn_size_u.all;
	dat2pc_p[2]  = &state_u.all;
	// channel 1 values
	dat2pc_p[3]  = &ch[ch1].volfadc_u.parse.hi;
	dat2pc_p[4]  = &ch[ch1].volfadc_u.parse.lo;
	dat2pc_p[5]  = &ch[ch1].curfadc_u.parse.hi;
	dat2pc_p[6]  = &ch[ch1].curfadc_u.parse.lo;
	//channel 2 values	
	dat2pc_p[7]  = &ch[ch2].volfadc_u.parse.hi;
	dat2pc_p[8]  = &ch[ch2].volfadc_u.parse.lo;
	dat2pc_p[9]  = &ch[ch2].curfadc_u.parse.hi;
	dat2pc_p[10] = &ch[ch2].curfadc_u.parse.lo;
	//stop byte
	dat2pc_p[11] = &stop;//STOP_BYTE;

    // Used to keep a snapshot of all data
	tempDat2Pc[0]  = *dat2pc_p[0];
	tempDat2Pc[1]  = *dat2pc_p[1];
	tempDat2Pc[2]  = *dat2pc_p[2];
	tempDat2Pc[3]  = *dat2pc_p[3];
	tempDat2Pc[4]  = *dat2pc_p[4];
	tempDat2Pc[5]  = *dat2pc_p[5];
	tempDat2Pc[6]  = *dat2pc_p[6];
	tempDat2Pc[7]  = *dat2pc_p[7];
	tempDat2Pc[8]  = *dat2pc_p[8];
	tempDat2Pc[9]  = *dat2pc_p[9];
	tempDat2Pc[10] = *dat2pc_p[10];
	tempDat2Pc[11] = *dat2pc_p[11];
	
	/* Use USARTC0 and initialize buffers. */
	USART_InterruptDriver_Initialize(&USART_data, &USART, USART_DREINTLVL_OFF_gc);
	
	/* USARTC0, 8 Data bits, ODD Parity, 1 Stop bit. */
	USART_Format_Set(USART_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_ODD_gc, false);
	/* Enable RXC interrupt. */
	USART_RxdInterruptLevel_Set(USART_data.usart, USART_RXCINTLVL_HI_gc);
	/* Enable TXC interrupt. */
	USART_TxdInterruptLevel_Set(USART_data.usart, USART_TXCINTLVL_HI_gc);
	
	/* Set Baudrate to 2000000 bps:
	 * Use the PLL I/O clock frequency that is 32 MHz.
	 * Do not use the baudrate scale factor
	 *
	 * Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
	 *                 = 0
	 */
	USART_Baudrate_Set(&USART, BAUD_PRESCALE, 0);

	/* Enable both RX and TX. */
	USART_Rx_Enable(USART_data.usart);
	USART_Tx_Enable(USART_data.usart);

	/* Enable PMIC interrupt level low. */
	PMIC.CTRL |= PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm;
	
	USARTF0.DATA = 0x04;
}
//-----------------------------------------------------------------------------
void parseRxMsg(void) {
	/*
		1. Check start and stop bytes
		2. Check for proper length
		3. Update Enables
		4. Update New Values (vol,cur)
	*/	
	chEn_size_u.all = rx_buff[1];
	ch[ch1].en = chEn_size_u.chEN.chan1;
	ch[ch2].en = chEn_size_u.chEN.chan2;
		
	// rx_buff[2] --- see below
	
	ch[ch1].vol2dac_u.parse.hi = rx_buff[3];
	ch[ch1].vol2dac_u.parse.lo = rx_buff[4];
	ch[ch1].cur2dac_u.parse.hi = rx_buff[5];
	ch[ch1].cur2dac_u.parse.lo = rx_buff[6];
	
	ch[ch2].vol2dac_u.parse.hi = rx_buff[7];
	ch[ch2].vol2dac_u.parse.lo = rx_buff[8];
	ch[ch2].cur2dac_u.parse.hi = rx_buff[9];
	ch[ch2].cur2dac_u.parse.lo = rx_buff[10];
				
	sflags.newDacData = 1; // Let the main loop know to update the DACs
	
	if ((rx_buff[2] & 0xF0) == 0x10) {// Normal Operation
		// Switch into high state
		sflags.calib = 0;
		sflags.norm = 1;
	
		ch[ch2].state = hi_state;
		state_u.state.chan2 = ch[ch2].state;
		chEn_size_u.chEN.chan2 = 1;
		ch[ch2].en = 1;
		
		CH1_REL_MED_EN = 0;
		CH1_REL_HI_EN  = 1;
		CH2_REL_MED_EN = 0;
		CH2_REL_HI_EN  = 1;
	}
	if ((rx_buff[2] & 0xF0) == 0x20) {// Voltage source calibration
		// Turn on all relays to protect against high currents.
		// Although user should not be allowing I to flow into meas system.
		sflags.calib = 1;
		sflags.norm = 0;
		CH1_REL_MED_EN = 1;
		CH1_REL_HI_EN  = 1;
		CH2_REL_MED_EN = 1;
		CH2_REL_HI_EN  = 1;
	}
	else if ((rx_buff[2] & 0xF0) == 0x30) {// Voltage measurement calibration
		// Turn on all relays to protect against high currents.
		sflags.calib = 1;
		sflags.norm = 0;
		CH1_REL_MED_EN = 1;
		CH1_REL_HI_EN  = 1;
		CH2_REL_MED_EN = 1;
		CH2_REL_HI_EN  = 1;
	}
	else if ((rx_buff[2] & 0xF0) == 0x40) {// Current Source Calibration
	// Turn on all relays to protect against high currents.
		sflags.calib = 1;
		sflags.norm = 0;
		CH1_REL_MED_EN = 1;
		CH1_REL_HI_EN  = 1;
		CH2_REL_MED_EN = 1;
		CH2_REL_HI_EN  = 1;
	}
	else if (((rx_buff[2] & 0xF0) == 0x50) || ((rx_buff[2] & 0xF0) == 0x60)) {
	// Current Measurement Calibration
	// ||
	// Relay Control
	
	// Set Relays so We don't blow out our measurement channels.
	// 0x(mode)(special bits)
	// 0x(mode)(ch2hi ch2med ch1hi ch1med)
		
		sflags.calib = 1;
		sflags.norm = 0;
		
		ch[ch1].state =  (rx_buff[2]&3)		 + 1;
		ch[ch2].state = ((rx_buff[2]&12)>>2) + 1;
		state_u.state.chan1 = ch[ch1].state;
		state_u.state.chan2 = ch[ch2].state;
		
		CH1_REL_MED_EN = (rx_buff[2] & 0x01) >> 0;
		CH1_REL_HI_EN  = (rx_buff[2] & 0x02) >> 1;
		CH2_REL_MED_EN = (rx_buff[2] & 0x04) >> 2;
		CH2_REL_HI_EN  = (rx_buff[2] & 0x08) >> 3;	
	} // end	else if Curr Meas ctrl of relay ctrl
} // end	void parseTxMsg(void) {
//-----------------------------------------------------------------------------
// ISRs
//-----------------------------------------------------------------------------
ISR(USARTF0_RXC_vect)
{
	/*
		This ISR will be executed when there is a byte is the USART rx buffer.
		It places bytes in an array until a complete message has been retrieved
		
		1. Store byte in data buffer to rxByte.
		2. Check to see if the rx byte is Start byte iff not in the middle of
			an rx (rxingMsg != 1)
			
			a. Yes -->  set rxingMsg = 1
						set rx_ind = 0; // only ever set here to eliminate errors
						store incoming byte to first index in rx_buff
						increment rx_ind
			b. No  --> Ignore incoming byte
		3. If at 2nd byte (rx_ind == 1), then store the expected message length
			This is used to check if too many bytes have been rxed before 
			getting a stop byte.
		4. check if rx_ind is at the last position and rx_byte is STOP_BYTE
			a. Yes -->  then store the byte in the rx_buffer array
						set rxingMsg = 0
						call parseRxMsg();
			b. No  -->	then we have an error, we will ignore this message
						set rxingMsg = 0
		5. Check to see if rx_ind > rx_max_ind
			a. yes
				set rxingMsg = 0
		5. if rx_ind < msgLen && rxingMsg == 1
			We should be in the middle of a message.
			store rx_byte to rx_buff						
	*/
	
	rxByte = USARTF0.DATA;
	if (testing_flags.usart_rx_msg == 1) {
		usartRxMsg_test(rxByte);
	}
	
	else {
		if (sflags.rxingMsg == 0) { // was previously not rxing a msg.
			if (rxByte == START_BYTE) {
				rx_ind = 0;
				rx_buff[rx_ind] = rxByte;
				sflags.rxingMsg = 1;
				rx_ind++;
			}			
		}
		else {
			if ((rx_ind == rx_max_ind) && (rxByte == STOP_BYTE)) {
				rx_buff[rx_ind] = rxByte;
				sflags.rxingMsg = 0;
				parseRxMsg();
			} // end	else if ((rx_ind == rx_max_ind) && (rx_buff == STOP_BYTE)) {
		
			else {
				rx_buff[rx_ind] = rxByte;
				rx_ind++;
			} // end	else
		} // end	else
	} // end	else
} // end	ISR(USARTF0_RXC_vect)
//-----------------------------------------------------------------------------
ISR(USARTF0_TXC_vect){
	static uint8_t cnt = 0;
	#define max	(12)
	
	if (testing_flags.usart == 1) {
		usartTx_test();	
	}		
	else {
		// Continuously cycle through all of the data to be sent to the PC and send
		// out one byte at a time.
		USARTF0.DATA = tempDat2Pc[cnt];
		
		cnt++;
		if (cnt >= max) {
			cnt = 0;
			// take a snapshot of the data so that
			// it does not change in the middle of a transmission.
		    tempDat2Pc[0]  = *dat2pc_p[0];
			tempDat2Pc[1]  = *dat2pc_p[1];
			tempDat2Pc[2]  = *dat2pc_p[2];
			tempDat2Pc[3]  = *dat2pc_p[3];
			tempDat2Pc[4]  = *dat2pc_p[4];
			tempDat2Pc[5]  = *dat2pc_p[5];
			tempDat2Pc[6]  = *dat2pc_p[6];
			tempDat2Pc[7]  = *dat2pc_p[7];
			tempDat2Pc[8]  = *dat2pc_p[8];
			tempDat2Pc[9]  = *dat2pc_p[9];
			tempDat2Pc[10] = *dat2pc_p[10];
			tempDat2Pc[11] = *dat2pc_p[11];
		} // end	if (cnt2pc >= cnt2pc_max)
	} // end	else {
} // end	ISR(USARTF0_TXC_vect)
//-----------------------------------------------------------------------------
// eof
