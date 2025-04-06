#include <kfs3.h>
#include <stdio.h>
#include <stdlib.h>

int readFilename(FILE *fp, int *rc, struct kFile *kfs, int *kfsi, int *i)
{
  while(*rc != EOF)
  {
    *rc = getc(fp);
    if (*rc == EOF)
    {
      return 0;
    }
    if (*rc == UNIT_SEPARATOR) {
      break;
    }
    kfs[*kfsi].fn[*i] = *rc;
    (*i)++;
  }
  *i = 0;
  return 1;
}

int readFilesize(FILE *fp, int *rc, struct kFile *kfs, int *kfsi, int *i)
{
  while(*rc != EOF)
  {
    *rc = getc(fp);
    if (*rc == EOF)
    {
      return 0;
    }
    if (*rc == UNIT_SEPARATOR)
    {
      break;
    }
    kfs[*kfsi].fsz |= (*rc << (*i*8));
    (*i)++;
  }
  *i = 0;
  return 1;
}

int readFiledata(FILE *fp, int *rc, struct kFile *kfs, int *kfsi, int *i)
{
  while(*rc != EOF)
  {
    *rc = getc(fp);
    if (*rc == EOF)
    {
      return 0;
    }
    if (*rc == ESCAPE)
    {
      *rc = getc(fp);
    }
    else if (*rc == RECORD_SEPARATOR)
    {
      break;
    };
    kfs[*kfsi].fd[*i] = *rc;
    (*i)++;
  }
  *i = 0;
  return 1;
}

int readSignature(FILE *fp)
{

}

int readNthByte(long byte, FILE *fp)
{

}

int getFilesize(FILE *fp)
{
  int fsz;
  fseek(fp, 0, SEEK_END);
  fsz = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return fsz;
}

void readArchive(char* fn, kArchive_t *kfa)
{
  FILE *fp;
  int fsz;

  /* INIT STRUCT */
  kfa->fileA = 0;
  kfa->fn = fn;

  printf("reading %s\n", kfa->fn);
  fp = fopen(kfa->fn, "rb");
  if (fp == NULL) {
    perror("Error opening file");
    return NULL;
  }
  fsz = getFilesize(fp);

  printf("%d", fsz);
  fclose(fp);
}

kFile_t* readFiles(kArchive_t *kfa)
{
  FILE *fp;
  int rc, i, kfsi;

  kfa->kfile = malloc(kfa->fileA * sizeof(kFile_t));

  fp = fopen(kfa->fn, "rb");
  if (fp == NULL) {
    perror("Error opening file");
    return NULL;
  }

  while(rc != EOF)
  {

  }

  fclose(fp);
}

kFile_t* readfile(char* fn)
{
  FILE *fp;
  static struct kFile kfs[100];
  int rc;
  int i, kfsi;

  fp = fopen(fn, "rb");
  if (fp == NULL) {
    perror("Error opening file");
    return NULL;
  }

  i = 0;
  rc = '0';
  kfsi = 0;

  while(rc != EOF) {

    kfs[kfsi].fsz = 0;

#ifdef _DEBUG
    printf("READING FILENAME: ");
#endif

    if (!readFilename(fp, &rc, kfs, &kfsi, &i)) break;

#ifdef _DEBUG
    printf("%s\n", kfs[kfsi].fn);
    printf("READING SIZE: ");
#endif

    if (!readFilesize(fp, &rc, kfs, &kfsi, &i)) break;

#ifdef _DEBUG
    printf("%d\n", kfs[kfsi].fsz);
    printf("ALLOCATING MEMORY\n");
#endif

    kfs[kfsi].fd = malloc(sizeof(char) * kfs[kfsi].fsz);
    if (kfs[kfsi].fd == NULL) {
      perror("Error allocating memory");
      fclose(fp);
      return NULL;
    }

#ifdef _DEBUG
    printf("READING FILEDATA: ");
#endif

    if (!readFiledata(fp, &rc, kfs, &kfsi, &i)) break;

#ifdef _DEBUG
    printf("%s\n", kfs[kfsi].fd);
    printf("FILE COMPLETE. LAST BYTE: %x\n", rc);
#endif

    kfsi++;

  }

  fclose(fp);
  return kfs;
}

void unpack(struct kFile *kfile)
{
  int i, fcsz;
  i = 0;
  FILE *fp;
  while(kfile[i].fsz)
  {
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

void pack(char* kfile, char* fn)
{
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
  putc(fsz & 0xFF, kfp);
  putc((fsz >> 8) & 0xFF, kfp);
  putc((fsz >> 16) & 0xFF, kfp);
  putc((fsz >> 24) & 0xFF, kfp);

  putc(0x1F, kfp);

  while ((ch = getc(fp)) != EOF)
  {
    if(ch == 0x1B || ch == 0x1E || ch == 0x1F || ch == 0xFF) 
    {
      putc(0x1B, kfp);
    }
    putc(ch, kfp);
  }
  putc(0x1E, kfp);

  fclose(fp);
  fclose(kfp);
}