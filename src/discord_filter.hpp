#ifndef discord_filter_hpp
#define discord_filter_hpp

#include "discord_core.hpp"
#include <string>

namespace rocord {
enum class name_sensibility {
  BAN,
  RENAME,
  NOTHING
};

enum class word_sensibility {
  BAN,
  CENSOR,
  REMOVE,
  NOTHING
};

class filter {
public:
  filter(name_sensibility name_s, word_sensibility word_s, core& dcore
      /*, some lists */);
  virtual ~filter();
  int add_to_namelist(const std::string &name);
  int add_to_wordlist(const std::string &word);
  int check_name(const std::string &name);
  int check_message(std::string &str);
  void reload_lists();
  void chmod_name(name_sensibility mode);
  void chmod_word(word_sensibility mode);
private:
  name_sensibility name_mode;
  word_sensibility word_mode;
  std::vector<std::string> list;
  core& dcore;
};
}
#endif /* discord_filter_hpp */

