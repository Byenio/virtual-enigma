#include <gtest/gtest.h>
#include "enigma/reflector.h"
#include "enigma/constants.h"

TEST(ReflectorTest, Initialization)
{
  Reflector reflector;
  ReflectorInit(&reflector, kEnigmaReflector_B_Wiring);

  EXPECT_EQ(ReflectorReflect(&reflector, 0), 24);
}