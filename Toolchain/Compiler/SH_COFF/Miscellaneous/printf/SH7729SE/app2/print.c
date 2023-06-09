/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 1-Feb-2004.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

ACKNOWLEDGEMENT:
The original source is http://www.linet.gr.jp/~mituiwa/h8/printf-20020909.c

Modified by:
	Rohit Kumar Srivastava <rohits@kpitcummins.com>.

List of functions.
	print()
	sprint()
	scan()
	sscan()

The supported formats are:
	decimal:        %d / %4d / %04d / %-04d
	decimal:        %u / %4u / %04u / %-04u
	hexidecimal:    %x / %4x / %04x / %-04x
	string:         %s / %.4s/ %-4s
	character:      %c
*****************************************************************/


#include <string.h>
#include <ctype.h>
#include <stdlib.h> 


extern int _read(int, char *, int);
extern int _write(int, char *, int);


char	hextbl[] = "0123456789ABCDEF"; /* String Table for Hex representation*/

/* Bit definitions for decision making */  
#define SCANDEF		0x100000
#define BARDEF		0x80000
#define DOTDEF		0x40000
#define ZERODEF	 	0x20000
#define SIZEDEF		0x10000
#define NEGATIVE	0x200000
#define TRUE		1
#define FALSE		0
#define ESCAPE 		1
#define STDOUT		1
#define STDIN		1
#define MAXINTLEN	15
#define MAXSTRLEN	256

/* Write one character to the output stream */
static int outchar(char c, char *ptr, int off) {
	if(ptr == (char*)-1) _write(STDOUT, &c, 1);
	else if(ptr > (char*)255) ptr[off] = c;
	return off + 1;
}

/* Write a string to the output stream */
static int outstring(char *data, int len, char *ptr, int offset, unsigned long _num) {
	int	i, num1, siz, off;

	off = offset;
	num1 = _num & 0xffff;
	siz = len;
	
	/* Check for right justify and also for size */ 
	if((_num & (SIZEDEF | BARDEF | DOTDEF)) == SIZEDEF) 
		for(i = num1;i > len;i--)
			off = outchar(' ', ptr, off); /* Insert left padding */

	/* Check only for size */ 
	if((_num & SIZEDEF) && (_num & DOTDEF) && (siz > num1)) siz = num1;
	if(ptr == (char*)-1) _write(STDOUT, data, siz);
	else if(ptr > (char*)255) memcpy(&(ptr[off]), data, siz);
	off += siz;

	/* Check only for right justify */ 
	if((_num & (SIZEDEF | BARDEF | DOTDEF)) == (SIZEDEF | BARDEF))
		for(i = num1;i > len;i--)
			off = outchar(' ', ptr, off); /* Insert left padding */
	return off;
}

/* Pad left with Zero or space */
static int out_space(unsigned long _num, char* ptr, int off) {
	if(_num & ZERODEF)
		return outchar('0', ptr, off);
	else
		return outchar(' ', ptr, off);
}

/* Read from input stream */
static int input_number(int *value, char *ptr, int offset, int _num, int sel) {
	char	buffer[16], *inptr = '\0';
	int	off;

	off = offset;
	if(ptr == (char*)-1) {
		_read(STDIN, buffer, MAXINTLEN);
		_write(STDOUT, "", 1);
		inptr = buffer;
	} else if(ptr > (char*)255) {
		while(ptr[off] <= ' ') off++;
		inptr = &(ptr[off]);
		while(ptr[off] > ' ') off++;
	}
	if(sel == 0) 
		*value = atoi(inptr);
	else 
		*value = strtol(inptr,(char**)("\n"), 16); /* Convert input
					string to Hex integer */
	return off;
}

/* Converts integer to string for writing to output stream. Also reads from the 
input stream in case of scanf */
static int number(unsigned int value, char *ptr, int offset, unsigned long _num) {
	unsigned long	a, b, dec;
	int		f, off, k;
	unsigned long num1;
	long long i;
	unsigned char cnegative=FALSE; /* Flag to print negative sign */
	
	if(_num & SCANDEF)
		return input_number((int*)value, ptr, offset, _num, 0);
	dec = value;
	off = offset;
	if(_num & NEGATIVE){
		_num &= ~NEGATIVE;
		cnegative=TRUE;
	}
	num1 = _num & 0xffff;
	for(k = 10, i = 1000000000;i >= 10;i /= 10, k--) if(value / i) break;
	if((_num & (SIZEDEF | BARDEF)) == SIZEDEF)
		for(i = num1;i > k;i--)
			off = out_space(_num, ptr, off);
	if(cnegative){
		off = outchar('-', ptr, off);
		cnegative = FALSE;
	}
	f = 0;
	for(i = 1000000000;i >= 100;i /= 10) {
		a = dec % i;
		b = i / 10;
		a = a / b;
		if(a > 0) f = 1;
		if(f == 1) off = outchar(hextbl[a], ptr, off);
	}
	a = dec % 10;
	off = outchar(hextbl[a], ptr, off);
	if((_num & (SIZEDEF | BARDEF)) == (SIZEDEF | BARDEF))
		for(i = num1;i > k;i--)
			off = out_space(_num, ptr, off);
	return off;
}

/* Check for negative value */
static int decimal(int value, char *ptr, int offset, unsigned long _num) {
	int	num, off;

	if(_num & SCANDEF)
		return input_number((int*)value, ptr, offset, _num, 0);
	num = value;
	off = offset;
	if(num < 0) {
		/* Check for leading zeros */
		if(((_num & SIZEDEF) == SIZEDEF)&&((_num & ZERODEF) != ZERODEF))
			_num |= NEGATIVE; /*If no leading zeroes then set print
					negative sign later */
		else
			off = outchar('-', ptr, off);
		num = ~num + 1;
	}
	return number(num, ptr, off, _num);
}

