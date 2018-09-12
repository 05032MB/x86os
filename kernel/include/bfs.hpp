#ifndef _BFS_H
#define _BFS_H

#include <types.hpp>

struct bfs_superblock{
	dword magic;
	dword start;
	dword size;
	dword sanity[4];

}__attribute__((packed));

extern bfs_superblock BFSsuperblock;

struct bfs_inode{
	dword inumber;
	dword block_first;
	dword block_last;
	dword eof_offset;
	dword attributes;
	dword file_mode;
	dword uid;
	dword gid;
	dword nlinks;
	dword atime;
	dword mtime;
	dword ctime;
	dword spare[4];

}__attribute__((packed));

struct bfs_block{
	word inode;
	byte name[14];

}__attribute__((packed));

#endif