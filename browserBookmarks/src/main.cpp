#include <iostream>
#include "Brave.hpp"

using json = nlohmann::json;

int main()
{
  const auto parsedJson = Brave::parseBookmarks();

  std::cout << "result:\n" << parsedJson.dump(2) << "\n";
}