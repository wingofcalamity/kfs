#include <stdio.h>
#include <kfs3.h>

int main() {
  kArchive_t ARC;
  readArchive("test.kfs", &ARC);
  return 0;
}