#include <stdio.h>
#include <string.h>
#include <kfs.h>

int main(int argc, char** argv)
{
  int i;
#ifdef _DEBUG
  printf("DEBUG ENABLED\n");
#endif
  if(argc < 3)
  {
    printf("Usage: %s <archive.kfs>\nOptions:\n -u Unpack\n -p <files> Pack\n", argv[0]);
    return 0;
  }
  
  if(strcmp(argv[2], "-u") == 0)
  {
    struct kFile *kfile = readfile(argv[1]);
    if (kfile != NULL) {
      unpack(kfile);
    }
    return 0;
  }

  if(strcmp(argv[2], "-p") == 0) 
  {
    for (i = 3; i < argc; i++)
    {
      pack(argv[1], argv[i]);
    }
    return 0;
  }

  printf("Usage: kfs.x <archive.kfs>\nOptions:\n -u Unpack\n -p <files> Pack\n");
  return 1;
}