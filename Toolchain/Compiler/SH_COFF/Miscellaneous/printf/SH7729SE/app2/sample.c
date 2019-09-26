/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 1-Feb-2004.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Author:
	Rohit Kumar Srivastava <rohits@kpitcummins.com>.

*****************************************************************/

#include "printf.h"
extern void initComm( int );

void __main(){}

int main(void)
{
	char *ptr = "Hello world!";
	char *np = 0;
	int i = 5;
	unsigned int bs = sizeof(int)*8;
	int mi;
	char buf[80];
#ifndef SIM
        initComm(115200);              /* Set up the UART */
#endif
	mi = (1 << (bs-1)) + 1;
	l_printf("%4s\r\n", ptr);
	l_printf("printf test\r\n");
	l_printf("%s is null pointer\r\n", np);
	l_printf("%d = 5\r\n", i);
	l_printf("%d = - max int\r\n", mi);
	l_printf("char %c = 'a'\r\n", 'a');
	l_printf("hex %x = ff\r\n", 0xff);
	l_printf("hex %02x = 00\r\n", 0);
	l_printf("signed %d = unsigned %u = hex %x\r\n", -3, -3, -3);
	l_printf("%d %s(s)%", 0, "message");
	l_printf("\r\n");
	l_printf("%d %s(s) with %%\r\n", 0, "message");
	l_sprintf(buf, "justif: \"%-10s\"\r\n", "left"); l_printf("%s", buf);
	l_sprintf(buf, "justif: \"%10s\"\r\n", "right"); l_printf("%s", buf);
	l_sprintf(buf, " 3: %04d zero padded\r\n", 3); l_printf("%s", buf);
	l_sprintf(buf, " 3: %-4d left justif.\r\n", 3); l_printf("%s", buf);
	l_sprintf(buf, " 3: %4d right justif.\r\n", 3); l_printf("%s", buf);
	l_sprintf(buf, "-3: %04d zero padded\r\n", -3); l_printf("%s", buf);
	l_sprintf(buf, "-3: %-4d left justif.\r\n", -3); l_printf("%s", buf);
	l_sprintf(buf, "-3: %4d right justif.\r\n", -3); l_printf("%s", buf);

	return 0;
}

/*
 * if you compile this file with
 *   gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 16bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -32767 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
*/
