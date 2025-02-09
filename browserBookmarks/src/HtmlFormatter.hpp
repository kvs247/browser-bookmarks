#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <types.hpp>

using json = nlohmann::json;

class HtmlFormatter
{
public:
  HtmlFormatter(BookmarkBar bb) : bookmarkBar(bb) {}

  std::string getHtml()
  {
    addHeading();
    addBookmarkItems();

    return ss.str();
  }

private:
  const BookmarkBar bookmarkBar;
  std::stringstream ss;
  int currentIndent = 0;

  void addHeading()
  {
    ss << "<!DOCTYPE NETSCAPE-Bookmark-file-1>\n";
    ss << "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">\n";
    ss << "<TITLE>Bookmarks</TITLE>\n";
    ss << "<H1>Bookmarks</H1>\n";
  }

  void addBookmarkItems()
  {
    ss << "<DL><p>\n";

    currentIndent = 1;
    addBookmarks(bookmarkBar.items);

    ss << "</DL><p>\n";
  }

  void addBookmarks(const std::vector<BookmarkItem> &items)
  {
    for (const auto &it : items)
    {
      if (std::holds_alternative<Bookmark>(it))
      {
        const auto &bookmark = std::get<Bookmark>(it);
        addSingleBookmark(bookmark);
      }
      else if (std::holds_alternative<BookmarkFolder>(it))
      {
        const auto &folder = std::get<BookmarkFolder>(it);
        const std::string indent(currentIndent, '\t');
        ss << indent << "<DT><H3>" << folder.name << "</H3>\n";
        ss << indent << "<DL><p>\n";

        ++currentIndent;
        addBookmarks(folder.items);
        --currentIndent;

        ss << indent << "</DL><p>\n";
      }
    }
  }

  void addSingleBookmark(const Bookmark &bookmark)
  {
    const std::string indent(currentIndent, '\t');
    ss << indent;
    ss << "<DT>";
    ss << "<A ";
    ss << "HREF=\"" << bookmark.url << "\" ";
    ss << "ADD_DATE=\"" << bookmark.addDate << "\"";
    ss << ">";
    ss << bookmark.name;
    ss << "</A>\n";
  }
};