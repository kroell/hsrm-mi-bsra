#include <stdio.h>
static int programm[] = { 1, 0, 1, 1, 0, 2, 4, 1, 2, 3, 2, 3, 0, 9, 3, 10, 3, 14, 0, 0 };

 main(int argc, char *argv[])
{
  FILE *datei;
  if((argc == 2) && (datei = fopen(argv[1], "w")))
  {
    fwrite(programm, sizeof(programm), 1, datei);
  }
}
