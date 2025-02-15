#pragma once

#include <memory>
#include <string>
#include <types.hpp>

class Parser
{
public:
  virtual ~Parser() = default;
  virtual BookmarkData parseBookmarks(const std::string &path) const = 0;
  static std::unique_ptr<Parser> createParser(BrowserType type);

protected:
  Parser() = default;
};