#include "control.h"


static ObjectHeader input_header = {
    INPUT_TYPE_ID,
    input_update
};

Input * new_Input()
{
    Input* input = Memmalloc(sizeof (Input));
    if(input) input_initialize(input);
    return input;
}

void input_initialize(Input* input)
{
    input->header = &input_header;
    // input->gamepadInput = 0;
}

int input_update()
{
    int gamepadInput = Smpc_Peripheral[0].data;
    //Start
    if(!(gamepadInput & PER_DGT_ST)){
        return 1;
        // slPrint("Start Pressed!!!!",slLocate(0,10));
    }
    // else
    //     slPrint("                  ",slLocate(0,10));

    //A
    // if(!(gamepadInput & PER_DGT_TA))
    //     slPrint("A Pressed!!!!",slLocate(0,9));
    // else
    //     slPrint("                  ",slLocate(0,9));
    // //B
    // if(!(gamepadInput & PER_DGT_TB))
    //     slPrint("B Pressed!!!!",slLocate(0,8));
    // else
    //     slPrint("                  ",slLocate(0,8));
    
    // //C
    // if(!(gamepadInput & PER_DGT_TC))
    //     slPrint("C Pressed!!!!",slLocate(0,7));
    // else
    //     slPrint("                  ",slLocate(0,7));
    
    // //X
    // if(!(gamepadInput & PER_DGT_TX))
    //     slPrint("X Pressed!!!!",slLocate(0,6));
    // else
    //     slPrint("                  ",slLocate(0,6));

    // //Y
    // if(!(gamepadInput & PER_DGT_TY))
    //     slPrint("Y Pressed!!!!",slLocate(0,5));
    // else
    //     slPrint("                  ",slLocate(0,5));
 
    // //Z
    // if(!(gamepadInput & PER_DGT_TZ))
    //     slPrint("Z Pressed!!!!",slLocate(0,11));
    // else
    //     slPrint("                  ",slLocate(0,11));

        
    // //L
    // if(!(gamepadInput & PER_DGT_TL))
    //     slPrint("L Pressed!!!!",slLocate(0,12));
    // else
    //     slPrint("                  ",slLocate(0,12));

        
    // //R
    // if(!(gamepadInput & PER_DGT_TR))
    //     slPrint("A Pressed!!!!",slLocate(0,13));
    // else
    //     slPrint("                  ",slLocate(0,13));

        
    // if(!(gamepadInput & PER_DGT_KD))
    //     slPrint("DOWN",slLocate(0,14));
    // else
    //     slPrint("     ",slLocate(0,14));
    

    // if(!(gamepadInput & PER_DGT_KU))
    //     slPrint("UP",slLocate(0,15));
    // else
    //     slPrint("  ",slLocate(0,15));
    
    // if(!(gamepadInput & PER_DGT_KL))
    //     slPrint("LEFT",slLocate(0,16));
    // else
    //     slPrint("    ",slLocate(0,16));
    
    // if(!(gamepadInput & PER_DGT_KR))
    //     slPrint("RIGHT",slLocate(0,17));
    // else
    //     slPrint("     ",slLocate(0,17));
    
    return 0;
};