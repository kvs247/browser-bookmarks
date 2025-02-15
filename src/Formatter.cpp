#include <Formatter.hpp>
#include <chrono>
#include <iomanip>
#include <string>
#include <types.hpp>

Formatter::Formatter(BookmarkData bd) : bookmarkData(bd) {}

std::string Formatter::getHtml()
{
  addHeading();
  addBookmarkItems();

  return ss.str();
}

std::string Formatter::getFileName() { return "bookmarks_" + getDateTimeString() + ".html"; }

void Formatter::addHeading()
{
  ss << "<!DOCTYPE NETSCAPE-Bookmark-file-1>\n";
  ss << "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">\n";
  ss << "<TITLE>Bookmarks</TITLE>\n";
  ss << "<H1>Bookmarks</H1>\n";
}

void Formatter::addBookmarkItems()
{
  ss << "<DL><p>\n";

  currentIndent = 1;
  addBookmarks(bookmarkData.items);

  ss << "</DL><p>\n";
}

void Formatter::addBookmarks(const std::vector<BookmarkItem> &items)
{
  for (const auto &it : items)
  {
    if (std::holds_alternative<Bookmark>(it))
    {
      const auto &bookmark = std::get<Bookmark>(it);
      addSingleBookmark(bookmark);
    }
    else if (std::holds_alternative<BookmarkFolder>(it))
    {
      const auto &folder = std::get<BookmarkFolder>(it);
      const std::string indent(currentIndent, '\t');
      ss << indent << "<DT><H3>" << folder.name << "</H3>\n";
      ss << indent << "<DL><p>\n";

      ++currentIndent;
      addBookmarks(folder.items);
      --currentIndent;

      ss << indent << "</DL><p>\n";
    }
  }
}

void Formatter::addSingleBookmark(const Bookmark &bookmark)
{
  const std::string indent(currentIndent, '\t');
  ss << indent;
  ss << "<DT>";
  ss << "<A ";
  ss << "HREF=\"" << bookmark.url << "\" ";
  ss << "ADD_DATE=\"" << bookmark.addDate << "\"";
  ss << ">";
  ss << bookmark.name;
  ss << "</A>\n";
}

std::string Formatter::getDateTimeString()
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
  ss << std::to_string(year) << std::setfill('0') << std::setw(2) << month << std::setfill('0') << std::setw(2) << day
     << "_" << std::setfill('0') << std::setw(2) << hour << std::setfill('0') << std::setw(2) << minute
     << std::setfill('0') << std::setw(2) << second;

  return ss.str();
}