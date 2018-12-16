//Test header
#ifndef discord_member_hpp
#define discord_member_hpp

#include <string>
#include <cstdint>
#include <memory>
#include <vector>

#include "discord_user.hpp"

namespace rocord {
class member
{
public:
  member(std::unique_ptr<user> usr, std::string nick, 
      std::vector<uint64_t> roles);
  virtual ~member();

  const std::string& get_username() const;
  const std::string& get_discriminator() const;
  const std::string& get_fullname() const;
  const std::string& get_nick() const;
  bool has_role(uint64_t role_id);

private:
  std::unique_ptr<user> usr;
  std::string nick;
  std::vector<uint64_t> roles;
};
}
#endif /* discord_member_hpp*/

