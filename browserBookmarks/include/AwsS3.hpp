#pragma once

#include <aws/s3/S3Client.h>
#include <sstream>
#include <string>

class AwsS3
{
public:
  AwsS3(const std::string &bucketName, const std::string &region);

  std::stringstream getObject(const std::string &fileName) const;

  bool putObject(const std::string &fileName, const std::string &fileContent) const;

private:
  const std::string bucketName;
  const std::string region;
  Aws::Client::ClientConfiguration clientConfig;
  Aws::S3::S3Client client;
};