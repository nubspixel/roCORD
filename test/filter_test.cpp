#include "../src/discord_filter.hpp"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

namespace testing {
namespace gmock_matchers_test {

class filter_test : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  filter_test()
  {
    // You can do set-up work for each test here.
    
    auto bad_user = std::unique_ptr<rocord::user>(new rocord::user(1234, 
         "BadName", "1337", "", false));
    auto good_user = std::unique_ptr<rocord::user>(new rocord::user(1234,
        "GoodName", "1337", "", false));
    members = std::vector<std::unique_ptr<rocord::member>>();
    
    std::vector<uint64_t> empty = std::vector<uint64_t>();

    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(bad_user), "GoodName", empty)));
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(good_user), "GoodName", empty)));
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(good_user), "BadName", empty))); 
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(bad_user), "BadName", empty))); 
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(good_user), "", empty)));
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(bad_user), "", empty)));

    
    // members = {  good_nick_bad_user,
    //              good_nick_good_user,
    //              bad_nick_good_user,
    //              bad_nick_bad_user,
    //              no_nick_good_user,
    //              no_nick_bad_user
    //           }
    EXPECT_EQ(members.size(), 6);

    expect_nick = std::vector<std::string>();
    for (auto it = members.begin(); it != members.end(); ++it)
      expect_nick.push_back((*it)->get_nick());

    EXPECT_EQ(members.size(), expect_nick.size());
 
    
    rocord::username_sensibility name_s = rocord::username_sensibility::NOTHING;
    rocord::word_sensibility word_s = rocord::word_sensibility::NOTHING;
    rocord::nickname_sensibility nick_s = rocord::nickname_sensibility::NOTHING;

    std::shared_ptr<rocord::log> logger(new rocord::log());
    auto mapping = 
      std::make_shared<std::vector<std::pair<std::string, std::string>>>();
    dcore = std::unique_ptr<rocord::core>(new rocord::core("Testing Filter",
          "some Token", "some presence", 0, mapping, nullptr, nullptr, logger));

    filter_ = std::unique_ptr<rocord::filter>(new rocord::filter(name_s, nick_s,
          word_s, *dcore));
 
  }

  virtual ~filter_test()
  {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp()
  {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown()
  {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  void check_name_run() {
    int ret;
    for (int i = 0; i < members.size(); i++) {
      ret = filter_->check_name(*members[i]);
      EXPECT_EQ(ret, expect_ret[i]) << "i was " << i;
      EXPECT_EQ(members[i]->get_nick(), expect_nick[i]); 
    }
  }

  // Objects declared here can be used by all tests in the test case for
  // Filter Test.
  std::unique_ptr<rocord::filter> filter_;
  std::unique_ptr<rocord::core> dcore;
  std::vector<std::unique_ptr<rocord::member>> members;
  std::vector<int> expect_ret;
  std::vector<std::string> expect_nick;

  //std::vector<rocord::member> list;
};

TEST_F(filter_test, constructor)
{
  EXPECT_TRUE(filter_ != nullptr);
}

/* check_name tests
 *
 * Nickname   Username      Result
 * BAN        BAN           Bad user/nickname -> BAN
 * BAN        FORCE_NICK    If bad nick -> BAN, if bad user -> enforce nickname 
 * BAN        NOTHING       If bad nick -> BAN
 * DELETE     BAN           If bad nick -> DELETE, if bad user -> BAN
 * DELETE     FORCE_NICK    If bad nick/user -> DELETE and set new generic Nickname else if only bad nick -> DELETE nick
 * DELETE     NOTHING       If bad nick -> DELETE
 * RENAME     BAN           If bad nick -> RENAME, if bad user -> BAN 
 * RENAME     FORCE_NICK    If bad nick -> RENAME, if bad user -> enforce nick
 * RENAME     NOTHING       If bad nick -> RENAME
 * NOTHING    BAN           If bad user -> BAN
 * NOTHING    FORCE_NICK    If bad user -> FORCE_NICK
 * NOTHING    NOTHING       NOTHING
 */

// TODO check if EXPECT_EQ(string1, string2) tests content not object.

TEST_F(filter_test, check_name_1)
{
  // set the appropriate mode for the test
  // BAN        BAN           Bad user/nickname -> BAN
  filter_->chmod_nickname(rocord::nickname_sensibility::BAN);
  filter_->chmod_username(rocord::username_sensibility::BAN);
  
  expect_ret = { -1, 0, -1, -1, 0, -1 };
  // nothing to do for expect_nick since we want the nicknames to be unchanged!

  filter_test::check_name_run();
}

TEST_F(filter_test, check_name_2)
{
  // set the appropriate mode for the test
  // BAN        FORCE_NICK    If bad nick -> BAN, if bad user -> enforce nickname 
  filter_->chmod_nickname(rocord::nickname_sensibility::BAN);
  filter_->chmod_username(rocord::username_sensibility::FORCE_NICK);
  
  expect_ret = { 0, 0, -1, -1, 0, 0};
  // no_nick_bad_user has to get a generic nickname
  expect_nick[5] = "Removed Nickname";

  filter_test::check_name_run();
}

TEST_F(filter_test, check_name_3)
{
}

TEST_F(filter_test, check_name_4)
{

}
/*TEST_F(filter_test, check_word_ban)
{
  // mock call core::ban
}

TEST_F(filter_test, check_word_censor)
{
  
}

TEST_F(filter_test, check_word_remove)
{
  // mock call core::delete_message
}

TEST_F(filter_test, check_word_nothing)
{

}*/
}
}
