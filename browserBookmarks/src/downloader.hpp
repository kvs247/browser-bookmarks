#pragma once

#include "AwsS3.hpp"
#include <fstream>

void downloadCurrentBookmarks(
    const AwsS3 &s3Bucket,
    const std::string &currentBookmarksFileName,
    const std::string &homePath)
{
  const std::string filePath = homePath + "/Downloads/" + currentBookmarksFileName;
  std::ofstream f(filePath);
  if (!f)
  {
    std::cerr << "error opening file " << filePath << std::endl;
  }

  const auto s3Object = s3Bucket.getObject(currentBookmarksFileName);
  f << s3Object.rdbuf();

  std::cout << "downloaded bookmarks to " << filePath << "\n";
}
