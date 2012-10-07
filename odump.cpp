/*
  odump.cpp

  > cl odump.cpp -EHsc -I. -link -LIBPATH:.
*/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>

int odump(FILE *fp)
{
  int count = 0;
  char buf[16];
  char chr[sizeof(buf) + 1];
  while(int s = fread(buf, 1, sizeof(buf), fp)){
    fprintf(stdout, "%08x: ", count);
    for(int i = 0; i < s; i++){
      fprintf(stdout, "%02x ", (unsigned char)buf[i]);
      if((i + 1) == sizeof(buf) / 2) fprintf(stdout, "- ");
      if(buf[i] >= 0x20 && buf[i] < 0x7f) chr[i] = buf[i];
      else chr[i] = '.';
    }
    for(int j = s; j < sizeof(buf); j++){
      fprintf(stdout, "   ");
      if((j + 1) == sizeof(buf) / 2) fprintf(stdout, "- ");
      chr[j] = 0x20;
    }
    chr[sizeof(buf)] = '\0';
    fprintf(stdout, "%.*s\n", sizeof(buf), chr);
    count += s;
  }
  return count;
}

int main(int ac, char **av)
{
  if(ac < 2){
    fprintf(stderr, "Usage: %s filename", av[0]);
    return 1;
  }
  FILE *fp = fopen(av[1], "rb");
  if(!fp){
    fprintf(stderr, "cannot open file: %s", av[1]);
    return 1;
  }
  fprintf(stdout, "total %d bytes", odump(fp));
  fclose(fp);
  return 0;
}