#ifndef SETTINGS_H
#define SETTINGS_H

#define HISTORY_SLOTS 288 // original 288 -> 24 hours
#define HISTORY_PERIOD 5  // original 5
#define MOISTURE_TARGET 40
#define CRITICAL_VALUE 80.00
#define WATERING_DAY_TIME_HOUR 6
#define WATERING_DAY_TIME_MINUTE 0
#define WATERING_PERIOD 30 // in minutes
#define WATERING_DEAD_PERIOD 1/6 // in hours
#define GRAPH_UPDATE_PERIOD 4 // in minutes so with a red line every 15th value, the red line marks 1 hour....... original 5

#endif