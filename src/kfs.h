#ifndef _KFS_HEADER
#define _KFS_HEADER

struct kFile {
  unsigned int fsz;
  unsigned int* fd;
  char fn[32];
};

struct kArchive {
  int fileA;
  struct kFile* kfile;
};

struct kFile* readfile(char*);
void unpack(struct kFile *kfile);
void pack(char* kfile, char* fn);

#endif