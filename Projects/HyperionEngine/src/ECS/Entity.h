#ifndef ENTITY_H
#define ENTITY_H

typedef struct{
    short oldEntityId;
    short newEntityId;
} EntityUpdate;

inline short Entity_AssignId();

inline void Entity_FreeId(short entityId);

#endif // !ENTITY_H
