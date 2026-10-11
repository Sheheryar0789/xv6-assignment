#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2026 };
    memdump("ii", (char*) a, sizeof(a));

    printf("Example 2:\n");
    memdump("S", "a string", 9);

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *) &s, sizeof(s));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char*) &example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char*) &example, sizeof(example));
  } else if(argc == 2){
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

static void
notenough(void)
{
  printf("memdump: not enough data\n");
  exit(1);
}

void
memdump(char *fmt, char *data, int len)
{
  char *end = data + len;

  for(; *fmt != '\0'; fmt++){
    switch (*fmt) {
      case 'i': {
        int v;
        if(end - data < 4) notenough();
        memmove(&v, data, 4);
        printf("%d\n", v);
        data += 4;
        break;
      }
      case 'h': {
        short v;
        if(end - data < 2) notenough();
        memmove(&v, data, 2);
        printf("%d\n", v);
        data += 2;
        break;
      }
      case 'c': {
        if(end - data < 1) notenough();
        printf("%c\n", *data);
        data += 1;
        break;
      }
      case 'p': {
        uint64 v;
        if(end - data < 8) notenough();
        memmove(&v, data, 8);
        printf("%lx\n", v);
        data += 8;
        break;
      }
      case 's': {
        char *v;
        if(end - data < 8) notenough();
        memmove(&v, data, 8);
        printf("%s\n", v);
        data += 8;
        break;
      }
      case 'S': {
        while(1){
          if(data >= end) notenough();
          if(*data == '\0')
            break;
          printf("%c", *data);
          data++;
        }
        printf("\n");
        data++;   // skip the terminating NUL
        break;
      }
    }
  }
}