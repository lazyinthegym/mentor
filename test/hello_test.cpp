#include <gtest/gtest.h>

TEST(HelloTest, BasicAssertions) {
// Expect two strings not to be equal.
EXPECT_STRNE("hello", "Ahmad");
}