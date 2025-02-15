#include <ChromiumParser.hpp>
#include <FirefoxParser.hpp>
#include <Parser.hpp>
#include <memory>
#include <stdexcept>

std::unique_ptr<Parser> Parser::createParser(BrowserType type)
{
  switch (type)
  {
  case BrowserType::Brave:
  case BrowserType::Opera:
    return std::make_unique<ChromiumParser>();
  case BrowserType::Firefox:
    return std::make_unique<FirefoxParser>();
  default:
    throw std::runtime_error("unsupported browser type");
  }
}