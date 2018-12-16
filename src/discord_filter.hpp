#ifndef discord_filter_hpp
#define discord_filter_hpp

#include "discord_core.hpp"
#include <string>

namespace rocord {

/*
enum sensibility_mode {
  SOFT = 0x1,   // bad nickname: rename
                // bad username: nothing
                // bad word: censor
                //
  MEDIUM = 0x2, // bad nickname: rename
                // bad username: ban
                // bad word: remove message
                //
  HARD = 0x3    // bad nickname: ban
                // bad username: ban
                // bad word: remove message
}
*/
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
  int check_message(const std::string &str);
  void reload_lists();
private:
  name_sensibility name_s;
  word_sensibility word_s;
  core& dcore;
};
}
#endif /* discord_filter_hpp */

