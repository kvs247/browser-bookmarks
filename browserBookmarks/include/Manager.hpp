#pragma once

#include <AwsS3.hpp>
#include <aws/core/Aws.h>
#include <types.hpp>

class Manager
{
public:
  Manager();
  ~Manager();

  void uploadFromBrowser(BrowserType browserType) const;
  void downloadCurrentBookmarks() const;

private:
  Aws::SDKOptions sdkOptions;
  std::unique_ptr<AwsS3> s3Bucket;

  const std::string homePath = std::string(std::getenv("HOME"));
  const std::string bucketName = "kvs-bookmarks";
  const std::string region = "us-east-2";
  const std::string currentBookmarksFileName = "current.html";

  void uploadBookmarks(const std::string &fileContent) const;
  std::string jsonToHtml(const BookmarkData &json) const;
};