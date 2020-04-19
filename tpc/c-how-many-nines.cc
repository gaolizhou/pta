//
// Created by lizgao on 4/17/20.
//

#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;
namespace {

    constexpr int nine_per_year = 3 * 12 + 30 - 1;

    int contains_nine(int v) {
        int cnt = 0;
        while (v != 0) {
            cnt += (v % 10 == 9) ? 1 : 0;
            v = v / 10;
        }
        return cnt;
    }

    bool is_leap_year(int year) {
        return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
    }

    int count_nine_between_year(int start, int end) {
        int nines = 0;
        for (int i = start; i < end; ++i) {
            nines += contains_nine(i) * (365 + is_leap_year(i));
            nines += nine_per_year;
            nines += is_leap_year(i);
        }
        return nines;
    }
/*
int count_nine_between_year(int start, int end) {
    int nines = 0;
    for (int k = 0; k < 4; ++k) {
        int p = 1;
        for (int j = 0; j < k; ++j) {
            p *=10;
        }
        int m_start = start/(p*10)*p + start%p;
        int n_start = start/p%10;
        int m_end = end/(p*10)*p + end%p;
        int n_end = end/p%10;
        int x = m_end - m_start;
        if ((n_end>=9 && n_start<=9)) {
            nines += x;
        }
    }
    nines *= 365;
    int x = ((start & 0x4)==0x4) ? start : ((start&0xfffffffc)+4);

    for (int i = x; i < end; i = i + 4 ) {
        if(is_leap_year(i)) {
            nines += contains_nine(i) + 1;
        }
    }
    return nines;
}
*/
/*
int count_nine_between_year(int start, int end) {
    int nines = 0;
    for (int k = 0; k < 4; ++k) {
        int p = 1;
        for (int j = 0; j < k; ++j) {
            p *=10;
        }
        int m_start = start/(p*10)*p + start%p;
        int n_start = start/p%10;
        int m_end = end/(p*10)*p + end%p;
        int n_end = end/p%10;
        int x = m_end - m_start;
        if ((n_end>=9 && n_start<=9)) {
            nines += x;
        }
    }


    return nines;
}
*/
    int get_days(int year, int month, int day) {
        int d[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int days = 0;

        if (is_leap_year(year)) { d[1] = 29; }
        for (int i = 0; i < month - 1; i++) {
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
}
void count_nine_main()
{
    //cout << test(9996, 9999) << endl;
    //cout << count_nine_between_year(9996, 9999) << endl;
    //cout << count_nine_till_date(9997, 12, 31) << endl;
    //cout << count_nine_till_date(9996, 1, 1) << endl;
    //cout << contains_nine(9996) + contains_nine(1) + contains_nine(1) << endl;
    //cout << count_nine({2100, 02, 01}, {2100, 03, 01}) << endl;
    //cout << count_nine({9996, 2, 1}, {9996, 3, 1}) << endl;
    cout << count_nine({2000, 01, 01}, {9999, 12, 31}) << endl;
    //cout << count_nine({9996, 01, 01}, {9997, 12, 31}) << endl;
    //cout << is_leap_year(2000) << endl;
    //cout << count_nine_between_year(9000, 9999) << endl;
    //cout << contains_nine(9999) << endl;
    //cout << get_days(9996, 2,1) << endl;
    //cout << get_days(9996, 12,31) << endl;
    //cout << get_days(9999, 12,31) << endl;
    //cout << count_nine_between_year(9996, 9999) << endl;
    //cout <<  get_days(9999, 12, 31) * contains_nine(9999) << endl;
}
