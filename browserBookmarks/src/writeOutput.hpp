#pragma once

#include <string>
#include <fstream>
#include "paths.h"
#include <chrono>
#include <sstream>

void writeOutput(const std::string &output)
{
  const auto now = std::chrono::system_clock::now();
  std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
  std::tm *timeInfo = std::gmtime(&timeNow);

  const int year = timeInfo->tm_year + 1900;
  const int month = timeInfo->tm_mon + 1;
  const int day = timeInfo->tm_mday;
  const int hour = timeInfo->tm_hour;
  const int minute = timeInfo->tm_min;
  const int second = timeInfo->tm_sec;

  std::stringstream ss;
  ss << "bookmarks_"
     << std::to_string(year)
     << std::setfill('0') << std::setw(2) << month
     << std::setfill('0') << std::setw(2) << day
     << "_"
     << std::setfill('0') << std::setw(2) << hour
     << std::setfill('0') << std::setw(2) << minute
     << std::setfill('0') << std::setw(2) << second
     << ".html";

  const std::string filepath = BOOKMARKS_DIR + ss.str();
  std::ofstream f(filepath);
  f.write(output.c_str(), output.length());
}