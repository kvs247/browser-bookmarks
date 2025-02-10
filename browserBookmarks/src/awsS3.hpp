#pragma once

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

std::string getDateTimeString()
{
  const auto now = std::chrono::system_clock::now();
  std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
  std::tm *timeInfo = std::gmtime(&timeNow);

  const int year = timeInfo->tm_year + 1900;
  const int month = timeInfo->tm_mon + 1;
  const int day = timeInfo->tm_mday;
  const int hour = timeInfo->tm_hour;
  const int minute = timeInfo->tm_min;
  const int second = timeInfo->tm_sec;

  std::stringstream ss;
  ss << std::to_string(year) << std::setfill('0') << std::setw(2) << month << std::setfill('0') << std::setw(2) << day
     << "_" << std::setfill('0') << std::setw(2) << hour << std::setfill('0') << std::setw(2) << minute
     << std::setfill('0') << std::setw(2) << second;

  return ss.str();
}

std::string getFileName() { return "bookmarks_" + getDateTimeString() + ".html"; }

int s3PutObject(
    const std::string &bucketName,
    const std::string &fileName,
    const std::string &fileContents,
    const std::string &region)
{
  Aws::SDKOptions options;

  Aws::InitAPI(options);
  {
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = "us-east-2";

    Aws::S3::S3Client s3Client(clientConfig);

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket("kvs-bookmarks");
    request.SetKey(fileName);

    std::shared_ptr<Aws::IOStream> inputData = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream");
    *inputData << fileContents;
    request.SetBody(inputData);

    auto outcome = s3Client.PutObject(request);

    if (outcome.IsSuccess())
    {
      std::cout << "success\n";
    }
    else
    {
      std::cout << "error: " << outcome.GetError().GetMessage() << std::endl;
    }
  }
  Aws::ShutdownAPI(options);

  return 0;
}