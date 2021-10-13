#include "InputSystem.h"
#include "..\EventSystem\EventManager.h"

typedef short ButtonMap[Action_Count];

static ButtonMap g_bindings[Binding_Count];

static Binding g_currentBinding[Player_Count];

static bool g_init = false;

void InputSystem_SetActionMap(Binding binding, Action action, short map)
{
    g_bindings[binding][action] = map;
}

static void Init()
{
    if (!g_init)
    {
        // InputSystem_SetActionMap(Binding_Default, Action_TurnUp, PER_DGT_KU);
        // InputSystem_SetActionMap(Binding_Default, Action_TurnDown, PER_DGT_KD);
        // InputSystem_SetActionMap(Binding_Default, Action_TurnLeft, PER_DGT_KL);
        // InputSystem_SetActionMap(Binding_Default, Action_TurnRight, PER_DGT_KR);

        g_currentBinding[Player_1] = Binding_Default;
        g_currentBinding[Player_2] = Binding_Default;

        g_init = true;
    }
}

void UpdatePlayer(Player player)
{
    for (Action i = 0; i < Action_Count; i++)
    {
        short data = 1; // ~Smpc_Peripheral[player].data;
        if (data & g_bindings[g_currentBinding[player]][i])
            EventManager_TriggerEvent(Action_Event, i);
    }
}

void InputSystem_Update()
{
    Init();
    for (Player i = 0; i < Player_Count; i++)
        UpdatePlayer(i);
}
