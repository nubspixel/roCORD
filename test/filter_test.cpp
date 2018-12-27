#include "../src/discord_filter.hpp"
#include "core_mock.hpp"
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
    
    members = std::vector<std::unique_ptr<rocord::member>>();
    
    std::vector<uint64_t> empty = std::vector<uint64_t>();

    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(std::unique_ptr<rocord::user>(
			new rocord::user(1234, "BadName", "1337", "", false))
			),
			"GoodName", empty)));

    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(std::unique_ptr<rocord::user>(
			new rocord::user(1234, "GoodName", "1337", "", false))
			), 
			"GoodName", empty)));

    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(std::unique_ptr<rocord::user>(
			new rocord::user(1234, "GoodName", "1337", "", false))
			),
			"BadName", empty))); 
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(std::unique_ptr<rocord::user>(new rocord::user(1234, 
         "BadName", "1337", "", false))
), "BadName", empty))); 
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(std::unique_ptr<rocord::user>(new rocord::user(1234,
        "GoodName", "1337", "", false))
), "", empty)));
    members.push_back(std::unique_ptr<rocord::member>(new rocord::member(
            std::move(std::unique_ptr<rocord::user>(new rocord::user(1234, 
         "BadName", "1337", "", false))
), "", empty)));

    // members = {  good_nick_bad_user,
    //              good_nick_good_user,
    //              bad_nick_good_user,
    //              bad_nick_bad_user,
    //              no_nick_good_user,
    //              no_nick_bad_user
    //           }
    EXPECT_EQ(members.size(), 6);

    /* 
	expect_nick = std::vector<std::string>();
    for (auto it = members.begin(); it != members.end(); ++it)
      expect_nick.push_back((*it)->get_nick());

    EXPECT_EQ(members.size(), expect_nick.size());
	*/
 
    
    rocord::username_sensibility name_s = rocord::username_sensibility::NOTHING;
    rocord::word_sensibility word_s = rocord::word_sensibility::NOTHING;
    rocord::nickname_sensibility nick_s = rocord::nickname_sensibility::NOTHING;

    //std::shared_ptr<rocord::log> logger(new rocord::log());
    auto mapping = 
      std::make_shared<std::vector<std::pair<std::string, std::string>>>();
    //dcore = std::unique_ptr<rocord::core>(new rocord::core("Testing Filter",
    //      "some Token", "some presence", 0, mapping, nullptr, nullptr, logger));
    
    filter_ = std::unique_ptr<rocord::filter>(new rocord::filter(name_s, nick_s,
          word_s, mcore));
 
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
	// Preparing expected calls.
  	MockFunction<void(int check_point_name)> check;
	{
	  InSequence s;
      for (int i = 0; i < members.size(); ++i) {
        EXPECT_CALL(mcore, ban_member(*members[i], "Reason: bad nickname",
            7)).Times(ban_nickname[i]);
	    EXPECT_CALL(mcore, ban_member(*members[i], "Reason: bad username",
			7)).Times(ban_username[i]);
	    EXPECT_CALL(check, Call(i));
	  }
	}

	// Run all tests in sequence
	for (int i = 0; i < members.size(); ++i) {
	  std::string nick = members[i]->get_nick();
      std::cout << "Testing: Nickname=" << ((nick == "") ? "no_nick" : nick) << " Username="
		  << members[i]->get_username() << std::endl;
      filter_->check_name(*members[i]);
	  check.Call(i);
	}
  }

  // Objects declared here can be used by all tests in the test case for
  // Filter Test.
  std::vector<int> ban_nickname = {0, 0, 0, 0, 0, 0};
  std::vector<int> ban_username = {0, 0, 0, 0, 0, 0};

  
  std::unique_ptr<rocord::filter> filter_;
  //std::unique_ptr<rocord::core> dcore;
  rocord::core_mock mcore;
  std::vector<std::unique_ptr<rocord::member>> members;

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

TEST_F(filter_test, check_name_BAN_BAN)
{
  // set the appropriate mode for the test
  // BAN        BAN           Bad user/nickname -> BAN
  filter_->chmod_nickname(rocord::nickname_sensibility::BAN);
  filter_->chmod_username(rocord::username_sensibility::BAN);

  ban_nickname[2] = 1;
  ban_nickname[3] = 1;

  ban_username[0] = 1;
  ban_username[3] = 1;
  ban_username[5] = 1;

  filter_test::check_name_run();
}

TEST_F(filter_test, check_name_2)
{
  // set the appropriate mode for the test
  // BAN        FORCE_NICK    If bad nick -> BAN, if bad user -> enforce nickname 
  filter_->chmod_nickname(rocord::nickname_sensibility::BAN);
  filter_->chmod_username(rocord::username_sensibility::FORCE_NICK);
 
  ban_nickname[2] = 1;
  ban_nickname[3] = 1;

  filter_test::check_name_run();
}

TEST_F(filter_test, check_name_NOTHING_NOTHING)
{
  filter_->chmod_nickname(rocord::nickname_sensibility::NOTHING);
  filter_->chmod_username(rocord::username_sensibility::NOTHING);
 
  filter_test::check_name_run();
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
