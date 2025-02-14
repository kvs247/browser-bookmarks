#pragma once

#include <sstream>
#include <types.hpp>
#include <vector>

class Formatter
{
public:
  Formatter(BookmarkData bd);

  std::string getHtml();

  static std::string getFileName();

private:
  const BookmarkData bookmarkData;
  std::stringstream ss;
  int currentIndent = 0;

  void addHeading();
  void addBookmarkItems();
  void addBookmarks(const std::vector<BookmarkItem> &items);
  void addSingleBookmark(const Bookmark &bookmark);
  static std::string getDateTimeString();
};