// #include <stdio.h>

// int main()
// {
//    while (1) { 
//       printf("Hello, World; from user!\n");
//    }
//    return 0;
// }

// #include "hello_world.h"

#include <sys/syscall.h>
#include <unistd.h>

int main()
{
   while (1) {
      syscall(292);
   }
   return 0;
}