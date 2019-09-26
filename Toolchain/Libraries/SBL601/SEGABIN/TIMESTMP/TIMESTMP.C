/*======================================================================
 *      timestmp.c
 *      Copyright(c) SEGA ENTERPRISES, LTD. 1995
 *      Written by eMuKei on 1995-05-22 Ver.1.00
 *----------------------------------------------------------------------
 *Note: \hitech\C -o -v -f -i.\ -mtimestmp.map timestmp.c
 *Bug:  Change space charactors to one space charactor.
 *======================================================================*/
#include <stdio.h>
#include <time.h>

#define DBL 1
#define YEN 2
static char msg[]="(c)eMuKei 1995 V.1.00\nUsage: timestmp [\"<comment>[\\]\"]\n";

/*======================================
 *      TMS_puts()
 *======================================*/
int TMS_puts(char *p)
{
    register int r = 0;

    while (*p != '\0'){
        if (*p == '"'){
            p++;
            if (*p == '\0'){
                r = DBL;
                break;
            }
        }
        if (*p == '\\'){
            if (*(p+1) == '"'){
                r = YEN;
                break;
            }
        }
        putc(*p++,stdout);
    };
    return r;
}

/*======================================
 *      TMS_puttime()
 *======================================*/
void TMS_puttime()
{
    time_t sec;

    time(&sec);
    TMS_puts(ctime(&sec));
}

/*======================================
 *      main()
 *======================================*/
main(int argc, char *argv[])
{
    register int n;
    register char *p;

    if(--argc){
        p = argv[1];
        if (*p != '"'){
            TMS_puts(msg);
        }
        else {
            n = 0;
            while (argc--){
                switch (TMS_puts(argv[++n])){
                case YEN:
                    break;
                case DBL:
                        putc('\n',stdout);
                    break;
                default:
                    if (argc)
                        putc(' ',stdout);
                }
            }
            TMS_puttime();
        }
    }
    else{
        TMS_puttime();
    }
}

/*===== End of file ====================================================*/
