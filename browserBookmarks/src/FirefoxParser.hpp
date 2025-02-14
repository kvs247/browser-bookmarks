#pragma once

#include <map>
#include <sqlite3.h>
#include <types.hpp>
#include <variant>
#include <vector>

class FirefoxParser
{
public:
  static BookmarkData parseBookmarks(const std::string &path)
  {
    sqlite3 *db = openDatabase(path);

    int rootFolderId = getRootFolderId(db);

    const FoldersMap foldersMap = buildFoldersMap(db, rootFolderId);

    sqlite3_close(db);

    const auto bookmarkData = folersMapToBookmarkData(foldersMap, rootFolderId);

    return bookmarkData;
  }

private:
  using FoldersMap = std::map<int, std::vector<BookmarkItem>>;

  static constexpr const char *ROOT_FOLDER_ID_QUERY = "SELECT id FROM moz_bookmarks WHERE title = 'toolbar';";
  static constexpr const char *TOOLBAR = "toolbar";
  static constexpr const char *BOOKMARK_DATA_QUERY = "SELECT * FROM moz_bookmarks b "
                                                     "LEFT JOIN moz_places p ON "
                                                     "b.fk = p.id;";

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

  static sqlite3 *openDatabase(const std::string &path)
  {
    sqlite3 *db;
    int rc = sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
    if (rc != SQLITE_OK)
    {
      std::string error = sqlite3_errmsg(db);
      sqlite3_close(db);
      throw std::runtime_error("failed to open database: " + error);
    }

    return db;
  }

  static sqlite3_stmt *prepareStatement(sqlite3 *db, const char *query)
  {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
      std::string error = sqlite3_errmsg(db);
      throw std::runtime_error("failed to prepare statement: " + error);
    }

    return stmt;
  }

  static int getRootFolderId(sqlite3 *db)
  {
    auto stmt = prepareStatement(db, ROOT_FOLDER_ID_QUERY);

    int id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
      id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    if (id < 0)
    {
      throw std::runtime_error("'toolbar' folder not found in database");
    }

    return id;
  }

  static FoldersMap buildFoldersMap(sqlite3 *db, int rootFolderId)
  {
    auto stmt = prepareStatement(db, BOOKMARK_DATA_QUERY);
    FoldersMap foldersMap = {{rootFolderId, {}}};

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {

      const auto itemId = sqlite3_column_int(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::ID));
      const auto parentId = sqlite3_column_int(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::PARENT));
      const auto type = sqlite3_column_int(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::TYPE));
      const auto name = reinterpret_cast<const char *>(
          sqlite3_column_text(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::TITLE)));
      const auto url = reinterpret_cast<const char *>(
          sqlite3_column_text(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::URL)));
      const auto addDate = sqlite3_column_int64(stmt, static_cast<int>(MOZ_BOOKMARKS_PLACES_COLUMN::ADD_DATE));

      if (auto it = foldersMap.find(parentId); it != foldersMap.end())
      {
        if (type == static_cast<int>(MOZ_BOOKMARKS_TYPE_COLUMN::FOLDER))
        {
          foldersMap[itemId] = {};
          it->second.emplace_back(BookmarkFolder{name, {}, itemId});
        }

        if (type == static_cast<int>(MOZ_BOOKMARKS_TYPE_COLUMN::BOOKMARK))
        {
          it->second.emplace_back(Bookmark{name, url, addDate});
        }
      }
    }

    sqlite3_finalize(stmt);
    return foldersMap;
  }

  static BookmarkData folersMapToBookmarkData(const FoldersMap &foldersMap, int rootFolderId)
  {
    BookmarkData bookmarkData;
    for (const auto &bookmarkItem : foldersMap.at(rootFolderId))
    {
      if (std::holds_alternative<BookmarkFolder>(bookmarkItem))
      {
        const auto &folder = std::get<BookmarkFolder>(bookmarkItem);
        bookmarkData.items.emplace_back(BookmarkFolder{folder.name, getBookmarkFolderItems(folder.id, foldersMap)});
      }
      if (std::holds_alternative<Bookmark>(bookmarkItem))
      {
        const auto &bookmark = std::get<Bookmark>(bookmarkItem);
        bookmarkData.items.push_back(bookmark);
      }
    }

    return bookmarkData;
  }

  static std::vector<BookmarkItem>
  getBookmarkFolderItems(int folderId, const std::map<int, std::vector<BookmarkItem>> &foldersMap)
  {
    const auto &items = foldersMap.at(folderId);
    std::vector<BookmarkItem> res;
    res.reserve(items.size());

    for (const auto &bookmarkItem : items)
    {
      if (std::holds_alternative<Bookmark>(bookmarkItem))
      {
        const auto &bookmark = std::get<Bookmark>(bookmarkItem);
        res.push_back(std::move(bookmark));
      }
      else if (std::holds_alternative<BookmarkFolder>(bookmarkItem))
      {
        const auto &folder = std::get<BookmarkFolder>(bookmarkItem);
        res.emplace_back(BookmarkFolder{folder.name, getBookmarkFolderItems(folder.id, foldersMap)});
      }
    }

    return res;
  }
};