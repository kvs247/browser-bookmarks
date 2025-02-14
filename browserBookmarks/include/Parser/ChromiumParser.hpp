#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <types.hpp>
#include <vector>

using json = nlohmann::json;

class ChromiumParser
{
public:
  static BookmarkData parseBookmarks(const std::string &path);

private:
  static std::vector<BookmarkItem> addBookmarkItems(json data);

  static long timestampFromJson(json j);

  static long windowsFiletimeToEpoch(long wft);
};