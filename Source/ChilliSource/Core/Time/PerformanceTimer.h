//
//  PerformanceTimer.h
//  MoshiMonsters
//
//  Created by Scott Downie on 04/09/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_CORE_PERFORMANCE_TIMER_H_
#define _MOFLOW_CORE_PERFORMANCE_TIMER_H_

#ifdef TARGET_WINDOWS   
    #include <windows.h>
#else          
    #include <sys/time.h>
#endif

namespace moFlo
{
    class CPerformanceTimer
    {
    public:
        
#ifdef TARGET_WINDOWS
        typedef LARGE_INTEGER TimeValue;
#else
        typedef timeval TimeValue;
#endif
        
        void Start();
        void Stop();
        
        f64 GetTimeTakenS() const;
        f64 GetTimeTakenMS() const;
        f64 GetTimeTakenMicroS() const;
        
    private:
        
        static f64 GetTimeDurationMicroS(const TimeValue& inStart, const TimeValue& inEnd);
        
        TimeValue mStartTime, mFrequency;
        f64 mffLastDurationMicroS;
    };
}

#endif
