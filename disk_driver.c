#include "disk_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>  
#include <sys/types.h>  
#include <sys/mman.h> 
#include <errno.h>
#include <unistd.h> 
#include <fcntl.h>  
#include <ctype.h> 
#include <string.h>  
#include <errno.h>  
#include <signal.h> 




void DiskDriver_init(DiskDriver* disk, const char* filename, int num_blocks){

	int file;
	file = open (filename, O_CREAT |O_RDWR | O_SYNC,0666);
	if (file == -1) {
		perror("errore nell'apertura del file");
	} 
	
	//dopo aver controllato che la open è andata a buon fine inizializzo il file descriptor del disco con il file descriptor del file appena aperto
	disk->fd = file;
	//impostiamo la dimensione del disco
	int ret=ftruncate(disk->fd,sizeof(DiskHeader)+num_blocks*sizeof(int)+num_blocks*BLOCK_SIZE);
	if(ret==-1){
	    perror("errore ftruncate");
	}

	//mappiamo l'header in memoria e inizializziamo i campi del DiskHeader
	disk->header = mmap (0, sizeof(DiskHeader), PROT_READ | PROT_WRITE, MAP_SHARED, disk->fd, 0); 
	if(disk->header==MAP_FAILED){
	    perror("errore nella mmap dell'header \n");
	}
	disk->header->num_blocks=num_blocks;
	disk->header->free_blocks=num_blocks;
	disk->header->first_free_block=0;
	printf("%d\n",disk->header->free_blocks);

	//mappiamo la fat in memoria e inizializziamo i blocchi della fat a -1
	disk->fat = mmap(0,num_blocks*sizeof(int),PROT_READ | PROT_WRITE,MAP_SHARED,disk->fd,sizeof(DiskHeader));
	if(disk->fat==MAP_FAILED){
	    perror("errore nella mmap della fat \n");
	}
	for(int i=0;i<num_blocks;i++){
	    disk->fat[i]=-1;
    }
   
}

int DiskDriver_writeBlock(DiskDriver* disk, void* src, int block_num){

	// Se il numero del blocco da scrivere è maggiore del numero di blocchi esistenti, restituisco un errore
	if(block_num > disk->header->num_blocks) return -1;

    int fd=disk->fd;
    int ret=lseek(fd,sizeof(DiskHeader)+disk->header->num_blocks*sizeof(int)+BLOCK_SIZE*block_num,SEEK_SET);
    if(ret<0){
		perror("errore nella seek");
		return -1;
    }
    ret=write(fd,src,BLOCK_SIZE);
    if(ret==-1){
		perror("errore nella write");
		return -1;
    }

    disk->header->free_blocks--;
	disk->header->first_free_block = DiskDriver_getFreeBlock(disk,0);
    return ret;

}

int DiskDriver_getFreeBlock(DiskDriver* disk, int start){

	if(disk->header->free_blocks==0){
		printf("free_blocks==0");
		return -1;
    }

	//controllo che l'indice start sia valido
    if(start<0) {
		printf("Start index %d must be >=0",start);
		return -1;
    }else if ( start>disk->header->num_blocks){
		printf("Start index %d must be <=%d ",start,disk->header->num_blocks);
		return -1;
	}

	//cerco il primo blocco libero a partire da start
    for(int i=start;i<disk->header->num_blocks;i++) {
        if(disk->fat[i]==-1) {
			return i;
        }
    }

    return disk->header->first_free_block;
}