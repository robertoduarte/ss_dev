#pragma once

typedef struct IdGenerator IdGenerator;

IdGenerator *IdGenerator_Init(int capacity);

void IdGenerator_FreeId(IdGenerator *idGenerator, int id);

int IdGenerator_GenerateId(IdGenerator *idGenerator);

