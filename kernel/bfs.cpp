#include <bfs.hpp>

#include <vfs.hpp>
#include <kmemory.hpp>

#include <string.h>

bfs_superblock *initrd_superblock;
bfs_inode * initrd_inodes;
initrd_virtual_node * initrd_root;
//initrd_virtual_node * initrd_etc;
initrd_virtual_node * initrd_nodes;
unsigned inodes_amount = 0;
addr_t location;

d_node retnode;
	#include <vga.hpp>
static d_node *initrd_readdir(initrd_virtual_node *node, dword index)
{
    if(!(node->flags == f_type::DIRECTORY))return nullptr;

	unsigned i;
	for(i=0; node[i].inode != initrd_nodes[i].inode && i < inodes_amount; i++ );  //wyszukujemy inoda
	if(i == inodes_amount)return nullptr;

	if(sizeof(bfs_dirblock)*index >= initrd_inodes[i].eof_offset)return nullptr;//je�li chce si� odwo�a� do nieistniej�cego obiektu
	
	bfs_dirblock *db = reinterpret_cast<bfs_dirblock *> ( location + initrd_inodes[i].block_pointer + sizeof(bfs_dirblock)*index  ); //pobieramy wskaznik na pocz. zaw. katalogu, i przemieszczamy sie pod indeks
	
	//d_node retnode;
	
    strcpy(retnode.name, reinterpret_cast<const char*>(db->name) );
    retnode.inode = db->inode;

    return &retnode;
}

initrd_virtual_node * initrd_findindir(initrd_virtual_node* node,char *name)
{	
	if(!(node->flags == f_type::DIRECTORY))return nullptr;

	unsigned i;
	for(i=0; node[i].inode != initrd_nodes[i].inode && i < inodes_amount; i++ );  //wyszukujemy inoda
	if(i == inodes_amount)return nullptr;

	for(unsigned f = 0; sizeof(bfs_dirblock)*f < initrd_inodes[i].eof_offset; f++)
	{
		bfs_dirblock *db = reinterpret_cast<bfs_dirblock *> ( location + initrd_inodes[i].block_pointer + sizeof(bfs_dirblock)*f) ;

		if(strcmp(name, reinterpret_cast<const char*>(db->name) ) == 0)
		{//term_print_dec(db->inode);
			unsigned z;
			for(z=0; db->inode != initrd_nodes[z].inode && z < inodes_amount; z++ );
			if(initrd_nodes[z].inode == db->inode)return &initrd_nodes[z];
		}
	}
	return nullptr;

}

initrd_virtual_node * init_initrd(addr_t loc)
{
	location = loc;
	initrd_superblock = reinterpret_cast<bfs_superblock *>(loc);
	
	initrd_inodes = reinterpret_cast<bfs_inode *>(loc + sizeof(bfs_superblock) );
	
	
	inodes_amount = initrd_superblock->inodesmax;
	initrd_nodes = reinterpret_cast<initrd_virtual_node*>( __ALLOC(sizeof(initrd_virtual_node) * inodes_amount) );
	
	
	for (unsigned i = 0; i < inodes_amount; i++)
    {
        initrd_nodes[i].gid = initrd_inodes[i].gid;
        initrd_nodes[i].oid = initrd_inodes[i].oid;
        initrd_nodes[i].inode = initrd_inodes[i].inode;
        initrd_nodes[i].size = initrd_inodes[i].eof_offset;
        initrd_nodes[i].flags = initrd_inodes[i].attributes;
        initrd_nodes[i].mask = initrd_inodes[i].file_mode;
		
		initrd_nodes[i].callback.readdir = initrd_readdir;
		initrd_nodes[i].callback.findindir = initrd_findindir;
		
		
    }
	
	initrd_root = &initrd_nodes[0];
	
	return initrd_root;
}
