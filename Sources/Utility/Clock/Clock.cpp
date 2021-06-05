#include "Clock.hpp"

Clock::Clock()
{
    Start();
    Stop();
}

void Clock::Start()
{
    m_start = HRClock::now();
}

void Clock::Stop()
{
    m_stop = HRClock::now();
}

int64_t Clock::Elapsed()
{
    m_current = HRClock::now();
    return std::chrono::duration_cast<ClockFreq>(m_current - m_start).count();
}

int64_t Clock::Difference() const
{
    return std::chrono::duration_cast<ClockFreq>(m_stop - m_start).count();
}

int64_t Clock::Current()
{
    m_current = HRClock::now();
    return std::chrono::duration_cast<ClockFreq>(m_current.time_since_epoch()).count();
}