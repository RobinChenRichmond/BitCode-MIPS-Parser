// Programmer: Robin Chen

#ifndef __BITCODEPARSER_H__
#define __BITCODEPARSER_H__

using namespace std;

#include <iostream>
#include <fstream>
#include "Instruction.h"
#include "RegisterTable.h"
#include "Opcode.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <string>

/* This class reads in a MIPS assembly file and checks its syntax.  If
 * the file is syntactically correct, this class will retain a list 
 * of Instructions (one for each instruction from the file).  This
 * list of Instructions can be iterated through.
 */
   

class BitCodeParser{
 public:
  // Specify a text file containing MIPS assembly instructions. Function
  // checks syntactic correctness of file and creates a list of Instructions.
  BitCodeParser(string filename);

  // Returns true if the file specified was syntactically correct.  Otherwise,
  // returns false.
  bool isFormatCorrect() { return myFormatCorrect; };

  // Iterator that returns the next Instruction in the list of Instructions.
  Instruction getNextInstruction();

 private:
  vector<Instruction> myInstructions;      // list of Instructions
  int myIndex;                             // iterator index
  bool myFormatCorrect;

  RegisterTable registers;                 // encodings for registers
  OpcodeTable opcodes;                     // encodings of opcodes

  // Decomposes a line of assembly code into strings for the opcode field and operands, 
  // checking for syntax errors and counting the number of operands.
  void getTokens(string line, string &opcode, string *operand, string &funct);

  // Given an Opcode, a string representing the operands, 
  // breaks operands apart and stores fields into Instruction.
  bool getOperands(Instruction &i, Opcode o, string *operand);


  // Returns true if character is white space
  bool isWhitespace(char c)    { return (c == ' '|| c == '\t'); };

  // Returns true if character is a digit
  bool isDigit(char c)         { return (c >= '0' && c <= '9'); };

  // Returns true if character is an uppercase letter
  bool isAlphaUpper(char c)    { return (c >= 'A' && c <= 'Z'); };

  // Returns true if character is a lowercase letter
  bool isAlphaLower(char c)    { return (c >= 'a' && c <= 'z'); };

  // Returns true if character is a plus or minus
  bool isSign(char c)          { return (c == '-' || c == '+'); };

  // Returns true if character is an alphabetic character
  bool isAlpha(char c)         {return (isAlphaUpper(c) || isAlphaLower(c)); };

  //Given a string of 32-bit binary representation, return a valid instruction
  string decode(Instruction i);


  // convert the string type binary number into the int type decimal number
  int convertToDecimal(string binaryNumber);

  // convert the int type decimal number into the string type hexadecimal number
  string convertToHex(int decimal);

  // convert a decimal integer into string
  string convertToString(int decimal);
 

  //Given a string of 32-bit binary representation of R-type, return a valid instruction
  string decodeR(Instruction i);

  //Given a string of 32-bit binary representation of J-type, return a valid instruction
  string decodeJ(Instruction i);

  //Given a string of 32-bit binary representation of I-type, return a valid instruction
  string decodeI(Instruction i);

};

#endif
