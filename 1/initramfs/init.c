#include "hello_world.h"
#include <sys/syscall.h>

int main()
{
   while (1) {
      hello_world();
      syscall(404);
   }
   return 0;
}