#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <types.hpp>

using json = nlohmann::json;

class ChromiumParser
{
public:
  static BookmarkBar parseBookmarks(const std::string &path)
  {
    std::ifstream f(path);
    if (!f.is_open())
    {
      throw std::runtime_error("error: failed to open file " + path);
    }

    json fileJson = json::parse(f);

    BookmarkBar parsedJson;
    parsedJson.addDate = timestampFromJson(fileJson["roots"]["bookmark_bar"]["date_added"]);
    parsedJson.lastModified = timestampFromJson(fileJson["roots"]["bookmark_bar"]["date_modified"]);

    auto bookmarkItems = fileJson["roots"]["bookmark_bar"]["children"];
    parsedJson.items = addBookmarkItems(bookmarkItems);

    return parsedJson;
  }

private:
  static std::vector<BookmarkItem> addBookmarkItems(json data)
  {
    std::vector<BookmarkItem> res;
    for (auto item : data)
    {
      const bool isFolder = item.contains("children");
      if (isFolder)
      {
        res.push_back(BookmarkFolder{item["name"], addBookmarkItems(item["children"])});
      }
      else
      {
        const auto name = item["name"];
        const auto url = item["url"];
        const auto addDate = timestampFromJson(item["date_added"]);
        res.push_back(Bookmark{name, url, addDate});
      }
    }
    return res;
  }

  static long timestampFromJson(json j) { return windowsFiletimeToEpoch(std::stol(j.get<std::string>())); }

  static long windowsFiletimeToEpoch(long wft)
  {
    static const long differenceUs = 11646297200000000; // microseconds between Jan 1 1601 - 1970
    return (wft - differenceUs) / 1e6;
  }
};