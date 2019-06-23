#include <elf.hpp>
#include <string.h>
#include <paging.hpp>


using namespace ELF;

#include <logger.hpp>

bool ELF::check_elf_header(ELF32_Header *hdr)
{
	if(!hdr)return false;

	if(hdr->e_ident[EI_MAG0] != EIMAG0){
		term_log("EI_MAG0 Incorrect", LOG_CRITICAL);
		return false;
	
	}	
	if(hdr->e_ident[EI_MAG1] != EIMAG1){
		term_log("EI_MAG1 Incorrect", LOG_CRITICAL);
		return false;
	
	}	
	if(hdr->e_ident[EI_MAG2] != EIMAG2){
		term_log("EI_MAG2 Incorrect", LOG_CRITICAL);
		return false;
	
	}	
	if(hdr->e_ident[EI_MAG3] != EIMAG3){
		term_log("EI_MAG3 Incorrect", LOG_CRITICAL);
		return false;
	
	}	
	//////////////////////////////////////////////////
	if(hdr->e_ident[EI_CLASS] != ELFCLASS_CURRENT){
		term_log("Invalid ELF class", LOG_CRITICAL);
		return false;
	}	
	if(hdr->e_ident[EI_DATA] != ELFDATA_CURRENT){
		term_log("Invalid ELF byte order", LOG_CRITICAL);
		return false;
	}	
	if(hdr->e_ident[EI_VERSION] != EV_CURRENT){
		term_log("Invalid ELF version", LOG_CRITICAL);
		return false;
	}
	if(hdr->e_machine != EM_CURRENT){
		term_log("Invalid ELF target", LOG_CRITICAL);
		return false;
	}
	if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
		term_log("Unsupported ELF File type", LOG_CRITICAL);
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

	return nullptr;

}
static void * load_elf_exec(ELF32_Header *hdr)
{
	term_log("Loading elf exec.", LOG_MINOR);
	void* entry = reinterpret_cast<void*>(hdr->e_entry);
	/*term_print_dec((int)hdr);
	term_print(":");*/
	term_log("Header is at: ", (int)hdr, LOG_MINOR);
	term_log("ProgramHeaders are this many: ", hdr->e_phnum, LOG_MINOR);
	term_log("ProgramHeaders are this deep: ", hdr->e_phoff, LOG_MINOR);
	term_log("ProgramHeaders are this large: ", hdr->e_phentsize, LOG_MINOR);
	term_log("First ProgramHeader resides here: ", (int)hdr + hdr->e_phoff, LOG_MINOR);

	for(unsigned i=0; i < hdr->e_phnum; i++)
	{
		ELF32_ProgHeader* ph = reinterpret_cast<ELF32_ProgHeader*>(to_addr_t(hdr) + hdr->e_phoff + i * hdr->e_phentsize);

		  	//term_print_dec(to_addr_t(ph));
			//term_print_dec(ph->p_type);
			//term_print_dec(ph->p_vaddr);
		term_log("Parsing section at address: ", to_addr_t(ph), LOG_MINOR);

		switch(ph->p_type)
		{

			case Prog_Types::PT_LOAD:
			{
				term_log("Loading LOAD section into: ",ph->p_vaddr , LOG_MINOR);
				if(ph->p_vaddr < 0xA00000)
				{
					term_log("An embarassing error.", LOG_CRITICAL);
					return nullptr;
				}
				else if(ph->p_memsz > ph->p_filesz){
					term_log("Mapped bytes=", fmemmap(to_addr_t(hdr) + ph->p_offset, ph->p_memsz, ENTR_PRESENT(1) | ENTR_RW(1) | ENTR_USER(1) | ENTR_WRITETHROUGH(1), &pTracker), LOG_WARNING);
					memset((void*)to_addr_t(hdr) + ph->p_offset, 0, ph->p_memsz);
				}
				else {
					term_log("Mapped bytes=", fmemmap(ph->p_vaddr, ph->p_filesz, ENTR_PRESENT(1) | ENTR_RW(1) | ENTR_USER(1) | ENTR_WRITETHROUGH(1), &pTracker), LOG_WARNING);
					memcpy((void*)ph->p_vaddr, (const void*)to_addr_t(hdr) + ph->p_offset, ph->p_filesz);
				}
				break;
			}
			default:
				term_log("Unknown section omitted.\n", LOG_WARNING);
				break;
		}
	}

	//remember to swap aspaces in future(tm)
	term_log("ELF Possibly Loaded", LOG_OK);
	return entry;

}

void * ELF::load_elf(ELF32_Header *hdr)
{
	if(!check_elf_header(hdr))return nullptr;
	term_log("ELF ok.", LOG_OK);
	switch(hdr->e_type){
		case ET_REL:
			return load_elf_rel(hdr);
		case ET_EXEC:
			return load_elf_exec(hdr);
	}
	return nullptr;

}