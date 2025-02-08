#pragma once

#include <vector>
#include <string>
#include <variant>

struct Bookmark {
  std::string name;
  std::string url;
  long addDate;
};

struct BookmarkFolder {
  std::string name;
  std::vector<std::variant<Bookmark, BookmarkFolder>> items;;
};

struct BookmarkBar {
  long addDate;
  long lastModified;
  std::vector<std::variant<Bookmark, BookmarkFolder>> items;
};