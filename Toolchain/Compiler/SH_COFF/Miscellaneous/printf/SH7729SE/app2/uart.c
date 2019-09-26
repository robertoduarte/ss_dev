/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 1-Feb-2004.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************/


#include "uart.h"

#ifdef SH7615SE
#define RBR   (*(volatile unsigned short *)(0x020007F0))
#define THR   (*(volatile unsigned short *)(0x020007F0))
#define DLL   (*(volatile unsigned short *)(0x020007F0))
#define DLM   (*(volatile unsigned short *)(0x020007F2))
#define IER   (*(volatile unsigned short *)(0x020007F2))
#define IIR   (*(volatile unsigned short *)(0x020007F4))
#define FCR   (*(volatile unsigned short *)(0x020007F4))
#define LCR   (*(volatile unsigned short *)(0x020007F6))
#define MCR   (*(volatile unsigned short *)(0x020007F8))
#define LSR   (*(volatile unsigned short *)(0x020007FA))
#define MSR   (*(volatile unsigned short *)(0x020007FC))
#define SCR   (*(volatile unsigned short *)(0x020007FE))

#else
#ifdef SH7729SE

/*****************************/
/* Solution Engine Registers */
/*****************************/
#define	 ULTRA_IO_BASE	0xB0400000	/* Ultra I/O base address */
#define  CONF_ADDR	0x3f0		/* Ultra I/O configuration poart address */
#define	 INDEX_ADDR	0x3f0		/* Ultra I/O index port address */
#define  DATA_ADDR	0x3f1		/* Ultra I/O data port address */
#define  UART0_ADDR	0x3f8		/* Ultra I/O uart ch0 base address */
/* configuration */
#define  CONF_PORT  	(*(volatile unsigned short *)(ULTRA_IO_BASE + CONF_ADDR * 2))
#define  INDEX_PORT 	(*(volatile unsigned short *)(ULTRA_IO_BASE + INDEX_ADDR * 2))
#define  DATA_PORT  	(*(volatile unsigned short *)(ULTRA_IO_BASE + DATA_ADDR * 2))
/*****************************/

#define RBR   (*(volatile unsigned short *)(0xB04007F0))
#define THR   (*(volatile unsigned short *)(0xB04007F0))
#define DLL   (*(volatile unsigned short *)(0xB04007F0))
#define DLM   (*(volatile unsigned short *)(0xB04007F2))
#define IER   (*(volatile unsigned short *)(0xB04007F2))
#define IIR   (*(volatile unsigned short *)(0xB04007F4))
#define FCR   (*(volatile unsigned short *)(0xB04007F4))
#define LCR   (*(volatile unsigned short *)(0xB04007F6))
#define MCR   (*(volatile unsigned short *)(0xB04007F8))
#define LSR   (*(volatile unsigned short *)(0xB04007FA))
#define MSR   (*(volatile unsigned short *)(0xB04007FC))
#define SCR   (*(volatile unsigned short *)(0xB04007FE))

#else
#error Must set -DSH7729SE or -DSH7615SE
#endif
#endif

//LCR values
#define DATA_8_BITS		0x0300
#define STOP_BIT_1		0x0000
#define STOP_BIT_2		0x0400
#define NO_PARITY		0x0000
#define EVEN_PARITY		0x1800
#define ODD_PARITY		0x0800
#define	DLAB			0x8000

//MCR values
#define DTR			0x0100
#define RTS			0x0200
#define OUT2			0x0800 	// Must be set to enable interrupts

//LSR values
#define	DR			0x0100 // Data ready
#define	OE			0x0200 // Overrun error
#define	PE			0x0400 // Parity Error
#define THRE			0x2000 // THR Empty
#define TEMT			0x4000 // Trasmitter Empty

//FCR values
#define FIFO_DISABLED		0x0000

// Baud Rate values
#define	BAUD4800		0x1800
#define BAUD9600		0x0C00
#define BAUD19200		0x0600
#define BAUD38400		0x0300
#define BAUD115200		0x0100


#define TIMEOUT 100000

static unsigned short  read_w(unsigned short  index)
{
	INDEX_PORT = index;
	return	DATA_PORT;
}

static void  write_w(unsigned short  index, unsigned short  data)
{
	INDEX_PORT = index;
	DATA_PORT  = data;
}

void com1_conf(unsigned short add)
{
unsigned short low_add;
unsigned short high_add;

	low_add = ((add << 8) & 0xff00);
	high_add = (add & 0xff00);

	CONF_PORT  = 0x5500;  	/* Entering the configuration state */
	CONF_PORT  = 0x5500;

	write_w(0x2200, (read_w(0x2200) | 0x1000)); /* Power on uart ch0 (COM1) */
	write_w(0x0700, 0x0400);	/* Set uart ch0 logical device No. */
	write_w(0x3000, 0x0100);	/* Enable uart ch0 */
	write_w(0x6000, high_add);	/* Set uart ch0 base address */
	write_w(0x6100, low_add);
	write_w(0x7000, 0x0400);   	/* Select IRQ4 */
	write_w(0xF000, 0x0000);
	write_w(0x0700, 0x0000);

	CONF_PORT  = 0xAA00;  		/* Exiting the configuration state */

}

void initComm ( unsigned long baudRate )
{
	com1_conf(0x3f8) ;

	/* 8 bit data, 1 stop bit, Non parity and select DLL,DLM register */
	LCR = DLAB | DATA_8_BITS | STOP_BIT_1 | NO_PARITY  ;
	switch( baudRate )
	{
		case 9600:
			DLL = BAUD9600 ; 	/* baud rate 9600 */
			break ;

		case 19200:
			DLL = BAUD19200 ; 	/* baud rate 19200 */
			break ;

		case 38400:
			DLL = BAUD38400 ; 	/* baud rate 38400 */
			break ;

		case 115200:
			DLL = BAUD115200 ; 	/* baud rate 115200 */
			break ;

		default:
			DLL = BAUD9600 ; 	/* defualt to 9600 */
			break ;

	}
	DLM = 0; // Baud rate high byte 0
	LCR =  DATA_8_BITS | STOP_BIT_1 | NO_PARITY  ; // Remove DLAB
	MCR = OUT2; // required to enable serial interrupt				
	FCR = FIFO_DISABLED;
}
int sendComm( char * cBuf, int iSize )
{
	short iTemp ;
	while( iSize-- )
	{
		long lTimeOut = TIMEOUT ;
		// Wait till last character is transmitted
		while( ( LSR & (THRE | TEMT ) ) != (THRE | TEMT )  ) 
		{
			lTimeOut-- ;
			if( lTimeOut == 0 )
			{
				return SEND_FAILED ;
			}
		}
		iTemp =  *cBuf << 8 ;
		THR = iTemp ;
		cBuf++ ;

	}
	return SEND_SUCCESS ;
}

int readComm( char * cBuf, int iSize ) 
{
	int bytesReceived = 0 ;
	int iTemp ;
	while( iSize )
	{
		iTemp = rxChar( ) ;
		if( iTemp == -1 )
		{
			break ;
		}
		*cBuf++ = (char) ( iTemp >> 8 ) ;
		bytesReceived++ ;

	}
	return bytesReceived ;
}
int rxChar( void )
{

	long lTimeOut = TIMEOUT ;
		// Wait till last character is transmitted
	while( ( LSR & DR ) != DR ) 
	{
		lTimeOut-- ;
		if( lTimeOut == 0 )
		{
			return -1 ;
		}
	}
	return RBR ;
}
