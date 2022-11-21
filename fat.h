#pragma once
#include "disk_driver.h"
#include <stdlib.h>
#include "linked_list.h"





typedef struct {
  int directory_block;
  int block_in_disk;   
  char name[64];
  int  size;
  int is_dir;        
} FileControlBlock;



typedef struct {
  FileControlBlock fcb;
  char data[BLOCK_SIZE-sizeof(FileControlBlock)] ;
} FirstFileBlock;


typedef struct {
  char  data[BLOCK_SIZE];
} FileBlock;


typedef struct {
  FileControlBlock fcb;
  int num_entries;
  int file_blocks[ (BLOCK_SIZE
		   -sizeof(FileControlBlock)
		    -sizeof(int))/sizeof(int) ];
} FirstDirectoryBlock;

typedef struct {
  int file_blocks[ (BLOCK_SIZE)/sizeof(int) ];
} DirectoryBlock;



  
typedef struct {
  DiskDriver* disk;
  FirstDirectoryBlock* cwd;
} fat;

typedef struct {
  ListItem item;
  fat* f;                   
  FirstFileBlock* ffb;             
  FirstDirectoryBlock* directory;  
  int pos_in_file;                 
} FileHandle;

typedef struct {
  fat* f;                   
  FirstDirectoryBlock* dcb;      
  FirstDirectoryBlock* directory;
} DirectoryHandle;


DirectoryHandle* fat_init(fat* fs, DiskDriver* disk);


void fat_format(fat* fs);


FileHandle* fat_createFile(DirectoryHandle* d, const char* filename);

int fat_listDir(char** names, DirectoryHandle* d);




FileHandle* fat_openFile(DirectoryHandle* d, const char* filename);



int fat_close(FileHandle* f);


int fat_write(FileHandle* f, void* data, int size);


int fat_read(FileHandle* f, void* data, int size);


int fat_seek(FileHandle* f, int pos);


int fat_changeDir(DirectoryHandle* d, char* dirname);



int fat_mkDir(DirectoryHandle* d, char* dirname);