#pragma once

#define MAX_DAY 31
#define MAX_MONTH 12
#define MAX_HOUR 24
#define MAX_MINUTE 60

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

typedef struct Time { // He 24h
    int hour;
    int minute;
} Time;


