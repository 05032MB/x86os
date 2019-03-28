#ifndef _VFS_H_
#define _VFS_H_

#include <types.hpp>

//POSIX-like calls
typedef size_t (*read_type_t)(struct f_node*,addr_t,size_t,byte*);
typedef size_t (*write_type_t)(struct f_node*,addr_t,size_t,byte*);
typedef unsigned (*open_type_t)(struct f_node*);
typedef unsigned (*close_type_t)(struct f_node*);
typedef struct d_node * (*readdir_type_t)(struct f_node*,dword);
typedef struct f_node * (*findindir_type_t)(struct f_node*,char *name);

struct f_callbacks_pack{
	read_type_t read{nullptr};
	write_type_t write{nullptr};
	open_type_t open{nullptr};
    close_type_t close{nullptr};
    readdir_type_t readdir{nullptr};
    findindir_type_t findindir{nullptr};
};

struct pre_f_node{
	char name[64];
	dword inode;
};

enum  f_type {FILE=1, DIRECTORY=2, MOUNTPOINT=4, DEVICE=8};

struct f_node{

	//char name[64]; //file name
	dword oid{0}; //owner id
	dword gid{0}; //group id
	dword mask{0}; //mask of permissions
	dword size{0}; //size of file
	
	dword flags;
	
	dword inode;
	
	f_callbacks_pack callback; //callbacks to low-level drivers
	
	f_node* mptr{nullptr};  //for mounts

}__packed;

struct d_node {
	char name[64];
	dword inode;

}__packed;

extern f_node *first_fs;

size_t fs_read(struct f_node*,addr_t,size_t,byte*);
size_t fs_write(struct f_node*,addr_t,size_t,byte*);
unsigned fs_open(struct f_node*);
unsigned fs_close(struct f_node*);
struct d_node * fs_readdir(struct f_node*,dword);
struct f_node * fs_findindir(struct f_node*,char*);


void init_vfs();

#endif