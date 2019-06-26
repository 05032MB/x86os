#ifndef _ELF_H
#define _ELF_H

#include <types.hpp>
#include <stdio.h>

extern dword * current_directory;

namespace ELF{

#define ELF32_Addr dword
#define ELF32_Off dword
#define ELF32_Sword sdword
#define ELF32_Word dword
#define ELF32_Half word

#define EI_NIDENT 16

struct ELF32_Header{

	byte	e_ident[EI_NIDENT]; //way to identify elf file type (see ELF_Ident)
	ELF32_Half	e_type; //what we are dealing with (see ELF_Type)
	ELF32_Half	e_machine; //target machine. See(ELF_Machine). We are expecting EM_386 (0x3).
	ELF32_Word	e_version; //version of ELF standard. Should be 1.
	ELF32_Addr	e_entry; //Address of entry point of executable.
	ELF32_Off	e_phoff; //Offset to program header table.
	ELF32_Off	e_shoff; //Offset to first section header table.
	ELF32_Word	e_flags; //ELF Machine Flags (don't confuse with ELF_Machine enum). For IA-32 is always 0. 
	ELF32_Half	e_ehsize; //Header's own size (bytes).
	ELF32_Half	e_phentsize; //Size of one entry in program header table. All others are same size.
	ELF32_Half	e_phnum; //Number of entries in pht.
	ELF32_Half	e_shentsize; //Size of section header (bytes)
	ELF32_Half	e_shnum; //Number of section headers.
	ELF32_Half	e_shstrndx; //Index of section header table where we can find string table (special kind of section table).

}__packed;

enum ELF_Type { //e_type
	ET_NONE		= 0, // No Type
	ET_REL		= 1, // Relocatable File
	ET_EXEC		= 2, // Executable File
	ET_DYN		= 3, // Shared Object Type
	ET_CORE		= 4, // Core File
};

enum ELF_Machine {
	EM_NONE		= 0, //No machine
	EM_M32		= 1, //AT&T WE 32100
	EM_SPARC	= 2, //SPARC
	EM_386		= 3, //Intel 80386
	EM_68K		= 4, //
	EM_88K		= 5, //
	EM_860		= 6, //Intel 80860 //RISC
	EM_MIPS		= 8, //MIPS RS3000

};

enum ELF_Version {
	EV_NONE		= 0, //Invalid
	EV_CURRENT	= 1, //Current

};

enum ELF_Ident {
	EI_MAG0		=0,	//0x7f
	EI_MAG1	,		//E
	EI_MAG2	,		//L
	EI_MAG3,		//F
	EI_CLASS,		//ELFCLASS
	EI_DATA,
	EI_VERSION, //EV_CURRENT
	EI_PAD,
};

#define EIMAG0 0x7f
#define EIMAG1 'E'
#define EIMAG2 'L'
#define EIMAG3 'F'

enum ELF_Class{
	ELFCLASSNONE = 0,
	ELFCLASS32 = 1, //32bit
	ELFCLASS64 = 2, //64bit

};

enum ELF_Data{
	ELFDATANONE = 0, //Invalid
	ELFDATA2LSB, //LittleEndian
	ELFDATA2MSB, //BigEndian

};

#define ELFDATA_CURRENT ELFDATA2LSB
#define EM_CURRENT EM_386 //our target architecture
#define ELFCLASS_CURRENT ELFCLASS32


//////////////////////////////////////////////////////////////////////

struct ELF32_SecHeader{
	ELF32_Word	sh_name; //section name
	ELF32_Word	sh_type; //section type. See Sec_Types.
	ELF32_Word	sh_flags; //flags. See Sec_Flags
	ELF32_Addr	sh_addr; //Address where to load section
	ELF32_Off	sh_offset; //Offset from the beginning of file to the start of the section
	ELF32_Word	sh_size; //Size of section (bytes).
	ELF32_Word	sh_link; //nwm
	ELF32_Word	sh_info; //look below
	ELF32_Word	sh_addralign; //0/1 no alignment. Otherwise align it when loading.
	ELF32_Word	sh_entsize; //Size of each entry in special sections (eg symbol table section). Otherwise 0.
}__packed;

enum Sec_Types {	//guess
	SHT_NULL	= 0,   
	SHT_PROGBITS= 1,  
	SHT_SYMTAB	= 2,
	SHT_STRTAB	= 3,   
	SHT_RELA	= 4, 
	SHT_HASH	= 5,
	SHT_DYNAMIC	= 6,
	SHT_NOTE	= 7,
	SHT_NOBITS	= 8,   
	SHT_SHLIB	= 10,  
	SHT_DYNSYM	= 11,  
};

enum Sec_Flags{
	SHF_WRITE = 1,
	SHF_ALLOC = 2,
	SHF_EXECINSTR = 3,
};

#define SHN_UNDEF 0 //xD

//////////////////////////////////////////////

struct ELF32_ProgHeader{
	ELF32_Word p_type;
	ELF32_Off p_offset;
	ELF32_Addr p_vaddr;
	ELF32_Addr p_paddr;
	ELF32_Word p_filesz;
	ELF32_Word p_memsz;
	ELF32_Word p_flags;
	ELF32_Word p_align;
}__packed;

enum Prog_Types{
	PT_NULL = 0,
	PT_LOAD,
	PT_DYNAMIC,
	PT_INTERP,
	PT_NOTE,
	PT_SHLIB,
	PT_PHDR,
};

void* load_elf(ELF32_Header*, dword * pdir = current_directory);
bool check_elf_header(ELF32_Header*);

}
#endif