#pragma once

#include <aws/core/Aws.h>
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
  AwsS3(const std::string &bn, const std::string &r) : bucketName(bn), region(r) {}

  std::stringstream getObject(const std::string &fileName)
  {
    std::stringstream buffer;

    Aws::SDKOptions options;

    Aws::InitAPI(options);
    {
      Aws::Client::ClientConfiguration clientConfig;
      clientConfig.region = region;

      Aws::S3::S3Client s3Client(clientConfig);

      Aws::S3::Model::GetObjectRequest request;
      request.SetBucket(bucketName);
      request.SetKey(fileName);

      auto outcome = s3Client.GetObject(request);

      if (outcome.IsSuccess())
      {
        std::cout << "successfully retrieved object from S3\n";
        buffer << outcome.GetResult().GetBody().rdbuf();
      }
      else
      {
        std::cout << "error retrieving object from S3: " << outcome.GetError().GetMessage() << std::endl;
      }
    }
    Aws::ShutdownAPI(options);

    return buffer;
  }

  int putObject(const std::string &fileName, const std::string &fileContents)
  {
    Aws::SDKOptions options;

    Aws::InitAPI(options);
    {
      Aws::Client::ClientConfiguration clientConfig;
      clientConfig.region = region;

      Aws::S3::S3Client s3Client(clientConfig);

      Aws::S3::Model::PutObjectRequest request;
      request.SetBucket(bucketName);
      request.SetKey(fileName);

      std::shared_ptr<Aws::IOStream> inputData = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream");
      *inputData << fileContents;
      request.SetBody(inputData);

      auto outcome = s3Client.PutObject(request);

      if (outcome.IsSuccess())
      {
        std::cout << "successfully put object in S3 bucket\n";
      }
      else
      {
        std::cout << "error putting object in s3 bucket: " << outcome.GetError().GetMessage() << std::endl;
      }
    }
    Aws::ShutdownAPI(options);

    return 0;
  }

private:
  const std::string bucketName;
  const std::string region;
};
