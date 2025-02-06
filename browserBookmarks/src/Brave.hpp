#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

using json = nlohmann::json;

class Brave
{
public:
  static json parseBookmarks(const std::string &path)
  {
    std::ifstream f(path);
    if (!f.is_open())
    {
      throw std::runtime_error("error: failed to open file " + path);
    }

    json fileJson = json::parse(f);

    json parsedJson;
    parsedJson["add_date"] = fileJson["roots"]["bookmark_bar"]["date_added"];
    parsedJson["last_modified"] = fileJson["roots"]["bookmark_bar"]["date_modified"];
    parsedJson["bookmarks"] = json::array();

    auto bookmarkItems = fileJson["roots"]["bookmark_bar"]["children"];
    for (auto item : bookmarkItems)
    {
      json jsonBookmark;
      jsonBookmark["href"] = item["url"];
      jsonBookmark["add_date"] = item["date_added"];
      jsonBookmark["text"] = item["name"];

      parsedJson["bookmarks"].push_back(jsonBookmark);
    }

    return parsedJson;
  }

  static json parseBookmarks()
  {
    return parseBookmarks(getDefaultPath());
  }

private:
  static std::string getDefaultPath()
  {
    const auto home = std::getenv("HOME");
    return std::string(home) + "/.config/BraveSoftware/Brave-Browser/Default/Bookmarks";
  }
};