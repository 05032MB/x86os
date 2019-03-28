#include <vfs.hpp>
#include <kmemory.hpp>
#include <critical.hpp>

f_node *first_fs = nullptr;

size_t fs_read(f_node* node,addr_t adr,size_t size,byte* off)
{
	return (node->callback.read != nullptr)? node->callback.read(node, adr, size, off) : 0;
}

size_t fs_write(f_node* node,addr_t adr,size_t size,byte* off)
{
	return (node->callback.write != nullptr)? node->callback.write(node, adr, size, off) : 0;
}
unsigned fs_open(f_node* node)
{
	return (node->callback.open != nullptr)? node->callback.open(node) : 2;
}
unsigned fs_close(f_node* node)
{
	return (node->callback.close != nullptr)? node->callback.close(node) : 2;
}

d_node* fs_readdir(f_node *node, dword index)
{
    return ( (node->flags & 0xF) == f_type::DIRECTORY && node->callback.readdir != nullptr ) ?
			node->callback.readdir(node, index) :
			nullptr;
}
f_node* fs_findindir(f_node *node, char* name)
{
    return ( (node->flags & 0xF) == f_type::DIRECTORY && node->callback.findindir != nullptr ) ?
			node->callback.findindir(node, name) :
			nullptr;
}

void init_vfs()
{
	//does nothing now
	
}