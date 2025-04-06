//
// Created by 86158 on 2024/11/1.
//

#ifndef MY_DATE_H
#define MY_DATE_H

#include <string>

class myDate {
public:
    /**
     * Tomohiko Sakamoto 算法获取星期
     * @param year
     * @param month
     * @param day
     * @return int
     */
    static int dow(int year, int month, int day);

    /**
     * 获取星期的字符串
     * @param day (0~6)
     * @return
     */
    static std::string getDowString(int day);

    myDate();

private:
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    int ms; // milliseconds

public:
    [[nodiscard]] int getYear() const;
    [[nodiscard]] int getMonth() const;
    [[nodiscard]] int getDay() const;
    [[nodiscard]] int getHour() const;
    [[nodiscard]] int getMinute() const;
    [[nodiscard]] int getSecond() const;
    [[nodiscard]] int getMillisecond() const;
    [[nodiscard]] std::string getDow() const;
};

#endif //MY_DATE_H
