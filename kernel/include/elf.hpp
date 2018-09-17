#ifndef _ELF_H
#define _ELF_H

#include <types.hpp>
#include <stdio.h>

namespace ELF{

#define ELF32_Addr dword
#define ELF32_Off dword
#define ELF32_Sword sdword
#define ELF32_Word dword
#define ELF32_Half word

#define EI_NIDENT 16

struct ELF32_Header{

	dword	e_ident[EI_NIDENT];
	ELF32_Half	e_type;
	ELF32_Half	e_machine;
	ELF32_Word	e_version;
	ELF32_Addr	e_entry;
	ELF32_Off	e_phoff;
	ELF32_Off	e_shoff;
	ELF32_Word	e_flags;
	ELF32_Half	e_ehsize;
	ELF32_Half	e_phentsize;
	ELF32_Half	e_phnum;
	ELF32_Half	e_shentsize;
	ELF32_Half	e_shnum;
	ELF32_Half	e_shstrndx;

}__attribute__((packed));

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
	ELF32_Word	sh_name;
	ELF32_Word	sh_type;
	ELF32_Word	sh_flags;
	ELF32_Addr	sh_addr;
	ELF32_Off	sh_offset;
	ELF32_Word	sh_size;
	ELF32_Word	sh_link;
	ELF32_Word	sh_info;
	ELF32_Word	sh_addralign;
	ELF32_Word	sh_entsize;
}__attribute__((packed));

enum Sec_Types {
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

void* load_elf(ELF32_Header*);
bool check_elf_header(ELF32_Header*);

}
#endif