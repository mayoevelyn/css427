#include "Schedule.h"

// Constructor
Schedule::Schedule()
{
    for (int i = 1; i <= TOTAL_ZONES; i++)
    {
        collection[i].zone = i;
        collection[i].hour = -1;
        collection[i].minute = -1;
        collection[i].duration = -1;
        collection[i].enabled = false;
    }
}

// Destructor
Schedule::~Schedule()
{
    // nothing to destruct
}

// Set Schedule
void Schedule::setSchedule(byte zone, byte hour, byte minute, byte duration)
{
    if (zone < 1 || zone > TOTAL_ZONES)
    {
        return;
    }

    zone--; // correct for 1 based index
    collection[zone].hour = hour;
    collection[zone].minute = minute;
    collection[zone].duration = duration;
    collection[zone].enabled = true;
}

// Get Duration
byte Schedule::getDuration(byte zone)
{
    if (zone < 1 || zone > TOTAL_ZONES)
    {
        return 0;
    }
    
    zone--; // correct for 1 based index
    return collection[zone].duration;
}

// Check Schedule
bool Schedule::checkSchedule(byte zone, byte hour, byte minute)
{
    if (zone < 1 || zone > TOTAL_ZONES)
    {
        return false;
    }

    zone--; // correct for 1 based index
    if (collection[zone].enabled && collection[zone].hour == hour && collection[zone].minute == minute)
    {
        collection[zone].enabled = false;
        return true;
    }

    return false;
}

