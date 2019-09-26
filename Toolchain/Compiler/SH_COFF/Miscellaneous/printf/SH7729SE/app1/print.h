/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 1-Feb-2004.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Author:
	Rohit Kumar Srivastava <rohits@kpitcummins.com>.

*****************************************************************/

/* print.h */


void print(char *, int, int, int);
void sprint(char *, char *, int, int, int);
void scan(char *, int, int, int);
void sscan(char *, char *, int, int, int);

#ifndef __PRINT_LITE__
#define __PRINT_LITE__ 
#define printf1(a) print(a,'\0','\0','\0')
#define printf2(a,b) print(a,(int)b,'\0','\0')
#define printf3(a,b,c) print(a,(int)b,c,'\0')
#define printf4(a,b,c,d) print(a,(int)b, (int)c, (int)d)
#define printf printf2

#define sprintf1(a,b) sprint(a,b,'\0','\0','\0')
#define sprintf2(a,b,c) sprint(a,b,(int)c,'\0','\0')
#define sprintf3(a,b,c,d) sprint(a,b,(int)c,(int)d,'\0')
#define sprintf4(a,b,c,d,e) sprint(a,b,(int)c, (int)d, (int)e)
#define sprintf sprintf2


#define scanf1(a) scan(a,'\0','\0','\0')
#define scanf2(a,b) scan(a,(int)b,'\0','\0')
#define scanf3(a,b,c) scan(a,(int)b,c,'\0')
#define scanf4(a,b,c,d) scan(a,(int)b, (int)c, (int)d)
#define scanf scanf2


#endif
