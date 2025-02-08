#include <iostream>
#include "Brave.hpp"
#include "HtmlFormatter.hpp"

#include <fstream>

using json = nlohmann::json;

int main()
{
  const auto parsedJson = Brave::parseBookmarks();

  HtmlFormatter htmlFormatter(parsedJson);
  const auto html = htmlFormatter.getHtml();

  std::cout << "html:\n" << html << "\n\n";

  const std::string path = "bookmarks.html";
  std::ofstream f(path);
  f.write(html.c_str(), html.length());
}