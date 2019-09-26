/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 1-Feb-2004.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Author:
	Rohit Kumar Srivastava <rohits@kpitcummins.com>.

*****************************************************************/

#include "print.h"

void __main(){}

int main(void)
{
	/* Some local variables */

	char s1, cptr[80], c;
	char ss[20];
	unsigned int a=12345;
	signed int b=-15;
	int i;

	s1='Z';

#ifndef SIM
	initComm(115200) ; /* Initialize the COM1 port to 115200 */
	/* Other default settings are no parity 8data bits and 1stop bits */
#endif
	/* Test string input */
	printf1("\r\nEnter a string value: ");
#ifdef SIM
	printf1("\r\n"); /* flush out unwritten buffers on simulator */ 
#endif
	scanf2("%s", ss); /* Input string */ 

	printf1("\r\n123456789012345678901234567890\r\n");
	printf2("String is : %s\r\n", ss);
	printf4("%9s:%-9s:%.4s\r\n", ss, ss, ss); /* Try all formats */

	/* Test sprint for unsigned integers */
	printf1("\nTesting for integers...");
	sprintf4(cptr,"\r\na=%8d a=%08d a=%-8d\r\n", b,b,b);
	printf2("%s",cptr);

	/* Test print for unsigned integers */
	printf1("\r\n Testing for unsigned output...");
	printf4("\r\na=%8u a=%08u a=%-8u\r\n", b,b,b);

	/* Test print for hexadecimal integers */
	printf1("\r\n Testing for Hex output...");
	printf4("\r\nb=%8x b=%08x b=%-08x\r\n",a,a,a);

	/* Test print for character */
	printf1("\r\n Testing for Char output...");
	printf2("\r\ns1=%c\r\n",s1);

	/* Test scan for Hex input */
	printf1("\r\n Testing for Hex input...");
	printf1("\r\nEnter an hex value: ");
#ifdef SIM
	printf1("\r\n"); /* flush out unwritten buffers on simulator */ 
#endif
	scanf2("%x",&i);

	/* Test scan for character input */
	printf1("\r\nEnter a character value: ");
#ifdef SIM
	printf1("\r\n"); /* flush out unwritten buffers on simulator */ 
#endif
	scanf2("%c",&c);
	printf4("\r\nYou entered %x (HEX) which is %d (DEC) and %c (character) \r\n\n",i,i,c);
#ifdef SIM
	printf1("\r\n"); /* flush out unwritten buffers on simulator */ 
#endif
	printf1("*************** End of the sample *************\r\n");
	return 0;
}
