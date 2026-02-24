#include <gtest/gtest.h>
#include "enigma/plugboard.h"

TEST(PlugboardTest, DefaultIsIdentity)
{
  Plugboard pb;
  PlugboardInit(&pb);
  EXPECT_EQ(PlugboardForward(&pb, 0), 0);
  EXPECT_EQ(PlugboardForward(&pb, 25), 25);
}

TEST(PlugboardTest, SwapsLetters)
{
  Plugboard pb;
  PlugboardInit(&pb);
  PlugboardAddCable(&pb, 'A', 'Z');

  EXPECT_EQ(PlugboardForward(&pb, 0), 25);
  EXPECT_EQ(PlugboardForward(&pb, 25), 0);
  EXPECT_EQ(PlugboardForward(&pb, 1), 1);
}

/*
TEST(PlugboardTest, CannotOverwritExistingConnection)
{
    Plugboard pb;
    PlugboardInit(&pb);
    PlugboardAddCable(&pb, 'A', 'B');

    // This depends on if your C code has assert() calls
    ASSERT_DEATH(PlugboardAddCable(&pb, 'A', 'C'), "");
}
*/