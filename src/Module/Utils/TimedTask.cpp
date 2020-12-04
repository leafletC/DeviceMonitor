#include "TimedTask.h"

TimedTask::TimedTask(uint32_t taskId, Fn fn, const TimePoint &timePoint)
{
    TimedTask(taskId, fn, timePoint, seconds(0));
}

TimedTask::TimedTask(uint32_t taskId, Fn fn, const TimeDuration &timeDuration)
{
    TimedTask(taskId, fn, time_point_cast<minutes>(steady_clock::now()), timeDuration);
}

TimedTask::TimedTask(uint32_t taskId, Fn fn, const TimePoint &timePoint, const TimeDuration &timeDuration)
{
    option.taskId = taskId;
    option.fn = fn;
    option.timePoint = timePoint;
    option.timeDuration = timeDuration;
    reflesh();
}

bool TimedTask::isPermit()
{
    TimePoint now = time_point_cast<TimeDuration>(steady_clock::now());
    if(now > option.timePoint)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TimedTask::run()
{
    if(isPermit())
    {
        option.fn();
        reflesh();
    }
}

void TimedTask::reflesh()
{
    option.timePoint += option.timeDuration;
}

uint32_t TimedTask::getTaskId()
{
    return option.taskId;
}

void TimedTask::setTimePoint(const TimePoint &timePoint)
{
    this->option.timePoint = timePoint;
}

void TimedTask::setTimeDuration(const TimeDuration &timeDuration)
{
    this->option.timeDuration = timeDuration;
}