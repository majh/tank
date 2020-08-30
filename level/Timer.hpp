#pragma once
#include 
class Timer
{
public:
    Timer() :
        m_start(m_clock::now())
    {
    }

    void reset()
    {
        m_start = m_clock::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<microseconds> (m_clock::now() - m_start).count();
    }

private:
    typedef std::chrono::steady_clock m_clock;
    typedef std::chrono::duration<int, std::micro> microseconds;
    std::chrono::time_point<m_clock> m_start;
};
