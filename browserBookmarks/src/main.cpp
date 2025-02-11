#include <iostream>
#include <string>

#include "Manager.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[])
{
  Manager worker;

  const std::string arg = (argc > 1) ? argv[1] : "--help";
  if (arg == "--help")
  {
    std::cout << "~~~ Browser Bookmark Utility ~~~\n";
    std::cout << "Supported Browsers:\n" << "  Brave: --brave\n" << "  Opera: --opera\n";

    return 0;
  }
  else if (arg == "--brave")
  {
    worker.uploadFromBrave();
  }
  else if (arg == "--opera")
  {
    worker.uploadFromOpera();
  }
  else if (arg == "--download")
  {
    worker.downloadCurrentBookmarks();
  }

  return 0;
}