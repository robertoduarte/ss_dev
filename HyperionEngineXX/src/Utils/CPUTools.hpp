#pragma once

#include <yaul.h>

enum CPUType
{
    Master,
    Slave,
    Count
};

inline CPUType GetCPU()
{
    return cpu_dual_executor_get() == CPU_MASTER ? Master : Slave;
}

template <typename T>
volatile T &NoCache(T &value) { return *((T *)((char *)&value + 0x20000000)); }

class Mutex
{
private:
    bool locked[CPUType::Count];
    bool turn;

public:
    inline Mutex()
    {
        NoCache(locked[CPUType::Master]) = false;
        NoCache(locked[CPUType::Slave]) = false;
        NoCache(turn) = false;
    }

    inline void Lock()
    {
        CPUType cpu = GetCPU();
        NoCache(locked[cpu]) = true;
        NoCache(turn) = !cpu;
        while (NoCache(locked[!cpu]) == true &&
               NoCache(turn) == !cpu)
            /* Busy waiting */;
    }

    inline void Unlock() { NoCache(locked[GetCPU()]) = false; }
};
