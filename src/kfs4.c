#include <kfs4.h>
#include <stdio.h>
#include <stdlib.h>

int GLOBAL_LIMIT;
GLOBAL_LIMIT = 0;

/* Escape characters for file */
void putEscaped(FILE* fp, char ch)
{
  switch(ch)
  {
    case FILE_SEPARATOR:
    case GROUP_SEPARATOR:
    case RECORD_SEPARATOR:
    case UNIT_SEPARATOR:
    case 0xFF:
      putc(ESCAPE, fp);
    default:
      putc(ch, fp);
  }
}

/* update read character.
returns 1 when a body character is found
returns 0 when a control character is found*/
int readEscaped(FILE* fp, int* ch)
{
  *ch = getc(fp);
  switch(*ch)
  {
    case EOF:
    case FILE_SEPARATOR:
    case GROUP_SEPARATOR:
    case RECORD_SEPARATOR:
    case UNIT_SEPARATOR:
      return 0;
    case ESCAPE:
      *ch = getc(fp);
    default:
      return 1;
  }
}

/* Maps the archive file to memory */
int openArchive(char* arcName, kArc_t *arc)
{
  FILE *fp;
  int arcI;
  int dataI;
  int readByte;
  int stage;

  stage = STAGE_HEADER;

  arcI = 0; /* Archive Index */
  arc->arcName = arcName;
  arc->arcCount = 0;
  
  readByte = '0';

  fp = fopen(arc->arcName, "rb");

  if(fp == NULL)
  {
    perror("Error opening archive: ");
    return 0;
  }

  while (readByte != EOF) {
    readEscaped(fp, &readByte);
    switch(stage)
    {
      case STAGE_HEADER:
        if (readByte == GROUP_SEPARATOR) stage = STAGE_INDEX;
        break;
      case STAGE_INDEX:
        if (readByte == GROUP_SEPARATOR) stage = STAGE_DATA;
        if (readByte == UNIT_SEPARATOR) arc->arcCount++;
        break;
      case STAGE_DATA:
        if (readByte == GROUP_SEPARATOR) 
        {
          stage = STAGE_END;
        }
        if (readByte == UNIT_SEPARATOR) 
        {
          arcI++;
          dataI = 0;
          break;
        }
        break;
      default:
        perror("Unexpected Stage");
        goto LBL_ERROR;
    }
  }

  printf("\n");
  printf("Archive: %s\nSize: %d\n", arc->arcName, arc->arcCount);


  fclose(fp);
  return 1;
LBL_ERROR:
  fclose(fp);
  return 0;
}

int main() {
  kArc_t kArc;
  if(!openArchive("test.arc", &kArc)) return 1;
  printf("Success!\n");
  return 0;
}