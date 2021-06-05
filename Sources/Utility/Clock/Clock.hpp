#pragma once

#include <chrono>

using HRClock = std::chrono::high_resolution_clock;
using ClockFreq = std::chrono::nanoseconds;

class Clock
{
public:
    Clock();
    ~Clock() = default;

    void Start();
    void Stop();
    int64_t Elapsed();
    int64_t Current();
    [[nodiscard]] int64_t Difference() const;

private:
    HRClock::time_point m_start;
    HRClock::time_point m_stop;
    HRClock::time_point m_current;
};
