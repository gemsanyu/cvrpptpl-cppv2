#include <gtest/gtest.h>

TEST(SanityCheck, BasicMath) {
  EXPECT_EQ(2 + 2, 4);
}

TEST(SanityCheck2, FailCase) {
  EXPECT_NE(3 * 3, 10);
}
