#include <elf.hpp>
#include <string.h>

using namespace ELF;

#include <vga.hpp>

bool ELF::check_elf_header(ELF32_Header *hdr)
{
	if(!hdr)return false;

	if(hdr->e_ident[EI_MAG0] != EIMAG0){
		puts("EI_MAG0 Incorrect");
		return false;
	
	}	
	if(hdr->e_ident[EI_MAG1] != EIMAG1){
		puts("EI_MAG1 Incorrect");
		return false;
	
	}	
	if(hdr->e_ident[EI_MAG2] != EIMAG2){
		puts("EI_MAG2 Incorrect");
		return false;
	
	}	
	if(hdr->e_ident[EI_MAG3] != EIMAG3){
		puts("EI_MAG3 Incorrect");
		return false;
	
	}	
	//////////////////////////////////////////////////
	if(hdr->e_ident[EI_CLASS] != ELFCLASS_CURRENT){
		puts("Invalid ELF class");
		return false;
	}	
	if(hdr->e_ident[EI_DATA] != ELFDATA_CURRENT){
		puts("Invalid ELF byte order");
		return false;
	}	
	if(hdr->e_ident[EI_VERSION] != EV_CURRENT){
		puts("Invalid ELF version");
		return false;
	}
	if(hdr->e_machine != EM_CURRENT){
		puts("Invalid ELF target");
		return false;
	}
	if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
		puts("Unsupported ELF File type");
		return false;
	}
	return true;
}

static inline ELF32_SecHeader *elf_sheader(ELF32_Header *hdr) {
	return (ELF32_SecHeader *)((addr_t)hdr + hdr->e_shoff);
}
 
static inline ELF32_SecHeader *elf_section(ELF32_Header *hdr, int secid) {
	return (elf_sheader(hdr))+secid;
}

static char *elf_str_table(ELF32_Header *hdr) {
	if(hdr->e_shstrndx == SHN_UNDEF) return nullptr;
	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
}
 
static char *elf_lookup_string(ELF32_Header *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == nullptr) return nullptr;
	return strtab + offset;
}

static void * load_elf_rel(ELF32_Header *hdr)
{
	term_print("Relocating elf.\n");
	term_print("Not. Yet.\n");

}
static void * load_elf_exec(ELF32_Header *hdr)
{
	term_print("Loading elf exec.\n");
	void* entry = reinterpret_cast<void*>(hdr->e_entry);
	term_print_dec((int)hdr);
	term_print(":");
	term_print_dec(hdr->e_phnum);
	term_print(":");
	term_print_dec(hdr->e_phoff);
	term_print(":");
	term_print_dec(hdr->e_phentsize);
	term_print(":c");
	term_print_dec((int)((int)hdr + hdr->e_phoff));
	term_print(":\n");
	for(unsigned i=0; i < hdr->e_phnum; i++)
	{
		ELF32_ProgHeader* ph = reinterpret_cast<ELF32_ProgHeader*>(to_addr_t(hdr) + hdr->e_phoff + i * hdr->e_phentsize);

		  	term_print_dec(to_addr_t(ph));
			//term_print_dec(ph->p_type);
			//term_print_dec(ph->p_vaddr);
		switch(ph->p_type)
		{

			case Prog_Types::PT_LOAD:
			{
				term_print("Loading section\n");
				if(ph->p_vaddr < 0xA00000)
				{
					term_print("An embarassing error.");
					return nullptr;
				}
				else if(ph->p_memsz > ph->p_filesz){
					memcpy((void*)to_addr_t(hdr) + ph->p_offset, 0, ph->p_memsz);
				}
				else memcpy((void*)ph->p_vaddr, (const void*)to_addr_t(hdr) + ph->p_offset, ph->p_filesz);
				break;
			}
			default:
				term_print("Unknown section omitted.\n");
				break;
		}
	}

	//remember to swap aspaces in future(tm)
	return entry;

}

void * ELF::load_elf(ELF32_Header *hdr)
{
	if(!check_elf_header(hdr))return nullptr;
	term_print("ELF ok.\n");
	switch(hdr->e_type){
		case ET_REL:
			return load_elf_rel(hdr);
		case ET_EXEC:
			return load_elf_exec(hdr);
	}
	return nullptr;

}