// some header
//
//

#include "discord_filter.hpp"

namespace rocord {
filter::filter(name_sensibility name_s, word_sensibility word_s, core& dcore) : name_s(name_s), word_s(word_s), dcore(dcore)
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

void filter::check_name(const std::string &name)
{
  if (true) {
    switch(name_s) {
      case name_sensibility::BAN:
        break;
      case name_sensibility::RENAME:
        break;
      case name_sensibility::NOTHING:
      default:
        return;
    }
  }
}

void filter::check_string(const std::string &str)
{
  if (true) {
    switch(word_s) {
      case word_sensibility::BAN:
        break;
      case word_sensibility::CENSOR:
        break;
      case word_sensibility::REMOVE:
        break;
      case word_sensibility::NOTHING:
      default:
        return;
    }
  }
}

filter::~filter()
{
}
}
