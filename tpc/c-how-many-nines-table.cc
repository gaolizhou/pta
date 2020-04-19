//
// Created by lizgao on 4/17/20.
//

#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>

using namespace std;
namespace {
    bool is_leap_year(int year) {
        return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
    }

    int contains_nine(int v) {
        int cnt = 0;
        while (v != 0) {
            cnt += (v % 10 == 9) ? 1 : 0;
            v = v / 10;
        }
        return cnt;
    }

    constexpr int nine_per_year = 3*12 + 30 - 1;


    int get_days(int year, int month, int day) {
        int d[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        int days = 0;

        if(is_leap_year(year)) {d[1]=29;}
        for (int i=0;i<month-1;i++) {
            days += d[i];
        }
        return days + day;
    }
    int count_nine_till_date(int year, int month, int day) {
        int d[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int nines = 0;

        if (is_leap_year(year)) { d[1] = 29; }
        for (int i = 0; i < month-1; ++i) {
            if(d[i] >=29) {
                nines +=3;
            } else {
                nines +=2;
            }
        }
        if (day >= 9) {
            nines++;
        }
        if (day >= 19) {
            nines++;
        }
        if (day >= 29) {
            nines++;
        }
        if (month > 9) {
            nines += 30;
        } else if (month == 9) {
            nines += day;
        }
        nines += get_days(year, month, day) * contains_nine(year);
        return nines;
    }


    array<int, 79999> data_table;

    int count_nine_between_year(int start, int end) {
        return data_table[end-2000] - data_table[start-2000];
    }

    int count_nine(const vector<int> &date_start, const vector<int> &date_end) {
        int nines = 0;
        nines += count_nine_between_year(date_start[0], date_end[0]);

        nines -= count_nine_till_date(date_start[0], date_start[1], date_start[2]);
        nines += contains_nine(date_start[0]) + contains_nine(date_start[1]) + contains_nine(date_start[2]);

        nines += count_nine_till_date(date_end[0], date_end[1], date_end[2]);

        return nines;
    }


    int test(int start, int end) {
        int nines = 0;
        for (int i = start; i < end; ++i) {
            nines += 3 * 12 + 30 - 1;
            if (is_leap_year(i)) {
                nines += 1;
            }
            nines += contains_nine(i) * (365 + is_leap_year(i));
        }
        return nines;
    }
    int get_nine_one_year(int year) {
        int nines = 0;
        nines += 3 * 12 + 30 - 1;
        if (is_leap_year(year)) {
            nines += 1;
        }
        nines += contains_nine(year) * (365 + is_leap_year(year));
        return nines;
    }

    void build_table() {
        data_table[0]=0;
        for (int i = 2001; i < 10000; ++i) {
            data_table[i-2000] = get_nine_one_year(i-1) + data_table[i-2000-1];
            //cout << data_table[i-2000] << ',';
        }
    }
}

void count_nine_table_main() {
    //cout << count_nine({2000, 01, 01}, {2001, 12, 31}) << endl;
    //cout << count_nine({9996, 2, 1}, {9996, 3, 1}) << endl;
    //cout << count_nine({2000, 01, 01}, {9999, 12, 31}) << endl;
    build_table();
    cout << count_nine({2000, 01, 01}, {9999, 12, 31}) << endl;
}
