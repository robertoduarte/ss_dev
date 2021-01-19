#include "Component.h"

static const size_t g_componentSizes[] = {COMPONENTS(AS_SIZE)};

size_t ComponentGetSize(Component component)
{
    return g_componentSizes[component];
}

ComponentId ComponentGetId(Component component)
{
    return 1 << component;
}
