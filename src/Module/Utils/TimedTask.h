#pragma once

#include <chrono>
#include <functional>

using namespace std;
using namespace chrono;

class TimedTask
{
public:
    using Fn = function<void(void)>;
    using TimeDuration = seconds;
    using TimePoint = time_point<steady_clock, TimeDuration>;
    struct Option
    {
        Fn fn;
        uint32_t taskId;
        TimePoint timePoint;
        TimeDuration timeDuration;
    };
    TimedTask(uint32_t taskId, Fn fn, const TimePoint &timePoint);
    TimedTask(uint32_t taskId, Fn fn, const TimeDuration &timeDuration);
    TimedTask(uint32_t taskId, Fn fn, const TimePoint &timePoint, const TimeDuration &timeDuration);
    void run();
    void setTimePoint(const TimePoint &timePoint);
    void setTimeDuration(const TimeDuration &timeDuration);
    uint32_t getTaskId();
private:
    bool isPermit();
    void reflesh();
    Option option;
};