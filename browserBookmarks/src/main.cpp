#include <iostream>
#include "Brave.hpp"
#include "HtmlFormatter.hpp"
#include "writeOutput.hpp"

using json = nlohmann::json;

int main()
{
  const auto parsedJson = Brave::parseBookmarks();

  HtmlFormatter htmlFormatter(parsedJson);
  const auto html = htmlFormatter.getHtml();

  writeOutput(html);
}