/* Converts integer to string for writing to output stream. Also reads from the 
input stream in case of scanf */
static int hex(unsigned int value, char *ptr, int offset, unsigned long _num) {
	int	off, sft, index, nm, num1, f, i;
	unsigned long bits;

	if(_num & SCANDEF)
		return input_number((int*)value, ptr, offset, _num, 1);
	off = offset;
	num1 = _num & 0xffff;
	for(nm = 36,bits = 0xffffffff;bits > 0;bits >>= 1, nm--)
		if(~bits & value) break;
	nm >>= 2;
	if((_num & (SIZEDEF | BARDEF)) == SIZEDEF)
		for(i = num1;i > nm;i--)
			off = out_space(_num, ptr, off);
	f = 0;
	for(sft = 28;sft >= 0;sft -= 4) {
		index = (value >> sft) & 0xf;
		if(index > 0) f = 1;
		if(index > 0 || f == 1 || sft == 0)
			off = outchar(hextbl[index], ptr, off);
	}
	if((_num & (SIZEDEF | BARDEF)) == (SIZEDEF | BARDEF))
		for(i = num1;i > nm;i--)
			off = out_space(_num, ptr, off);
	return off;
}

/* Process the format string */
int vprintf_l(char *p, char *form, int a, int b, int *arg, unsigned long _opt) {
	int value=0;
	long	i, n, off, argc, num=0, numindex=0, inoff;
	char	flag, numbuf[5], *ptr, *inptr;
	unsigned long opt=0;
	unsigned char _zeroflag;  /* This flag will insert space instead of zero
			 in case of left justification */

	_zeroflag =0;
	flag = 0;
	off = 0;
	argc = -2;
	n = strlen(form);
	for(i = 0;i < n;i++) {
		if(flag & ESCAPE) {
			if(argc == -2) value = a;
			else if(argc == -1) value = b;
			else value = arg[argc];
		}
		if(form[i] == '%') {
			if(i > 0 && form[i - 1] == '%') {
				flag &= ~ESCAPE;
				if(!(_opt & SCANDEF)) off = outchar(form[i], p, off);
			} else {
				flag |= ESCAPE;
				numindex = -1;
				num = 0;
				opt = _opt;
			}
		} else if((flag & ESCAPE) && (form[i] == 'd' || form[i] == 'i')) {
			argc++;
			if(value < 0) num--; /* Adjust padding for negative symbol */
			off = decimal(value, p, off, num | opt);
			flag &= ~ESCAPE;
		} else if((flag & ESCAPE) && form[i] == 'u') {
			argc++;
			off = number(value, p, off, num | opt);
			flag &= ~ESCAPE;
		} else if((flag & ESCAPE) && (form[i] == 'x' || form[i] == 'X')) {
			argc++;
			off = hex(value, p, off, num | opt);
			flag &= ~ESCAPE;
		} else if((flag & ESCAPE) && form[i] == 's') {
			argc++;
			ptr = (char*)value;
			if(opt & SCANDEF) {
				if(p == (char*)-1) {
					if(!(opt & SIZEDEF)) num = MAXSTRLEN;
					_read(STDIN, ptr, num);
					_write(STDOUT, "", 1);
				} else if(p > (char*)255) {
					while(p[off] <= ' ') off++;
					inptr = &(p[off]);
					inoff = off;
					while(p[off] > ' ') off++;
					memcpy(ptr, inptr, off - inoff);
					ptr[off - inoff] = 0;
				}
			} else {
				off = outstring(ptr, strlen(ptr), p, off, num | opt);
			}
			flag &= ~ESCAPE;
		} else if((flag & ESCAPE) && form[i] == 'c') {
			argc++;
			if(opt & SCANDEF) {
				ptr = (char*)value;
				if(p == (char*)-1) {
					*ptr = 0;
					_read(STDIN, ptr, 1);
					_write(STDOUT, "", 1);
				} else {
					while(p[off] <= ' ') off++;
					*ptr = p[off++];
				}
			} else {
				off = outchar((char)value, p, off);
			}
			flag &= ~ESCAPE;
		} else if((flag & ESCAPE) && isdigit(form[i])) {
			if(numindex > 3) flag &= ~ESCAPE;
			else {
				numbuf[++numindex] = form[i];
				numbuf[numindex + 1] = 0;
				num = atoi(numbuf);
				opt |= SIZEDEF;
				if((form[i] == '0')&&(!_zeroflag)) opt |= ZERODEF;
			}
		} else if((flag & ESCAPE) &&
			  (form[i] == '.' || form[i] == ' ' ||
			   form[i] == 'l' || form[i] == '-')) {
			if(form[i] == '.') opt |= DOTDEF;
			if(form[i] == '-'){ opt |= BARDEF; _zeroflag = 1;}
		} else {
			flag &= ~ESCAPE;
			if(!(_opt & SCANDEF)) off = outchar(form[i], p, off);
		}
	}
	return off;
}

void print(char *form, int a, int b, int c) {
	vprintf_l((char*)-1, form, a, b, &c, 0);
}

void sprint(char *buf, char *form, int a, int b, int c) {
	int	n;

	n = vprintf_l(buf, form, a, b, &c, 0);
	buf[n] = 0;
}

void scan(char *form, int a, int b, int c) {
	vprintf_l((char*)-1, form, a, b, &c, SCANDEF);
}

void sscan(char *buf, char *form, int a, int b, int c) {
	vprintf_l(buf, form, a, b, &c, SCANDEF);
}

