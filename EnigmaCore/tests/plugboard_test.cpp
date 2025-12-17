#include <gtest/gtest.h>
#include "enigma/plugboard.h"

namespace enigma
{

  TEST(PlugboardTest, DefaultIsIdentity)
  {
    const Plugboard pb;
    EXPECT_EQ(pb.Forward(0), 0);
    EXPECT_EQ(pb.Forward(25), 25);
  }

  TEST(PlugboardTest, SwapsLetters)
  {
    Plugboard pb;
    pb.AddCable('A', 'Z');

    EXPECT_EQ(pb.Forward(0), 25);
    EXPECT_EQ(pb.Forward(25), 0);

    EXPECT_EQ(pb.Forward(1), 1);
  }

  TEST(PlugboardTest, ResetClearsConnections)
  {
    Plugboard pb;
    pb.AddCable('A', 'Z');
    pb.Reset();

    EXPECT_EQ(pb.Forward(0), 0);
    EXPECT_EQ(pb.Forward(25), 25);
  }

  TEST(PlugboardTest, CannotOverwritExistingConnection)
  {
    Plugboard pb;
    pb.AddCable('A', 'B');

    ASSERT_DEATH(pb.AddCable('A', 'C'), "Input character is already connected");
  }

}