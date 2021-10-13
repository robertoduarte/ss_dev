#include "Debug.h"
#include "Vector.h"
#include "SatMalloc.h"
#include <SGL.H>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

VECT_GENERATE_NAME(char *, LogMsg);

static LogMsgVector *g_logMsgVector = NULL;

int vsnprintf(char *restrict buffer, size_t bufsz, const char *restrict format, va_list vlist);

static void Print(char *string, int pos)
{
    slPrint("                                       ", slLocate(0, pos)); //empty line
    slPrint(string, slLocate(0, pos));
}

static void PrintAllLines()
{
    for (int i = 0; i < 30 && i < g_logMsgVector->size; i++)
    {
        Print(*LogMsgVector_Pointer(g_logMsgVector, i), i);
    }
}

static void PushMessage(char *msg)
{
    if ((g_logMsgVector->size + 1) >= 30)
    {
        free(*LogMsgVector_Pointer(g_logMsgVector, 0));
        LogMsgVector_Remove(g_logMsgVector, 0);
    }

    LogMsgVector_Push(g_logMsgVector, msg);
}

void Debug_WaitForInput()
{
    do
    {
        slSynch();
    } while (Smpc_Peripheral[0].push == 0xFFFF);
}

void Debug_PrintMsg(const char *msg, ...)
{
    char buffer[40];
    va_list list;
    va_start(list, msg);
    vsnprintf(buffer, 40, msg, list);
    va_end(list);
    Print("DEBUG MESSAGE:", 0);
    Print(buffer, 1);

    Debug_WaitForInput();
}

void Debug_PrintLine(const char *msg, ...)
{
    if (!g_logMsgVector)
        g_logMsgVector = LogMsgVector_Init(1);

    va_list list;
    char *buffer = calloc(128);
    va_start(list, msg);
    int size = vsnprintf(buffer, 128, msg, list);
    va_end(list);
    buffer = realloc(buffer, size + 1);

    // int totalLines = (size / 40) + ((size % 40) ? 1 : 0);
    // for (int i = 0; i < totalLines; i++)
    // {
    //     int lineSize = size - ((totalLines - i) * 40);
    //     Debug_PrintMsg("size: %d linesize: %d total lines %d", size,lineSize, totalLines);
    // }

    PushMessage(buffer);

    PrintAllLines();

    // Debug_WaitForInput();
}

void exit(int);

void Debug_ExitMsg(const char *msg, ...)
{
    char buffer[40];
    va_list list;
    va_start(list, msg);
    vsnprintf(buffer, 40, msg, list);
    va_end(list);
    Print("EXIT MESSAGE:", 0);
    Print(buffer, 1);

    Debug_WaitForInput();
    exit(1);
}
