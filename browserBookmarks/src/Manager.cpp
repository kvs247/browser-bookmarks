#include <AwsS3.hpp>
#include <Formatter.hpp>
#include <Manager.hpp>
#include <Parser.hpp>
#include <fstream>

#include "paths.h"

Manager::Manager()
{
  Aws::InitAPI(sdkOptions);
  s3Bucket = std::make_unique<AwsS3>(bucketName, region);
}

Manager::~Manager() { Aws::ShutdownAPI(sdkOptions); }

void Manager::uploadFromBrowser(BrowserType browserType) const
{
  std::string path;
  switch (browserType)
  {
  case BrowserType::Brave:
    path = homePath + BRAVE_BOOKMARKS_PATH;
    break;
  case BrowserType::Opera:
    path = homePath + OPERA_BOOKMARKS_PATH;
    break;
  case BrowserType::Firefox:
    path = homePath + "/.mozilla/firefox/4ka5c9m2.default-release/places.sqlite";
    break;
  }

  auto parser = Parser::createParser(browserType);
  const auto parsedJson = parser->parseBookmarks(path);
  const auto fileContent = jsonToHtml(parsedJson);
  uploadBookmarks(fileContent);
}

void Manager::downloadCurrentBookmarks() const
{
  const std::string filePath = homePath + "/Downloads/" + currentBookmarksFileName;
  std::ofstream f(filePath);
  if (!f)
  {
    std::cerr << "error opening file " << filePath << std::endl;
  }

  const auto s3Object = s3Bucket->getObject(currentBookmarksFileName);
  f << s3Object.rdbuf();

  std::cout << "downloaded bookmarks to " << filePath << "\n";
}

void Manager::uploadBookmarks(const std::string &fileContent) const
{
  const auto fileName = Formatter::getFileName();
  s3Bucket->putObject(fileName, fileContent);
  s3Bucket->putObject(currentBookmarksFileName, fileContent);
}

std::string Manager::jsonToHtml(const BookmarkData &json) const
{
  Formatter formatter(json);
  return formatter.getHtml();
}