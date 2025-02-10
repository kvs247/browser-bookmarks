#pragma once

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <memory>
#include <string>

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