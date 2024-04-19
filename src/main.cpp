#include <iostream>
#include "Internals/CPU/CPU.h"

void clearStack(Memory& memory, u32 sp) {
	sp++;
	while (--sp > 0) {
		memory[sp] = 0;
	}

}

int main() {
	CPU cpu; 
	cpu.reset();
	//clearStack(cpu.memory, cpu.sp);
	cpu.memory[0xFFFC] = 0x20;
	cpu.memory[0xFFFD] = 0x00;
	cpu.memory[0xFFFE] = 0x52;
	cpu.memory[0x5200] = 0xA9;
	cpu.memory[0x5201] = 0x42;
	cpu.memory[0x5202] = 0x60;

	cpu.execute(14);



	return 0;
}