#include "ChromiumParser.hpp"
#include "HtmlFormatter.hpp"
#include "paths.h"
#include "writeOutput.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

int main(int argc, char *argv[])
{
  const auto home = std::string(std::getenv("HOME"));
  std::string bookmarksPath = home + BRAVE_BOOKMARKS_PATH;

  for (int i = 1; i < argc; ++i)
  {
    const std::string arg = argv[i];

    if (arg == "--brave")
    {
      bookmarksPath = home + BRAVE_BOOKMARKS_PATH;
    }
    if (arg == "--opera")
    {
      bookmarksPath = home + OPERA_BOOKMARKS_PATH;
    }
  }

  const auto parsedJson = ChromiumParser::parseBookmarks(bookmarksPath);

  HtmlFormatter htmlFormatter(parsedJson);
  const auto html = htmlFormatter.getHtml();

  writeOutput(html);
}