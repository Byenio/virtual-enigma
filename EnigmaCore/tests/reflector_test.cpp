#include <gtest/gtest.h>
#include "../include/enigma/reflector.h"
#include "../include/enigma/constants.h"

namespace enigma
{
  TEST(ReflectorTest, Initialization)
  {
    const Reflector reflector(kReflector_B_Wiring);

    EXPECT_EQ(reflector.Reflect(0), 24);
  }

  TEST(ReflectorTest, IsReciprocal)
  {
    const Reflector reflector(kReflector_B_Wiring);

    const int a_to_y = reflector.Reflect(0);
    const int y_to_a = reflector.Reflect(a_to_y);

    EXPECT_EQ(y_to_a, 0);
  }
}