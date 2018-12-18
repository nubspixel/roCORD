#ifndef discord_filter_hpp
#define discord_filter_hpp

#include "discord_core.hpp"
#include "discord_member.hpp"
#include <string>

namespace rocord {
enum class nickname_sensibility {
  BAN,
  DELETE,
  RENAME,
  NOTHING
};

enum class username_sensibility {
  BAN,
  FORCE_NICK,
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
  filter(username_sensibility user_mode, nickname_sensibility nick_mode,
      word_sensibility word_mode, core& dcore
      /*, some lists */);
  virtual ~filter();
  int add_to_namelist(const std::string &name);
  int add_to_wordlist(const std::string &word);
  int check_name(member &memb);
  int check_message(std::string &str);
  void reload_lists();
  void chmod_username(username_sensibility mode);
  void chmod_nickname(nickname_sensibility mode);
  void chmod_word(word_sensibility mode);
private:
  username_sensibility user_mode;
  nickname_sensibility nick_mode;
  word_sensibility word_mode;
  std::vector<std::string> list;
  core& dcore;
};
}
#endif /* discord_filter_hpp */

