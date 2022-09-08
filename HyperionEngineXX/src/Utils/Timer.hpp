#pragma once

#include <yaul.h>
#include <assert.h>
#include "CPUTools.hpp"

class SystemTime
{
private:
    static constexpr uint16_t timerInterval = CPU_FRT_NTSC_320_8_COUNT_1MS;
    static volatile inline uint32_t ms;
    static volatile inline uint32_t accumulator;

    static inline void oviHandler() { assert(false); }

    static inline void TimerAutomaticUpdate()
    {
        const volatile uint16_t frtCount = cpu_frt_count_get();
        NoCache(ms)++;

        if (frtCount > timerInterval)
        {
            accumulator += frtCount - timerInterval;
            while (accumulator >= timerInterval)
            {
                NoCache(ms)++;
                accumulator -= timerInterval;
            }
            assert(accumulator < timerInterval);
        }
        cpu_frt_count_set(0);
    }

public:
    static inline void Initialize()
    {
        NoCache(ms) = 0;
        accumulator = 0;

        cpu_frt_init(CPU_FRT_CLOCK_DIV_8);
        cpu_frt_ovi_set(oviHandler);
        cpu_frt_oca_set(timerInterval, TimerAutomaticUpdate);
        cpu_frt_count_set(0);
        cpu_frt_interrupt_priority_set(8);
    }

    static inline volatile uint32_t &CurrentTime()
    {
        return NoCache(ms);
    }
};

class Timer
{
private:
    uint32_t startTime;
    bool isRunning;

public:
    inline Timer() : isRunning(true) { Reset(); }

    inline void Reset()
    {
        startTime = SystemTime::CurrentTime();
        isRunning = true;
    }

    inline void End()
    {
        uint32_t timeDiff = SystemTime::CurrentTime() - startTime;
        startTime = timeDiff;
        isRunning = false;
    }

    inline uint32_t Count()
    {
        return isRunning ? (SystemTime::CurrentTime() - startTime) : startTime;
    }
};
