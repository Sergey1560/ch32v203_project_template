#include <sys/stat.h>

int __attribute__ ((weak)) _read_r (struct _reent *r, int file, char * ptr, int len){
  return -1;
}

int __attribute__ ((weak)) _lseek_r (struct _reent *r, int file, int ptr, int dir){
  
  return 0;
};

int __attribute__ ((weak)) _write_r (struct _reent *r, int file, char * ptr, int len){  
  return len;
}

int __attribute__ ((weak)) _close_r (struct _reent *r, int file){
  return 0;
}

int __attribute__ ((weak)) _fstat_r (struct _reent *r, int file, struct stat * st){
  return 0;
}

int __attribute__ ((weak)) _isatty_r(struct _reent *r, int fd){
  return 1;
}



