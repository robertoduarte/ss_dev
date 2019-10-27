#ifndef _CONTROL_H_
#define _CONTROL_H_


#define INPUT_TYPE_ID 3
#include <sgl.h>
#include "../Utils/mem_mgr.h"
#include "../Object.h"


typedef struct
{
    /*------Object Header------*/
    ObjectHeader* header;
    /*-----Object Variables----*/
    FIXED test_val;
    int gamepadInput; // = Smpc_Peripheral[0].data;

} Input;

Input * new_Input();
void input_initialize(Input* input);
int input_update();

#endif
