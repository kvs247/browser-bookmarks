#include <string>
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

class HtmlFormatter
{
public:
  HtmlFormatter(json j) : parsedJson(j) {} 

  std::string getHtml()
  {
    addHeading();
    addBookmarks();

    return ss.str();
  }

private:
  const json parsedJson;
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
    for (const auto &it : parsedJson["bookmarks"])
    {
      const auto x = it["text"];
      ss << "\t\t<DT>";
      ss << "<A ";
      ss << "HREF=" << it["href"] << " ";
      ss << "ADD_DATE=\"" << it["add_date"] << "\"";
      ss << ">";
      ss << it["text"].get<std::string>();
      ss << "</A>\n";
    }
    ss << "</DL><p>\n";
  }
};