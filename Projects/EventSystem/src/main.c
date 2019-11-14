#include <sgl.h>
#include "Utils/mem_mgr.h"
#include "Utils/rbtree.h"

#include "EventSystem/EventManager.h"
#include "ECS/HealthComponentManager.h"
void init()
{
    slInitSystem(TV_320x240, NULL, 1);
    slPerspective(DEGtoANG(60.0));
    // handle set size = 16, trash RAM = 2K
    MemInit((pointer)0x06040000, (pointer)0x060BFFFF, 16, 16, 0x800);
}

void exampleEventListener(Event *event)
{
    switch (event->type)
    {
    case TestEvent:
        slPrintFX(toFIXED((int)event->arg), slLocate(0, (int)event->arg));
    }
}

void printInorder(RedBlackNode *node)
{
    if (node == NULL)
        return;
    printInorder(node->left);
    static int i = 0;
    slPrintFX(node->data, slLocate(0, i));
    i++;
    printInorder(node->right);
}

int main(void)
{
    init();

    //EventManager_Init();
    //EventManager_AddListener(TestEvent, &exampleEventListener);

    // RedBlackNode *TestTree = NULL;

    // Insert(&TestTree, 1, toFIXED(10));
    // Insert(&TestTree, 6, toFIXED(60));
    // Insert(&TestTree, 5, toFIXED(50));
    // Insert(&TestTree, 3, toFIXED(30));
    // Insert(&TestTree, 2, toFIXED(20));
    // Insert(&TestTree, 4, toFIXED(40));

    // Delete(&TestTree, 2);

    //printInorder(TestTree);

    HealthComponent hC = {
        10,
        20};

    HealthManager_Initialize(10);
    HealthManager_AddComponent(1, hC);
    HealthManager_TestComponent(1);

    return 1;
}
