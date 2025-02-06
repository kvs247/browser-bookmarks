#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

int main()
{
  const auto home = std::getenv("HOME");
  std::string path = std::string(home) + "/.config/BraveSoftware/Brave-Browser/Default/Bookmarks";

  std::ifstream f(path);
  if (!f.is_open())
  {
    throw std::runtime_error("error: failed to open file " + path);
  }

  json data = json::parse(f);

  std::cout << data.dump(4) << std::endl;
}