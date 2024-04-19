#pragma once
#include <assert.h>
#include <iostream>
using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;

struct Memory {
	static constexpr u32 MAX_MEMORY = 1024 * 64;
	Byte Data[MAX_MEMORY];

	void initialize() {
		for (Byte data : Data) data = 0;
	}

	Byte operator [](const u32 address) const { 
		assert(address < MAX_MEMORY);
		
		return Data[address]; 
	}

	Byte& operator [](const u32 address) {
		assert(address < MAX_MEMORY);

		return Data[address];
	}

	void writeWord(u32& cycles,const Word data,const u32 address) {
		Data[address] = data & 0xFF;
		Data[address + 1] = (data >> 8);
		cycles -= 2;
	}

};

class CPU{
public:

	//Registers
	Word pc; //Program Counter
	Word sp; //Stack Pointer

	Byte a, x, y;

	//Status Flags

	Byte C : 1;
	Byte Z : 1;
	Byte I : 1;
	Byte D : 1;
	Byte B : 1;
	Byte V : 1;
	Byte N : 1;

	Memory memory;

	void reset();

	void execute(u32 cycles);

private:
	Byte readByte(u32& cycles, Word Address);
	Byte readByte(u32& cycles, Byte Address);
	Word readWord(u32& cycles, Word Address);
	Byte fetch(u32& cycles);
	Word fetchWord(u32& cycles);

	void stackDump() {
		system("cls");
		std::cout << std::hex;
		u32 spClone = 256;
		int idx = 0;
		while (idx++ < 31) {
			std::cout << "Idx: " << idx << " ";
			if (idx <= 15) std::cout << " ";
			std::cout << " SP: " << spClone;
			if(spClone == 256)
				std::cout << " |";
			else
				std::cout << "  |";

			for (int i = 0; i < 8; i++) {
				std::cout << std::hex << (unsigned)memory[spClone--] << " ";
			}
			printf("\n");
		}
	}

};

