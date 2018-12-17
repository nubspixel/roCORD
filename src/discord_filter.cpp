// some header
//
//

#include "discord_filter.hpp"

namespace rocord {
filter::filter(name_sensibility name_mode, word_sensibility word_mode, core& dcore) 
    : name_mode(name_mode), word_mode(word_mode), dcore(dcore)
{
}

int filter::add_to_namelist(const std::string &name)
{
  // update list
  // save to file
  return 0;
}

int filter::add_to_wordlist(const std::string &word)
{
  // update list
  // save to file
  return 0;
}

void filter::reload_lists()
{
  // reload namelist form file
  // reload wordlist from file
}

void filter::chmod_name(name_sensibility mode)
{
  name_mode = mode;
}

void filter::chmod_word(word_sensibility mode)
{
  word_mode = mode;
}

int filter::check_name(const std::string &name)
{
  if (false /* TODO: condition needed */) { //check if name is in list
    switch(name_mode) {
      case name_sensibility::BAN:
        // call ban in core
        break;
      case name_sensibility::RENAME:
        // call change nick in core & change name var
      case name_sensibility::NOTHING:
      default:
        return 0;
    }
    return -1;
  }
  return 0;
}

int filter::check_message(std::string &str)
{
  if (false /* TODO: condition needed */) { //check if word is in list
    switch(word_mode) {
      case word_sensibility::BAN:
        // call ban in core
        break;
      case word_sensibility::REMOVE:
        // call delete in core
        break;
      case word_sensibility::CENSOR:
        // filter string
      case word_sensibility::NOTHING:
      default:
        return 0;
    }
    return -1;
  }
  return 0;
}

filter::~filter()
{
}
}
