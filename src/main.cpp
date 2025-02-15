#include <Manager.hpp>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  Manager manager;
  BrowserType browserType;
  bool validBrowserSelected = false;

  const std::string arg = (argc > 1) ? argv[1] : "--help";
  if (arg == "--help")
  {
    std::cout << "~~~ Browser Bookmark Utility ~~~\n";
    std::cout << "Download latest bookmarks file: --download\n";
    std::cout << "Supported Browsers:\n" << "  Brave: --brave\n" << "  Opera: --opera\n" << "  Firefox: --firefox\n";

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
    validBrowserSelected = true;
  }
  else if (arg == "--opera")
  {
    browserType = BrowserType::Opera;
    validBrowserSelected = true;
  }
  else if (arg == "--firefox")
  {
    browserType = BrowserType::Firefox;
    validBrowserSelected = true;
  }

  if (validBrowserSelected)
  {
    manager.uploadFromBrowser(browserType);
    return 0;
  }

  std::cout << "unkown command: " << arg << "\n";

  return 0;
}