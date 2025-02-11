#pragma once

#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <memory>
#include <sstream>
#include <string>

class AwsS3
{
public:
  AwsS3(const std::string &bn, const std::string &r) : bucketName(bn), region(r)
  {
    clientConfig.region = region;
    client = Aws::S3::S3Client(clientConfig);
  }

  std::stringstream getObject(const std::string &fileName) const
  {
    std::stringstream buffer;

    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(fileName);

    auto outcome = client.GetObject(request);

    if (outcome.IsSuccess())
    {
      buffer << outcome.GetResult().GetBody().rdbuf();
      std::cout << "successfully retrieved object " << fileName << " from bucket " << bucketName << "\n";
    }
    else
    {
      std::cout << "error retrieving object from S3: " << outcome.GetError().GetMessage() << std::endl;
    }

    return buffer;
  }

  bool putObject(const std::string &fileName, const std::string &fileContent) const
  {
    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(fileName);

    std::shared_ptr<Aws::IOStream> inputData = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream");
    *inputData << fileContent;
    request.SetBody(inputData);

    auto outcome = client.PutObject(request);

    if (outcome.IsSuccess())
    {
      std::cout << "added object " << fileName << " to bucket " << bucketName << "\n";
    }
    else
    {
      std::cerr << "error: putObject: " << outcome.GetError().GetMessage() << std::endl;
    }

    return outcome.IsSuccess();
  }

private:
  const std::string bucketName;
  const std::string region;
  Aws::Client::ClientConfiguration clientConfig;
  Aws::S3::S3Client client;
};
