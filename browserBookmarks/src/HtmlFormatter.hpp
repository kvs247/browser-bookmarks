#include <string>
#include <nlohmann/json.hpp>
#include <sstream>
#include <types.hpp>

using json = nlohmann::json;

class HtmlFormatter
{
public:
  HtmlFormatter(BookmarkBar bb) : bookmarkBar(bb) {}

  std::string getHtml()
  {
    addHeading();
    addBookmarks();

    return ss.str();
  }

private:
  const BookmarkBar bookmarkBar;
  std::stringstream ss;

  void addHeading()
  {
    ss << "<!DOCTYPE NETSCAPE-Bookmark-file-1>\n";
    ss << "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">\n";
    ss << "<TITLE>Bookmarks</TITLE>\n";
    ss << "<H1>Bookmarks</H1>\n";
  }

  void addBookmarks()
  {
    ss << "<DL><p>\n";
    for (const auto &it : bookmarkBar.items)
    {
      if (std::holds_alternative<Bookmark>(it))
      {
        const auto bookmark = std::get<Bookmark>(it);
        ss << "\t<DT>";
        ss << "<A ";
        ss << "HREF=" << bookmark.name << " ";
        ss << "ADD_DATE=\"" << bookmark.addDate << "\"";
        ss << ">";
        ss << bookmark.name;
        ss << "</A>\n";
      }
    }
    ss << "</DL><p>\n";
  }
};