#ifndef MOWTimer_H
#define MOWTimer_H

#include <windows.h>

class CMOWTimer
{
public:
    CMOWTimer(
        );

    void                        
        Start(
            );

    void                        
        Stop(
            );

    double                      
        ElapsedTimeMicroSeconds(
            )const;

    double                      
        ElapsedTimeMilliSeconds(
            )const;

    double                      
        ElapsedTimeSeconds(
            )const;

private:

    LARGE_INTEGER               
        m_frequency;

    LARGE_INTEGER               
        m_startTick;

    double                      
        m_time;

    bool                        
        m_hasBeenStarted;

};

#endif