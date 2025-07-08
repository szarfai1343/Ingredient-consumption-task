#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#pragma warning(disable : 4996)

struct contract {
    std::string start;
    std::string end;
};

struct contract_timet {
    time_t start;
    time_t end;
};

std::pair<time_t, int> convertToWarsaw(int year, int month, int day, int hour, int minute, int second, int offset_hours, int offset_minutes) {
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    time_t local_time = _mkgmtime(&tm);
    int offset_seconds = (offset_hours * 3600) + (offset_minutes * 60);
    time_t UTC = local_time + offset_seconds;

    int warsaw_offset = (month >= 4 && month <= 10) ? 2 : 1;
    offset_seconds = warsaw_offset * 3600;
    time_t warsaw_time = UTC + offset_seconds;

    return { warsaw_time, warsaw_offset };
}

int getYear(const std::string& date) { return std::stoi(date.substr(0, 4)); }
int getMonth(const std::string& date) { return std::stoi(date.substr(5, 2)); }
int getDay(const std::string& date) { return std::stoi(date.substr(8, 2)); }
int getHour(const std::string& date) { return std::stoi(date.substr(11, 2)); }
int getMinute(const std::string& date) { return std::stoi(date.substr(14, 2)); }
int getSecond(const std::string& date) { return std::stoi(date.substr(17, 2)); }
int getOffsetHour(const std::string& date) { char sign = date[19]; int offset = std::stoi(date.substr(20, 2)); return sign == '-' ? offset : -offset; }
int getOffsetMinute(const std::string& date) { char sign = date[19]; int offset = std::stoi(date.substr(23, 2)); return sign == '-' ? offset : -offset; }

std::string formatTime(time_t t, int offset) {
    std::tm* tm = std::gmtime(&t);
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer),
        "%04d-%02d-%02dT%02d:%02d:%02d+%02d:00",
        tm->tm_year + 1900,
        tm->tm_mon + 1, 
        tm->tm_mday,
        tm->tm_hour, 
        tm->tm_min, 
        tm->tm_sec,
        offset
    );
    return std::string(buffer);
}

int main() {
    std::string report_start = "2022-04-09T00:00:00+02:00";
    std::string report_end = "2023-04-08T00:00:00+02:00";

    std::vector<contract> data{
        {"2022-05-08T19:00:00-03:00", "2022-05-16T16:00:00-06:00"},
        {"2023-03-16T15:00:00-08:00", ""},
        {"2022-01-06T03:30:00+04:30", "2022-03-09T18:00:00-05:00"},
        {"2023-02-27T17:00:00-06:00", "2023-03-16T15:00:00-08:00"},
        {"2022-05-26T16:00:00-06:00", "2023-02-27T17:00:00-06:00"},
        {"2022-04-07T08:00:00+10:00", "2022-04-22T05:00:00+07:00"}
    };

    std::vector<contract_timet> timet_data(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        const std::string& startStr = data[i].start;
        auto start_time = convertToWarsaw(getYear(startStr), getMonth(startStr), getDay(startStr), getHour(startStr), getMinute(startStr), getSecond(startStr), getOffsetHour(startStr), getOffsetMinute(startStr));
        timet_data[i].start = start_time.first;

        if (!data[i].end.empty()) {
            const std::string& endStr = data[i].end;
            auto end_time = convertToWarsaw(getYear(endStr), getMonth(endStr), getDay(endStr), getHour(endStr), getMinute(endStr), getSecond(endStr), getOffsetHour(endStr), getOffsetMinute(endStr));
            timet_data[i].end = end_time.first;
        }
        else {
            timet_data[i].end = 0; 
        }
    }

    auto rpt_start = convertToWarsaw(getYear(report_start), getMonth(report_start), getDay(report_start), getHour(report_start), getMinute(report_start), getSecond(report_start), getOffsetHour(report_start), getOffsetMinute(report_start));
    auto rpt_end = convertToWarsaw(getYear(report_end), getMonth(report_end), getDay(report_end), getHour(report_end), getMinute(report_end), getSecond(report_end), getOffsetHour(report_end), getOffsetMinute(report_end));

    time_t report_start_time = rpt_start.first;
    time_t report_end_time = rpt_end.first;

    std::vector<std::pair<time_t, time_t>> clipped_ranges;

    for (const auto& c : timet_data) {
        time_t start = std::max(c.start, report_start_time);
        time_t end = (c.end == 0 ? report_end_time : std::min(c.end, report_end_time));
        if (start < end) {
            clipped_ranges.emplace_back(start, end);
        }
    }

    std::sort(clipped_ranges.begin(), clipped_ranges.end());
    std::vector<std::pair<time_t, time_t>> merged;

    for (const auto& range : clipped_ranges) {
        if (merged.empty() || merged.back().second < range.first) {
            merged.push_back(range);
        }
        else {
            merged.back().second = range.second;
        }
    }

    std::vector<std::pair<time_t, time_t>> full_ranges;
    time_t current = report_start_time;

    for (const auto& m : merged) {
        if (current < m.first) {
            full_ranges.emplace_back(current, m.first); 
        }
        full_ranges.emplace_back(m.first, m.second); 
        current = m.second;
    }
    if (current < report_end_time) {
        full_ranges.emplace_back(current, report_end_time);
    }

    std::cout << "Wynik\nPOCZATEK                  KONIEC\n";
    for (const auto& range : full_ranges) {
        int start_month = getMonth(formatTime(range.first, 0));
        int end_month = getMonth(formatTime(range.second, 0));
        int start_offset = (start_month >= 4 && start_month <= 10) ? 2 : 1;
        int end_offset = (end_month >= 4 && end_month <= 10) ? 2 : 1;

        std::cout << formatTime(range.first, start_offset) << " ";
        std::cout << formatTime(range.second, end_offset) << "\n";
    }

    return 0;
}
