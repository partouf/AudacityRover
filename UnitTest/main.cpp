#include "AudacityTests.h"
#include <cstdlib>

int main(int argc, char **argv) {
   auto ret = DoAudacityTests(argc, argv);

   system("pause");

   return ret;
}
