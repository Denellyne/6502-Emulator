#include "CPU.h"

#define LDSetStatus(reg) Z = (reg == 0); N = (reg & 0b10000000) > 0


void CPU::reset() {
	pc = 0xFFFC;
	sp = 0x0100;

	C = Z = I = D = B = V = N = 0;
	a = x = y = 0;

	memory.initialize();

}

Byte CPU::fetch(u32& cycles) {

	Byte data = memory[pc];
	pc++;
	cycles--;
	return data;

}

Word CPU::fetchWord(u32& cycles) {

	Word data = memory[pc];
	pc++;
	
	data |= (memory[pc] << 8);
	
	pc++;

	cycles -= 2;
	return data;

}

Byte CPU::readByte(u32& cycles,Word Address) {

	Byte data = memory[Address];
	cycles--;
	return data;

}

Byte CPU::readByte(u32& cycles, Byte Address) {

	Byte data = memory[Address];
	cycles--;
	return data;

}

Word CPU::readWord(u32& cycles, Word Address) {
	Byte LoByte = readByte(cycles, Address);
	Address++;
	Byte HiByte = readByte(cycles, Address);
	return LoByte | (HiByte << 8);
}

//Opcodes
static constexpr Byte
INS_ABS_JSR = 0x20,
//LDA
INS_LDA_ZP = 0xA5,
INS_LDA_ABS = 0xAD,
INS_LDA_ABSX = 0xBD,
INS_LDA_ABSY = 0xB9,
INS_LDA_INDX = 0xA1,
INS_LDA_INDY = 0xB1,
INS_LDA_IM = 0xA9,
INS_LDA_ZPX = 0xB5,
//LDX
INS_LDX_IM = 0xA2,
INS_LDX_ZP = 0xA6,
INS_LDX_ABS = 0xAE,
INS_LDX_ABSY = 0xBE,
INS_LDX_ZPY = 0xB2,
//LDY
INS_LDY_IM = 0xA0,
INS_LDY_ZP = 0xA4,
INS_LDY_ABS = 0xAC,
INS_LDY_ABSX = 0xBC,
INS_LDY_ZPX = 0xB4,
//Implied
INS_TSX_IMP = 0xBA,
INS_RTS_IMP = 0x60,
INS_SEC_IMP = 0x38,
INS_SED_IMP = 0xF8,
INS_SEI_IMP = 0x78,
INS_TAX_IMP = 0xAA,
INS_TAY_IMP = 0xA8,
INS_TXA_IMP = 0x8A,
INS_TXS_IMP = 0x9A,
INS_TYA_IMP = 0x98;


