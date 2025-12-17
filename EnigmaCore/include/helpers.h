#ifndef ENIGMACORE_HELPERS_H
#define ENIGMACORE_HELPERS_H

namespace helpers
{
  static int Mod26(int x)
  {
    return (x % 26 + 26) % 26;
  }
}

#endif //ENIGMACORE_HELPERS_H