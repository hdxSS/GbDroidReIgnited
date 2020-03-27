// GbReIgnited.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <stack>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream> 

typedef union
{
	struct
	{
		uint8_t lo;
		uint8_t hi;
	};
	uint16_t word;
}   Register16_t;

//Our CPU regisgers
typedef struct
{
	//Our basic registers, AF, BC, DE, HL
	Register16_t AF, BC, DE, HL;

	//Special Registers
	Register16_t SP, PC;
}   regs_t;

unsigned short address;
std::stack <unsigned short> gbStack;
unsigned short tAddress;

//--- LoadFile Module Related

unsigned char memory[0xFFFF];
unsigned char memoryA[0xFFFF];
std::string ROM = "tetris.gb";
char letter;

//-- OpFetcher/OpDecoder Module related
unsigned char engager;
unsigned char _8bitIn;
unsigned char _16bitIn;
unsigned short pc;
std::string vEng; //verbose engager


void PopMode();
void InitGb();
void LoadFile();
void OpDecoder();
void OpFetcher();

class Emulator {       // The class
public:             // Access specifier
	
	//Variables
	regs_t R16;

	//Functions

	void InitGb() {

		R16.PC.word = 0;
		R16.SP.word = 0;
		R16.AF.word = 0;
		R16.BC.word = 0;
		R16.DE.word = 0;
		R16.HL.word = 0;

	} //End InitGb()

	void OpFetcher() {


		engager = memoryA[R16.PC.word];
		std::cout << "opcode fetched - " << std::hex << (int)engager << "\n";
		if (engager == 0xcb) {
			engager = memoryA[R16.PC.word + 1];

			std::cout << "opcode type 0xCB found -" << std::hex << std::to_string(engager) << "\n";
		} //End 0xCB type

		else
		{
			_8bitIn = memoryA[R16.PC.word + 1];
			_16bitIn = memoryA[R16.PC.word + 2];
			std::stringstream stream;
			stream << std::hex << (int)engager;
			std::string result(stream.str());

			//vEng = std::to_string((int)engager);
			std::cout << "Engager - C3 -  " << result << "\n";
			std::cout << "8bit Inm -  " << std::hex << (int)_8bitIn << "\n";
			std::cout << "16bit Inm -  " << std::hex << (int)_16bitIn << "\n";


		} //End normal engager
	} // End OpFetcher()

	void OpDecoder() {


	} //End Opdecoder()

	void LoadFile() {

		std::ifstream fin(ROM, std::ios::binary);
		int length = fin.tellg();
		if (!fin) { std::cout << "File not found " << std::endl; }
		for (int i = 0; i <= 32000; i++) { fin.get(letter);	memoryA[i] = letter; }
		//did it worked?
		//it does :)

	} // End LoadFile()


};



void PopMode() {
	//get stack size
	for (int i = gbStack.size(); i >= 1; i--) {
		tAddress = gbStack.top();
		std::cout << "[" << i << "] - " << std::hex << tAddress << "\n";
		gbStack.pop();
	}
}


int main() {

	Emulator gb;
	/*
	while (1) {
	std::cout << "assign address to the stack - 0 to exit push mode\n";

	std::cin >> std::hex >> address;
	if (address == 0)  {
		PopMode();
	}
	else
	gbStack.push(address);
	std::cout << "\n";
	} */
	gb.InitGb();
	gb.LoadFile();
	gb.OpFetcher();
	gb.OpDecoder();
	return 0;
}