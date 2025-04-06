#ifndef _KFS_HEADER
#define _KFS_HEADER

#define ESCAPE 0x1B
#define FILE_SEPARATOR 0x1C
#define GROUP_SEPARATOR 0x1D
#define RECORD_SEPARATOR 0x1E
#define UNIT_SEPARATOR 0x1F

/* Internal Archive */



/* Human usable Archive */
struct kArchive {
  int fileA;
  char *fn;
  struct kFile* kfile;
};

struct kFile {
  unsigned int fsz;
  unsigned char* fd;
  char *fn;
};

typedef struct kFile kFile_t;
typedef struct kArchive kArchive_t;


kFile_t* readfile(char*);
void readArchive(char*, kArchive_t*);

void unpack(struct kFile *kfile);
void pack(char* kfile, char* fn);

#endif