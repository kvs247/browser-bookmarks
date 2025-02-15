#pragma once

#include <map>
#include <sqlite3.h>
#include <string>
#include <types.hpp>

#include "Parser.hpp"

class FirefoxParser : public Parser
{
public:
  BookmarkData parseBookmarks(const std::string &path) const override;

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

  static sqlite3 *openDatabase(const std::string &path);
  static sqlite3_stmt *prepareStatement(sqlite3 *db, const char *query);
  static int getRootFolderId(sqlite3 *db);
  static FoldersMap buildFoldersMap(sqlite3 *db, int rootFolderId);
  static BookmarkData foldersMapToBookmarkData(const FoldersMap &foldersMap, int rootFolderId);
  static std::vector<BookmarkItem>
  getBookmarkFolderItems(int folderId, const std::map<int, std::vector<BookmarkItem>> &foldersMap);
};