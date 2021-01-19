#include "ComponentMap.h"

static int ComponentPosition(ArchetypeId archetypeId, Component component)
{
    if (!(archetypeId & ComponentGetId(component)))
        return -1;

    int bytePosition = 0;
    for (Component i = 0; i < component; i++)
        if (archetypeId & ComponentGetId(i))
            bytePosition += ComponentGetSize(i);
    return bytePosition;
}

ComponentMap BuildComponentMap(ArchetypeId archetypeId)
{
    ComponentMap map;
    for (int i = 0; i < COUNT(COMPONENTS); i++)
    {
        map.componentPosition[i] = ComponentPosition(archetypeId, i);
    }
    return map;
}
