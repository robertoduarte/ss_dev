/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 1-Feb-2004.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************/


#define SEND_SUCCESS		0
#define SEND_FAILED		-1

// Function prototypes
void initComm ( unsigned long baudRate ) ;
int sendComm( char * cBuf, int iSize ) ;
int readComm( char * cBuf, int iSize ) ;


