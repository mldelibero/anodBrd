/*
	File:          init.h
	Author:        Michael DeLibero
	Date Created:  10-27-11
	Last Modified: 11.09-11 - Michael DeLibero
	
	Description: 	This is the header file for init0.c.
					See init0.c for further information.
					
	References: 	None
*/
//-----------------------------------------------------------------------------
#ifndef INIT_H
	#define INIT_H
	//#include "dac.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------	
// Structs
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
typedef struct{
		uint8_t lo; // lo 8 bits
		uint8_t hi; // hi 8 bits	
} u16_parse_t;
typedef union {
	uint16_t all; // full 16 bit value
	struct {
		uint8_t lo;
		uint8_t hi;
	} parse;
}parU16_u_t; // parsed u16 union type
//-----------------------------------------------------------------------------
typedef struct {
	//Hardware Settings -- needed for the spi output to the dacs
		PORT_t *ss_port;
		unsigned int ss_pin;
	//dac commanded values
		volatile parU16_u_t vol2dac_u; // 10bit set point (0x0000 - 0x03FF)
		volatile parU16_u_t cur2dac_u; // 10bit set point (0x0000 - 0x03FF)
		uint8_t LDAC;	
	//adc values
		volatile parU16_u_t volfadc_u; // voltage from adc
		volatile parU16_u_t curfadc_u; // current from adc
	// state -- in usart -- transfer here later
	//Channel Enable (1-enabled,0-disabled)
	//Used to keep track of which channels should be turned on/off at any time	
		uint8_t en;
		uint8_t state;
		uint8_t state_change[4]; // change to a lower state when the adc
		// readings are < these numbers.
		uint8_t hys; // counter for how long the adc has been under state_change
		uint8_t hys_max; // max for hys counter.
		uint16_t pwr_max; // max power alowed to be dissipated in the pass trans
} channel_t;
//-----------------------------------------------------------------------------
typedef struct{
	unsigned setEventSource:1; // Was this function called correctly?
		struct {
		unsigned badSta:1; // Received an incorrect Start Byte
		unsigned badSto:1; // Received an incorrect Start Byte
		unsigned rxlen:1; // rx msg length not as specified
		unsigned noStop:1; // Did not receive a stop byte when expected.
	} usartRx;
	unsigned pwrLimit:1; // Too much power dissipated in the last pass transist
} eflags_t; // error flags
typedef struct  {
	unsigned rxingMsg:1; // set if currently receiving message to micro usart.
		// This will be set when rx message is in between Start & stop bytes
	unsigned newDacData:1; // New data is available to be written to the DAC.
	unsigned calib:1; // Calibrating Unit
	unsigned norm:1; // Normal operation -- system will change feedback states
					// based upon adc readings
	unsigned hys:1;
} sflags_t; // system flags type -- to be used throughout entire program
//-----------------------------------------------------------------------------
	//Bitfield for use with assigning pins to variable names
	typedef struct {
		unsigned bit0:1;
		unsigned bit1:1;
		unsigned bit2:1;
		unsigned bit3:1;
		unsigned bit4:1;
		unsigned bit5:1;
		unsigned bit6:1;
		unsigned bit7:1;
	} port_t;
	
	volatile port_t *MYPORTC;
	volatile port_t *MYPORTD;
	volatile port_t *MYPORTE;
	volatile port_t *MYPORTF;
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
#define NUM_SPI_BYTES   2 // Number of bytes needed to be sent the dacs
#define hi_state		(3)
#define me_state		(2)
#define lo_state		(1)
#define done_state		(0)
//-----------------------------------------------------------------------------
	#ifndef F_CPU /*! \brief Define default CPU frequency, if this is not already defined. */
		#define F_CPU 32000000UL
	#endif

	#define ACK_BYTE 0x99
	#define SETUP_PACKET 0x01
	#define ANOD_PACKET	0x0A
	#define TEST_PACKET 0x04
	#define NUM_SPI_BYTES   2

	// DAC #defines
	#define DAC_VOL	(0x00) //DAC selection bits
	#define DAC_CUR	(0x80)	
	#define GA_1x	(0x20) // output gain selection bit (20---1X,00---2X)
	#define GA_2x	(0x00) // output gain selection bit (20---1X,00---2X)
	#define SHDN_y	(0x00) // shutdown the selected channel
	#define SHDN_n	(0x10) // do not shutdown the selected channel
	
	#define ch1		(0)
	#define ch2		(1)
	#define ch3		(2)
	#define ch4		(3)

	#define led0	(MYPORTF->bit4)
	#define led1	(MYPORTF->bit5)
	#define led2	(MYPORTF->bit6)
	#define led3	(MYPORTF->bit7)
	
	#define EN1		(MYPORTE->bit7)
	#define EN2		(MYPORTE->bit2)
	#define EN3		(MYPORTD->bit4)
	#define EN4		(MYPORTC->bit2)
	
	// Enable pins for the relays
	#define CH1_REL_MED_EN		(MYPORTE->bit5)
	#define CH1_REL_HI_EN		(MYPORTE->bit6)
	#define CH2_REL_MED_EN		(MYPORTE->bit0)
	#define CH2_REL_HI_EN		(MYPORTE->bit1)
//-----------------------------------------------------------------------------
// typedef externs
// externs
extern volatile eflags_t eflags;
extern volatile sflags_t sflags;
//-----------------------------------------------------------------------------
extern volatile port_t *MYPORTC;
extern volatile port_t *MYPORTD;
extern volatile port_t *MYPORTE;
extern volatile port_t *MYPORTF;

extern volatile channel_t ch[4];
//-----------------------------------------------------------------------------
// variable externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void flag_init(void);
/*
	This function initializes all of the system flags. Originally, this only
	included the eflags.
*/
//-----------------------------------------------------------------------------
void port_init(void);
/*
	This function initializes all of the ports to be used.
*/
//-----------------------------------------------------------------------------
void clk_init(void);
/* Author: Steve Ehret
	This function initializes the system clock. I took it verbatim from Steve's
	code and will use it if it passes its unit tests.
*/
//-----------------------------------------------------------------------------
void init(void);
/*
	This function is the parent function for all of the initialization.
	It calls all of the other functions in order to complete the initialization.
*/
//-----------------------------------------------------------------------------
void ledup(int num);
/*
	This function takes in an int and then converts the first 4 bits into a
	binary display on the led outputs.
	Ex
		ledup(3,0)  ->> [0,0,1,1]
*/
//-----------------------------------------------------------------------------
#endif

//eof
