/*
	File:          usart.h
	Author:        Michael DeLibero
	Date Created:  11-02-11
	Last Modified: 11-09-11 - Michael DeLibero
	
	Description: 	This is the header file for usart.c.
					See usart.c for further information.
					
	References: 	None
*/
//-----------------------------------------------------------------------------
#ifndef USART_H_
#define USART_H_
//-----------------------------------------------------------------------------
// Structs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Unions
//-----------------------------------------------------------------------------
typedef union {
	uint8_t all;
	struct {
	// four reserved bits		
		unsigned res0:1;
		unsigned res1:1;
		unsigned res2:1;
		unsigned res3:1;
	// four channel enable bits
		unsigned chan1:1;
		unsigned chan2:1;
		unsigned chan3:1;
		unsigned chan4:1;
	} chEN;
	struct {
	// four msg size bits	
		unsigned sz:4;
	// four reserved bits
		unsigned res4:1;
		unsigned res5:1;
		unsigned res6:1;
		unsigned res7:1;
	} msgSize;
} chEnSz_u_t; // channel enable union type


typedef union {
	uint8_t all;
	struct {
		unsigned chan1:2;
		unsigned chan2:2;
		unsigned chan3:2;
		unsigned chan4:2;
	}state;
} state_u_t;
//-----------------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#ifndef F_CPU
/*! \brief Define default CPU frequency, if this is not already defined. */
#define F_CPU 32000000UL
#endif

#define RX_BUFFER_SIZE 12
#define ACK_BYTE 0x99
#define PACKET_SIZE 20
#define SETUP_PACKET 0x01
#define ANOD_PACKET	0x0A
#define TEST_PACKET 0x04
#define NUM_SPI_BYTES   2

#define USART_BAUDRATE	(2000000)
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define USART	(USARTF0)
#define START_BYTE	(0x97)
#define STOP_BYTE	(0x68)
#define rx_max_ind_def	(11)

//-----------------------------------------------------------------------------
// typedef externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------
extern volatile uint8_t *dat2pc_p[12]; // pointer to data 2B sent to the PC.
extern volatile state_u_t state_u;
//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void usart_init(void);
/*
	Function:		usart_init
	Author:         Michael DeLibero
	Description: 	This function initializes the usart
	
	Depedencies: 	clk_init
	
	Run Afterwards:	
		
	References:		 
	
	Details:
*/
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
