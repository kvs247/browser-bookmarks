#pragma once

#include <sqlite3.h>
#include <types.hpp>

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

    const char *sql = "SELECT b.id, b.title, p.url, b.dateAdded, b.lastModified "
                      "FROM moz_bookmarks b "
                      "LEFT JOIN moz_places p ON b.fk = p.id "
                      "WHERE b.type = 1;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    listTables(db);

    printTable(db, stmt, "moz_bookmarks");

    std::cout << "\n\n";

    printTable(db, stmt, "moz_places", 8);

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    // parsedJson.print();

    return {};
  }

private:
  static void listTables(sqlite3 *db)
  {
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    std::cout << "tables:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      const char *tableName = (const char *)sqlite3_column_text(stmt, 0);
      std::cout << "  " << tableName << "\n";
    }
    std::cout << "\n";
  }

  static void printTable(sqlite3 *db, sqlite3_stmt *stmt, const std::string &tableName, const int maxW = 13)
  {
    const std::string sql = "SELECT * FROM " + tableName + ";";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    // Print column names
    int cols = sqlite3_column_count(stmt);
    for (int i = 0; i < cols; i++)
    {
      std::cout << std::setw(maxW) << truncate(sqlite3_column_name(stmt, i), maxW) << " | ";
    }
    std::cout << "\n" << std::string(cols * 16, '-') << "\n";

    // Print all rows
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      for (int i = 0; i < cols; i++)
      {
        // Get value based on column type
        switch (sqlite3_column_type(stmt, i))
        {
        case SQLITE_INTEGER:
          std::cout << std::setw(maxW) << sqlite3_column_int(stmt, i);
          break;
        case SQLITE_FLOAT:
          std::cout << std::setw(maxW) << sqlite3_column_double(stmt, i);
          break;
        case SQLITE_TEXT:
        {
          std::string text((const char *)sqlite3_column_text(stmt, i));
          std::cout << std::setw(maxW) << truncate(text, maxW);
          break;
        }
        case SQLITE_NULL:
          std::cout << std::setw(maxW) << "NULL";
          break;
        default:
          std::cout << std::setw(maxW) << "???";
        }
        std::cout << " | ";
      }
      std::cout << "\n";
    }
  }

  static std::string truncate(const std::string &str, size_t width)
  {
    if (str.length() > width)
    {
      return str.substr(0, width - 3) + "...";
    }
    return str;
  }
};