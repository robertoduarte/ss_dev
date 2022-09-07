#pragma once

#include "..\EventSystem\ActionEvent.h"

typedef enum
{
    Binding_Default,
    Binding_Custom1,
    Binding_Custom2,
    Binding_Count
} Binding;

typedef enum
{
    Player_1,
    Player_2,
    Player_Count
} Player;

void InputSystem_SetActionMap(Binding binding, Action action, short map);

void InputSystem_Update();