void CPU::execute(u32 cycles){

	while (cycles > 0) {
		Byte instruction = fetch(cycles);
		switch (instruction){
		
		case INS_LDA_IM:

			a = fetch(cycles);
			LDSetStatus(a);
			break;
		case INS_LDA_ABS: {

			Word absoluteAddress = fetchWord(cycles);
			a = readByte(cycles, absoluteAddress);
			LDSetStatus(a);

		}break;

		case INS_LDA_ABSX: {

			Word absoluteAddress = fetchWord(cycles);
			absoluteAddress += x;
			a = readByte(cycles, absoluteAddress);
			LDSetStatus(a);

		}break;
		case INS_LDA_ABSY: {

			Word absoluteAddress = fetchWord(cycles);
			absoluteAddress += y;
			a = readByte(cycles, absoluteAddress);
			LDSetStatus(a);

		}break;

		case INS_LDA_INDX: {

			Byte zeroPageAddress = fetch(cycles);
			zeroPageAddress += x;
			cycles--;
			Word effectiveAddress = readWord(cycles, zeroPageAddress);
			a = readByte(cycles, effectiveAddress);
			LDSetStatus(a);

		}break;

		case INS_LDA_INDY: {

			Byte zeroPageAddress = fetch(cycles);
			Word effectiveAddress = readWord(cycles, zeroPageAddress);
			zeroPageAddress += y;

			const bool CrossedPageBoundary = (effectiveAddress ^ effectiveAddress) >> 8;
			if (CrossedPageBoundary)
				cycles--;
			
			a = readByte(cycles, effectiveAddress);
			LDSetStatus(a);

		}break;
		case INS_LDA_ZP:{

			Byte zeroPageAddress = fetch(cycles);
			a = readByte(cycles, zeroPageAddress);
			LDSetStatus(a);

			}break;

		case INS_LDA_ZPX: {

			Byte zeroPageAddress = fetch(cycles);
			zeroPageAddress += x;
			cycles--;
			a = readByte(cycles, zeroPageAddress);
			LDSetStatus(a);

		}break;

		case INS_LDX_IM:

			x = fetch(cycles);
			LDSetStatus(x);
			break;

		case INS_LDX_ZP: {

			Byte zeroPageAddress = fetch(cycles);
			x = readByte(cycles, zeroPageAddress);
			LDSetStatus(x);

		}break;

		case INS_LDX_ZPY: {

			Byte zeroPageAddress = fetch(cycles);
			zeroPageAddress += y;
			cycles--;
			x = readByte(cycles, zeroPageAddress);
			LDSetStatus(x);

		}break;

		case INS_LDX_ABS: {

			Word absoluteAddress = fetchWord(cycles);
			x = readByte(cycles, absoluteAddress);
			LDSetStatus(x);

		}break;

		case INS_LDX_ABSY: {

			Word absoluteAddress = fetchWord(cycles);
			absoluteAddress += y;
			x = readByte(cycles, absoluteAddress);
			LDSetStatus(x);

		}break;

		case INS_LDY_IM:

			y = fetch(cycles);
			LDSetStatus(y);
			break;

		case INS_LDY_ZP: {

			Byte zeroPageAddress = fetch(cycles);
			y = readByte(cycles, zeroPageAddress);
			LDSetStatus(y);

		}break;

		case INS_LDY_ZPX: {

			Byte zeroPageAddress = fetch(cycles);
			zeroPageAddress += x;
			cycles--;
			y = readByte(cycles, zeroPageAddress);
			LDSetStatus(y);

		}break;

		case INS_LDY_ABS: {

			Word absoluteAddress = fetchWord(cycles);
			y = readByte(cycles, absoluteAddress);
			LDSetStatus(y);

		}break;

		case INS_LDY_ABSX: {

			Word absoluteAddress = fetchWord(cycles);
			absoluteAddress += x;
			y = readByte(cycles, absoluteAddress);
			LDSetStatus(y);

		}break;

		case INS_ABS_JSR: {

			Word jumpAddress = fetchWord(cycles);
			memory.writeWord(cycles,pc,sp);
			sp -= 2;
			pc = jumpAddress;
			stackDump();


 		}break;

		case INS_RTS_IMP:
			{
			sp += 2;
			Word address = readWord(cycles, sp);
			pc = address;
			pc++;
			cycles -= 4;
			}
			stackDump();

			break;

		case INS_SEC_IMP:
			C = 1;
			cycles--;
			break;

		case INS_SED_IMP:
			D = 1;
			cycles--;
			break;

		case INS_SEI_IMP:
			I = 1;
			cycles--;
			break;

		case INS_TAX_IMP:
			x = a;
			cycles--;
			LDSetStatus(x);
			break;

		case INS_TAY_IMP:
			y = a;
			cycles--;
			LDSetStatus(y);
			break;

		case INS_TSX_IMP:
			x = memory[sp];
			cycles--;
			LDSetStatus(x);
			break;

		case INS_TXA_IMP:
			a = x;
			cycles--;
			LDSetStatus(a);

			break;

		case INS_TXS_IMP:
			memory[sp] = x;
			cycles--;
			break;

		case INS_TYA_IMP:
			a = y;
			cycles--;
			break;

		default:
			pc++;
			break;
		}

	}


}