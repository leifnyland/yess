#include <cstdint>

#ifndef STATUS_H
#define STATUS_H
class Status
{
   public:
      static const int32_t SAOK = 1;
      static const int32_t SADR = 2;
      static const int32_t SINS = 3;
      static const int32_t SHLT = 4;
};
#endif
