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
    list = {"test1",
            "test2",
            "another_word",
            "with space",
            "multi space word",
            "öäü",
            "SpecialChars?!()&",
            "Nothing",
            "a"};
    
    rocord::name_sensibility name_s = rocord::name_sensibility::NOTHING;
    rocord::word_sensibility word_s = rocord::word_sensibility::NOTHING;
    std::shared_ptr<rocord::log> logger(new rocord::log());
    auto mapping = std::make_shared<std::vector<std::pair<std::string, std::string>>>();
    dcore = std::unique_ptr<rocord::core>(new rocord::core("Testing Filter",
          "some Token", "some presence", 0, mapping, nullptr, nullptr, logger));

    filter_ = std::unique_ptr<rocord::filter>(new rocord::filter(name_s, word_s,
        *dcore));
 
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

  /*
   * Tests mode if filter does not apply
   */
  void test_nothing() {
    std::string not_on_lst = "not";
    std::string tmp;
    int res;

    tmp = not_on_lst;
    res = filter_->check_name(not_on_lst);
    // nothing should be changed and execution should go on!
    EXPECT_EQ(res, 0);
    EXPECT_EQ(tmp, not_on_lst);
  }

  // Objects declared here can be used by all tests in the test case for
  // Filter Test.
  std::unique_ptr<rocord::filter> filter_;
  std::unique_ptr<rocord::core> dcore;
  std::vector<std::string> list;
};

TEST_F(filter_test, constructor)
{
  EXPECT_TRUE(filter_ != nullptr);
}


// TODO check if EXPECT_EQ(string1, string2) tests content not object.
TEST_F(filter_test, check_name_ban)
{
  std::string tmp;
  int retval;
  
  // set the appropriate mode for the test
  filter_->chmod_name(rocord::name_sensibility::BAN);

  for (auto it = list.begin(); it != list.end(); ++it) {
    tmp = *it;
    retval = filter_->check_name(*it);
    //check if there was a ban and execution stops!
    EXPECT_EQ(retval, -1);
    EXPECT_EQ(tmp, *it);
  }
  test_nothing();
}

TEST_F(filter_test, check_name_rename)
{
  std::string tmp;
  int res;

  // set the appropriate mode for the test
  filter_->chmod_name(rocord::name_sensibility::RENAME);

  for (auto it = list.begin(); it != list.end(); ++it) {
    tmp = *it;
    res = filter_->check_name(*it);
    //check if there was a rename and execution goes on!
    EXPECT_EQ(res, 0);
    EXPECT_NE(tmp, *it);
  }
  test_nothing();  
}

TEST_F(filter_test, check_name_nothing)
{
  std::string tmp;
  int res;

  // set the appropriate mode for the test
  filter_->chmod_name(rocord::name_sensibility::NOTHING);

  for (auto it = list.begin(); it != list.end(); ++it) {
    tmp = *it;
    res = filter_->check_name(*it);
    // nothing should be changed and execution should go on!
    EXPECT_EQ(res, 0);
    EXPECT_EQ(tmp, *it);
  }
  test_nothing();
}

/*TEST_F(filter_test, check_word_ban)
{

}

TEST_F(filter_test, check_word_censor)
{
  
}

TEST_F(filter_test, check_word_remove)
{

}

TEST_F(filter_test, check_word_nothing)
{

}*/
}
}
