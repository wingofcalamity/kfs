#ifndef _KFS_HEADER
#define _KFS_HEADER

#define ESCAPE            0x1B
#define FILE_SEPARATOR    0x1C
#define GROUP_SEPARATOR   0x1D
#define RECORD_SEPARATOR  0x1E
#define UNIT_SEPARATOR    0x1F

#define STAGE_HEADER      0x10
#define STAGE_INDEX       0x20
#define STAGE_DATA        0x30
#define STAGE_END         0x30

/* Human usable Archive */
struct kArc {
  int arcCount;
  char *arcName;
  struct kFile* arcFile;
};

struct kFile {
  unsigned int fileSize;
  unsigned char* fileName;
  unsigned char *fileData;
};

typedef struct kFile kFile_t;
typedef struct kArc kArc_t;

#endif