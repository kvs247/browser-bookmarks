#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct Bookmark;
struct BookmarkFolder;

using BookmarkItem = std::variant<Bookmark, BookmarkFolder>;

struct Bookmark
{
  std::string name;
  std::string url;
  long addDate;
};

struct BookmarkFolder
{
  std::string name;
  std::vector<BookmarkItem> items;
};

struct BookmarkBar
{
  long addDate;
  long lastModified;
  std::vector<BookmarkItem> items;

  void print() const
  {
    std::cout << "BookmarkBar:\n";
    for (const auto &x : items)
    {
      if (std::holds_alternative<Bookmark>(x))
      {
        const auto &bookmark = std::get<Bookmark>(x);
        std::cout << "bookmark: " << bookmark.name << "\n";
      }
      else if (std::holds_alternative<BookmarkFolder>(x))
      {
        const auto &folder = std::get<BookmarkFolder>(x);
        std::cout << "folder " << folder.name << " with " << folder.items.size() << " items\n";
      }
    }
    std::cout << "\n\n";
  }
};