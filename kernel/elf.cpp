#include <elf.hpp>

using namespace ELF;

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
	return (ELF32_SecHeader *)((int)hdr + hdr->e_shoff);
}
 
static inline ELF32_SecHeader *elf_section(ELF32_Header *hdr, int secid) {
	return (elf_sheader(hdr))+secid;
}

static char *elf_str_table(ELF32_Header *hdr) {
	if(hdr->e_shstrndx == SHN_UNDEF) return NULL;
	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
}
 
static char *elf_lookup_string(ELF32_Header *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == NULL) return NULL;
	return strtab + offset;
}

static void * load_elf_rel(ELF32_Header *hdr)
{


}

void * ELF::load_elf(ELF32_Header *hdr)
{
	if(!check_elf_header(hdr))return NULL;

	switch(hdr->e_type){
		case ET_REL:
			return load_elf_rel(hdr);
	}
	return NULL;

}