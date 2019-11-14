#ifndef HEALTH_MANAGER_H
#define HEALTH_MANAGER_H
#include "SL_DEF.H"

typedef struct
{
    Uint16 currentHealth;
    Uint16 maxHealth;
} HealthComponent;

void HealthManager_Initialize(Sint32 maxComponents);

void HealthManager_AddComponent(Uint16 entityID, HealthComponent healthComponent);

void HealthManager_UpdateComponent(Uint16 entityID);

#endif // !HEALTH_MANAGER_H
