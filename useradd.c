#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#define passwdFile "/passwd"
#define maxLength 100

int readLine(int fd, char *buf, int size){
  int i=0, n=0;
  char c;
  while(read(fd, &c, 1)>0){
    n++;
    if(c!='\n'){
      buf[i] = c;
      i++;
    } else { break; }
  }
  buf[i] = '\0';
  return n;
}

int contains(char *str, char *pattern){
  int i=0; int j=0;
  int sizeStr = strlen(str);
  int sizePattern = strlen(pattern);

  while(i<sizePattern && j<sizeStr && pattern[i]!=str[j]){
    j++;
  }
  if(j == sizeStr){
    return 0;
  }
  while(i<sizePattern && j<sizeStr && pattern[i]==str[j]){
    i++;
    j++;
  }
  if(i == sizePattern){
    return 1;
  } else {
    return 0;
  }
}

int getId(char *line, char id[]){
  int pos=0, readPos=0;
  int block=0;
  while(readPos<40){
    if(line[readPos]!=':' && block==1){
      id[pos] = line[readPos];
      pos++;
    } else if(line[readPos]==':') {
      block++;
    }
    readPos++;
  }
  id[pos]='\0';
  return atoi(id);
}

char *
itoa (int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

int main(int argc, char *argv[])
{
  int n=0, lastId=0;
  if(argc != 3){
    printf(2, "Usage useradd name passowrd\n");
    return 1;
  }
  int fd = open(passwdFile, 0);
  char record[maxLength];
  fd = open(passwdFile, O_RDWR);
  while((n = readLine(fd, record, sizeof(record))) > 0) {
    // printf(1, "\nrecord %s\n", record);
    char id[20];
    // printf(1, "id:%d\n", getId(record, id));
    lastId = getId(record, id);
    if(contains(record, argv[1])){
      printf(2, "user '%s' already exists.\n", argv[1]);
      close(fd);
      exit();
    }
  }
  write(fd, argv[1], strlen(argv[1]));
  char separator = ':';
  char newLine = '\n';
  write(fd, &separator, 1);
  char idBuf[100];
  itoa(lastId+1, idBuf, 10);
  write(fd, idBuf, strlen(idBuf));
  write(fd, &separator, 1);
  write(fd, argv[2], strlen(argv[2]));
  write(fd, &separator, 1);
  write(fd, &newLine, 1);

  // printf(1, "user:%s  passwd:%s  id:%d\n", argv[1], argv[2], lastId+1);
  // printf(1, "\nDONE\n");

  close(fd);
  exit();
}
