 #ifndef PORTS_H
 #define PORTS_H
 

 //extern "C" __attribute__((fastcall)) void outb(uint16_t port, uint8_t val); 
 //extern "C" __attribute__((fastcall)) void inb(uint16_t port); 
 
 
 //--OUTx----
 inline void outb(uint16_t port, uint8_t val)
 {
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port)); 
	//asm volatile(".intel_syntax;""outb %1, %0"".att_syntax;" ::"a"(val), "Nd"(port));
 }
 //---INx---

 inline uint8_t inb(uint16_t port)
 {
	uint8_t ret;
	
	//asm volatile(".intel_syntax;""inb %0, %1;"".att_syntax;" : "=a"(ret) : "Nd"(port) ); 
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port) );
	
	return ret;
 }
 
 #endif