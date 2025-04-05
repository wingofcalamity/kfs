#include "kfs.h"
#include <stdio.h>
#include <stdlib.h>

struct kFile* readfile(char* fn)
{
  FILE *fp;
  static struct kFile kfs[100];
  char rc;
  int i, kfsi;
  unsigned char byte1, byte2, byte3, byte4;

  fp = fopen(fn, "rb");
  if (fp == NULL) {
    perror("Error opening file");
    return NULL;
  }

  i = 0;
  kfsi = 0;

  while((rc = getc(fp)) != EOF) {
    i = 0;
    while(rc != 0x1F && rc != EOF) {
      kfs[kfsi].fn[i] = rc;
      i++;
      rc = getc(fp);
      if(rc == 0x1B) {
        rc = getc(fp);
        kfs[kfsi].fd[i] = rc;
        i++;
        rc = getc(fp);

      }
    }
    kfs[kfsi].fn[i] = '\0';

    /* GET FILESIZE */
    byte1 = getc(fp);
    byte2 = getc(fp);
    byte3 = getc(fp);
    byte4 = getc(fp);
    kfs[kfsi].fsz = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
    rc = getc(fp); 
    /* END FILESIZE */


    kfs[kfsi].fd = malloc(sizeof(char) * kfs[kfsi].fsz);
    if (kfs[kfsi].fd == NULL) {
      perror("Error allocating memory");
      fclose(fp);
      return NULL;
    }

    i = 0;
    rc = getc(fp);
    while(i < kfs[kfsi].fsz) {
      kfs[kfsi].fd[i] = rc;
      i++;
      rc = getc(fp);
      if(rc == 0x1B) {
        rc = getc(fp);
        kfs[kfsi].fd[i] = rc;
        i++;
        rc = getc(fp);
        continue;
      }
    }

#ifdef _DEBUG
    printf("FILENAME: %s\n", kfs[kfsi].fn);
    printf("FILESIZE: %d\n", kfs[kfsi].fsz);
#endif  

    kfsi++;
  }

  fclose(fp);

  return kfs;
}

void unpack(struct kFile *kfile) {
  int i, fcsz;
  i = 0;
  FILE *fp;
  while(kfile[i].fsz) {
#ifdef _DEBUG
    printf("Writing: %s\n", kfile[i].fn);
#endif
    fp = fopen(kfile[i].fn,"wb");
    for (fcsz = 0; fcsz < kfile[i].fsz; fcsz++)
    {
      putc(kfile[i].fd[fcsz], fp);
    }
    free(kfile[i].fd);
    fclose(fp);
    i++;
  }
}

void pack(char* kfile, char* fn) {
  FILE *kfp, *fp;
  int ch;
  int fsz;

#ifdef _DEBUG
  printf("Reading: %s\n", fn);
#endif  

  kfp = fopen(kfile, "ab");
  fp = fopen(fn, "rb");
  
  fprintf(kfp, "%s", fn);
  putc(0x1F, kfp);

  fseek(fp, 0, SEEK_END);
  fsz = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  /* READ FILESIZE (4 BYTES)*/
  putc((fsz >> 24) & 0xFF, kfp);
  putc((fsz >> 16) & 0xFF, kfp);
  putc((fsz >> 8) & 0xFF, kfp);
  putc(fsz & 0xFF, kfp); 

  putc(0x1F, kfp);
  
  while ((ch = getc(fp)) != EOF) {
    if(ch == 0x1B || ch == 0x1E || ch == 0x1F || ch == 0xFF) {
      putc(0x1B, kfp);
    }
    putc(ch, kfp);
  }
  putc(0x1E, kfp);
  
  fclose(fp);
  fclose(kfp);
}