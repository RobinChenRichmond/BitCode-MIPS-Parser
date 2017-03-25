BitCode: BitCode.o Instruction.o Opcode.o RegisterTable.o BitCodeParser.o
	g++ -Wall -Wno-deprecated -g -o BitCode BitCode.o Opcode.o BitCodeParser.o RegisterTable.o Instruction.o

BitCodeParser.o: BitCodeParser.h Opcode.h RegisterTable.h Instruction.h 

Instruction.o: Instruction.h 

Opcode.o: Opcode.h 

RegisterTable.o: RegisterTable.h   

clean:
	/bin/rm -f BitCode *.o core
