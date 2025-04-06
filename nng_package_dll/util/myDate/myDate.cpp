//
// Created by 86158 on 2024/11/1.
//

#include "myDate.h"

#include <chrono>
#include <ctime>

int myDate::dow(int year, const int month, const int day) {
    year -= month < 3;

    // "-bed=pen+mad." : static int t[] = {0,3,2,5,0,3,5,1,4,6,2,4}
    return (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[month] + day) % 7;
}

std::string myDate::getDowString(const int day) {
    static std::string days[7] = {"Sun.", "Mon.", "Tue.", "Wed.", "Thur.", "Fri.", "Sat."};

    if (day < 0 || day > 6) {
        return "";
    }

    return days[day];
}

myDate::myDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm p{};
    localtime_s(&p, &tt);

    year = p.tm_year + 1900;
    month = p.tm_mon + 1;
    day = p.tm_mday;
    hour = p.tm_hour;
    min = p.tm_min;
    sec = p.tm_sec;

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    );
    ms = duration.count() % 1000; // 提取毫秒部分
}

int myDate::getYear() const {
    return year;
}

int myDate::getMonth() const {
    return month;
}

int myDate::getDay() const {
    return day;
}

int myDate::getHour() const {
    return hour;
}

int myDate::getMinute() const {
    return min;
}

int myDate::getSecond() const {
    return sec;
}

int myDate::getMillisecond() const {
    return ms;
}

std::string myDate::getDow() const {
    return getDowString(dow(year, month, day));
}
