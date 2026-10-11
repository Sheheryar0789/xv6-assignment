#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
is_secret_char(char c)
{
  return (c >= '0' && c <= '9') ||
         (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z');
}

int
matches(char *mem, int i, int n, char *marker, int mlen)
{
  if(i + mlen > n)
    return 0;
  for(int k = 0; k < mlen; k++)
    if(mem[i + k] != marker[k])
      return 0;
  return 1;
}

int
main(void)
{
  int n = 64 * 4096;
  char *mem = sbrk(n);
  if(mem == (char*)-1){
    printf("sbrk failed\n");
    exit(1);
  }

  char *marker = "Here it is: ";
  int mlen = 12; // strlen("Here it is: ")

  for(int i = 0; i < n; i++){
    if(matches(mem, i, n, marker, mlen)){
      int start = i + mlen;
      int j = start;
      while(j < n && is_secret_char(mem[j]))
        j++;
      if(j > start){
        write(1, &mem[start], j - start);
        write(1, "\n", 1);
      }
    }
  }

  exit(0);
}