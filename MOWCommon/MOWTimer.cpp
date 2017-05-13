
#include "MOWTimer.h"

CMOWTimer::CMOWTimer()
{
    m_time = 0.0;
    m_hasBeenStarted = false;
}
//---------------------------------------------
void CMOWTimer::Start()
{
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_startTick);
    m_hasBeenStarted = true;
}
//---------------------------------------------
void CMOWTimer::Stop()
{
    if( m_hasBeenStarted )
    {
        LARGE_INTEGER endTime;

        QueryPerformanceCounter(&endTime);
        m_time = static_cast<double>(((endTime.QuadPart - m_startTick.QuadPart) * 1000000) / m_frequency.QuadPart);
    }
    else
    {
        m_time = 0.0;
    }
    m_hasBeenStarted = false;
    
}
//---------------------------------------------
double CMOWTimer::ElapsedTimeMicroSeconds() const
{
    return m_time;
}
//---------------------------------------
double CMOWTimer::ElapsedTimeMilliSeconds() const
{
    return m_time/1000.0;
}
//---------------------------------------
double CMOWTimer::ElapsedTimeSeconds() const
{
    return m_time/1000000.0;
}
