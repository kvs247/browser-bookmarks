#pragma once

#include <map>
#include <set>
#include <sqlite3.h>
#include <types.hpp>
#include <vector>

class FirefoxParser
{
public:
  static BookmarkData parseBookmarks(const std::string &path)
  {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    int rc = sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
    if (rc)
    {
      std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
    }

    const char *sql = "SELECT * FROM moz_bookmarks b LEFT JOIN moz_places p ON b.fk = p.id;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    BookmarkData parsedJson;

    std::set<int> folderIds;
    // std::map<int, std::vector<BookmarkItem>> foldersMap;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {

      const auto id = sqlite3_column_int(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::ID));
      const auto type = sqlite3_column_int(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::TYPE));
      const auto parent = sqlite3_column_int(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::PARENT));
      const auto name = reinterpret_cast<const char *>(
          sqlite3_column_text(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::TITLE)));
      const auto url = reinterpret_cast<const char *>(
          sqlite3_column_text(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::URL)));
      const auto addDate = sqlite3_column_int64(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::ADD_DATE));

      if (std::string(name) == TOOLBAR)
      {
        folderIds.insert(id);
        // foldersMap[id] = {};
      }

      if (folderIds.find(parent) != folderIds.cend())
      {
        if (type == static_cast<int>(MOZ_BOOKMARKS_TYPE_COLUMN::FOLDER))
        {
          folderIds.insert(id);
        }

        if (type == static_cast<int>(MOZ_BOOKMARKS_TYPE_COLUMN::BOOKMARK))
        {
          parsedJson.items.push_back(Bookmark{name, url, addDate});
        }
      }
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return parsedJson;
  }

private:
  static constexpr const char *TOOLBAR = "toolbar";
  enum class MOZ_BOOKMARKS_PLACES_COLUMN
  {
    ID = 0,
    TYPE = 1,
    PARENT = 3,
    TITLE = 5,
    ADD_DATE = 8,
    URL = 14,
  };
  enum class MOZ_BOOKMARKS_TYPE_COLUMN
  {
    BOOKMARK = 1,
    FOLDER = 2,
  };
};