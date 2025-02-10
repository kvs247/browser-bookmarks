#include <iostream>
#include <string>

#include "ChromiumParser.hpp"
#include "Formatter.hpp"
#include "awsS3.hpp"
#include "paths.h"

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

  Formatter formatter(parsedJson);
  const auto html = formatter.getHtml();

  const std::string bucketName = "kvs-bookmarks";
  const std::string region = "us-east-2";
  const std::string fileName = Formatter::getFileName();

  s3PutObject(bucketName, fileName, html, region);
}