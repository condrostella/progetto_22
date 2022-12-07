#pragma once
#include "disk_driver.h"
#include <stdlib.h>
#include "linked_list.h"



/*these are structures stored on disk*/

// this is in the first block of a chain, after the header
typedef struct {
  int directory_block; // first block of the parent directory
  int block_in_disk;   // repeated position of the block on the disk
  char name[64];
  int  size;
  int is_dir;        
} FileControlBlock;

// this is the first physical block of a file
// it has a header
// an FCB storing file infos
// and can contain some data

/******************* stuff on disk BEGIN *******************/



typedef struct {
  FileControlBlock fcb;
  char data[BLOCK_SIZE-sizeof(FileControlBlock)] ;
} FirstFileBlock;

// this is one of the next physical blocks of a file
typedef struct {
  char  data[BLOCK_SIZE];
} FileBlock;

// this is the first physical block of a directory
typedef struct {
  FileControlBlock fcb;
  int num_entries;
  int file_blocks[ (BLOCK_SIZE
		   -sizeof(FileControlBlock)
		    -sizeof(int))/sizeof(int) ];
} FirstDirectoryBlock;

// this is remainder block of a directory
typedef struct {
  int file_blocks[ (BLOCK_SIZE)/sizeof(int) ];
} DirectoryBlock;

/******************* stuff on disk END *******************/


  
typedef struct {
  DiskDriver* disk;
  FirstDirectoryBlock* cwd;
} fat;


// this is a file handle, used to refer to open files
typedef struct {
  ListItem item;
  fat* f;           
  FirstFileBlock* ffb;  // pointer to the first block of the file(read it)
  FirstDirectoryBlock* directory; // pointer to the directory where the file is stored
  int pos_in_file;  // position of the cursor              
} FileHandle;

typedef struct {
  fat* f;                   
  FirstDirectoryBlock* dcb;        // pointer to the first block of the directory(read it)
  FirstDirectoryBlock* directory;  // pointer to the parent directory (null if top level)
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

int fat_remove(DirectoryHandle* d, char* filename);
/*// removes the file in the current directory
// returns -1 on failure 0 on success
// if a directory, it removes recursively all contained files
int SimpleFS_remove(SimpleFS* fs, char* filename);
*/