// Programmer: Robin Chen

#include "BitCodeParser.h"
#include <iostream>

using namespace std;

/* This file reads in a 32 bits binary file specified at the command line.
 * If the file is correct syntactically, each 32 bit MIPS binary code in 
 * the file will be translated into its instruction decoding and printed
 * to stdout, one per line.
 *
 */

int main(int argc, char *argv[])
{
  BitCodeParser *parser;

  if(argc < 2){
    cerr << "Need to specify an binary file to translate."<<endl;
    exit(1);
  }

  parser = new BitCodeParser(argv[1]);
  if(parser->isFormatCorrect() == false){
    cerr << "Format of input file is incorrect " << endl;
    exit(1);
  }

  Instruction i;

  //Iterate through instructions, printing each decoding.
  i = parser->getNextInstruction();
  while( i.getOpcode() != UNDEFINED){
    cout << i.getDecoding() << endl;
    i = parser->getNextInstruction();
  }
  
  delete parser;
}

