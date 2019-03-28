#ifndef _BFS_H
#define _BFS_H

#include <types.hpp>

//simple filesystem
//used to create ramdisk-like structure on system boot
//assumes contiguous memory allocation


struct bfs_superblock{
	dword magic;
	dword start;
	dword size;
	dword inodesmax; //max number of inodes
	dword sanity[4];

}__packed;

struct bfs_inode{
	//byte name[64];
	dword inode;
	dword block_pointer; //pointer to data block of the file
	dword eof_offset; //size
	dword attributes; //type of node
	dword file_mode; //mask of permissions
	dword oid;
	dword gid;
	dword nlinks;
	dword ctime;
	
	//dword nextnode;

}__packed;

struct bfs_dirblock{ 
	word inode;
	byte name[64];

}__packed;

using initrd_virtual_node = struct f_node;

initrd_virtual_node * init_initrd(addr_t loc);

//extern initrd_virtual_node * initrd_nodes;

#endif