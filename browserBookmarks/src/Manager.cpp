#include <AwsS3.hpp>
#include <Formatter.hpp>
#include <Manager.hpp>
#include <Parser.hpp>
#include <filesystem>
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
  const auto bookmarksFilePath = getBrowserBookmarksFilePath(browserType);
  auto parser = Parser::createParser(browserType);
  const auto parsedJson = parser->parseBookmarks(bookmarksFilePath);
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

std::string Manager::getBrowserBookmarksFilePath(BrowserType browserType) const
{
  std::string pathSuffix;
  std::string bookmarksFileName;
  switch (browserType)
  {
  case BrowserType::Brave:
    pathSuffix = BRAVE_DIR_PATH;
    bookmarksFileName = CHROMIUM_BOOKMARKS_FILENAME;
    break;
  case BrowserType::Opera:
    pathSuffix = OPERA_DIR_PATH;
    bookmarksFileName = CHROMIUM_BOOKMARKS_FILENAME;
    break;
  case BrowserType::Firefox:
    pathSuffix = FIREFOX_DIR_PATH;
    bookmarksFileName = FIREFOX_BOOKMARKS_FILENAME;
    break;
  }

  std::vector<std::string> matches;
  for (const auto &dirEntry : std::filesystem::recursive_directory_iterator(homePath + pathSuffix))
  {
    if (dirEntry.path().string().ends_with(bookmarksFileName)) // ends_with() from C++ 20
    {
      matches.emplace_back(dirEntry.path().string());
    }
  }

  if (matches.size() == 0)
  {
    throw std::runtime_error("error: bookmarks file not found");
  }

  const auto res = matches[0];

  if (matches.size() > 1)
  {
    std::cout << "Warning: found multiple paths to bookmarks file. Using " << res << "\n";
  }
  else
  {
    std::cout << "reading bookmarks from " << res << "\n";
  }

  return res;
}