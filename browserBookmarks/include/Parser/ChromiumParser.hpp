#pragma once

#include "Parser.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <types.hpp>
#include <vector>

using json = nlohmann::json;

class ChromiumParser : public Parser
{
public:
  BookmarkData parseBookmarks(const std::string &path) const override;

private:
  static std::vector<BookmarkItem> addBookmarkItems(json data);

  static long timestampFromJson(json j);

  static long windowsFiletimeToEpoch(long wft);
};