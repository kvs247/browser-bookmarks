#include <Manager.hpp>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  Manager manager;
  BrowserType browserType;

  const std::string arg = (argc > 1) ? argv[1] : "--help";
  if (arg == "--help")
  {
    std::cout << "~~~ Browser Bookmark Utility ~~~\n";
    std::cout << "Supported Browsers:\n" << "  Brave: --brave\n" << "  Opera: --opera\n";

    return 0;
  }
  else if (arg == "--download")
  {
    manager.downloadCurrentBookmarks();

    return 0;
  }
  else if (arg == "--brave")
  {
    browserType = BrowserType::Brave;
  }
  else if (arg == "--opera")
  {
    browserType = BrowserType::Opera;
  }
  else if (arg == "--firefox")
  {
    browserType = BrowserType::Firefox;
  }

  manager.uploadFromBrowser(browserType);

  return 0;
}