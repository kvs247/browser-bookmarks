#include <iostream>
#include <string>

#include "AwsS3.hpp"
#include "ChromiumParser.hpp"
#include "Formatter.hpp"
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

  // begin AWS operations

  Aws::SDKOptions options;
  Aws::InitAPI(options);

  AwsS3 s3Bucket(bucketName, region);

  s3Bucket.putObject(fileName, html);
  s3Bucket.putObject("current.html", html);

  std::cout << s3Bucket.getObject("current.html").str() << "\n";

  Aws::ShutdownAPI(options);

  return 0;
}