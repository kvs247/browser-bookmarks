#pragma once

#include "AwsS3.hpp"
#include "ChromiumParser.hpp"
#include "FirefoxParser.hpp"
#include "Formatter.hpp"
#include "paths.h"
#include <aws/core/Aws.h>
#include <iostream>
#include <memory>
#include <string>
#include <types.hpp>

class Manager
{
public:
  Manager()
  {
    Aws::InitAPI(sdkOptions);
    s3Bucket = std::make_unique<AwsS3>(bucketName, region);
  }

  ~Manager() { Aws::ShutdownAPI(sdkOptions); }

  void uploadFromBrave() const
  {
    const std::string path = homePath + BRAVE_BOOKMARKS_PATH;
    const auto parsedJson = ChromiumParser::parseBookmarks(path);

    const auto fileContent = jsonToHtml(parsedJson);
    uploadBookmarks(fileContent);
  }

  void uploadFromOpera() const
  {
    const std::string path = homePath + OPERA_BOOKMARKS_PATH;
    const auto parsedJson = ChromiumParser::parseBookmarks(path);

    const auto fileContent = jsonToHtml(parsedJson);
    uploadBookmarks(fileContent);
  }

  void uploadFromFirefox() const
  {
    const std::string path = homePath + "/.mozilla/firefox/4ka5c9m2.default-release/places.sqlite";
    FirefoxParser::parseBookmarks(path);
  }

  void downloadCurrentBookmarks()
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

private:
  Aws::SDKOptions sdkOptions;
  std::unique_ptr<AwsS3> s3Bucket;
  const std::string homePath = std::string(std::getenv("HOME"));
  const std::string bucketName = "kvs-bookmarks";
  const std::string region = "us-east-2";
  const std::string currentBookmarksFileName = "current.html";

  void uploadBookmarks(const std::string &fileContent) const
  {
    const auto fileName = Formatter::getFileName();
    s3Bucket->putObject(fileName, fileContent);
    s3Bucket->putObject(currentBookmarksFileName, fileContent);
  }

  std::string jsonToHtml(const BookmarkData &json) const
  {
    Formatter formatter(json);
    return formatter.getHtml();
  }
};