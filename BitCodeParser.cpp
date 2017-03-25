// Programmer: Robin Chen

#include "BitCodeParser.h"

BitCodeParser::BitCodeParser(string filename)
  // Specify a text file containing MIPS assembly instructions. Function
  // checks syntactic correctness of file and creates a list of Instructions.
{
  Instruction i;
  myFormatCorrect = true;

  ifstream in;
  in.open(filename.c_str());
  if(in.bad()){
    myFormatCorrect = false;
  }
  else{
    string line;
    while(getline(in, line)){
      string opcode("");
      string operand[80];
      string funct_field("");
      getTokens(line, opcode, operand,funct_field);
      Opcode o = opcodes.getOpcode(opcode,funct_field);     
 
      if(o == UNDEFINED){
        // invalid opcode specified
        myFormatCorrect = false;
        break;
      }

      bool success = getOperands(i, o, operand);
      if(!success){
        myFormatCorrect = false;
        break;
      }

      string decoding = decode(i);
      i.setDecoding(decoding);
      myInstructions.push_back(i);

    }
  }

  myIndex = 0;
}


Instruction BitCodeParser::getNextInstruction()
  // Iterator that returns the next Instruction in the list of Instructions.
{
  if(myIndex < (int)(myInstructions.size())){
    myIndex++;
    return myInstructions[myIndex-1];
  }
  
  Instruction i;
  return i;

}

void BitCodeParser::getTokens(string line,
             string &opcode,
             string *operand,
        string &funct)
  // Decomposes a line of 32 bit binary code into strings for the opcode field, operands, and funct field
  // checking for syntax errors and counting the number of operands.
{
    // locate the start of a comment
    string::size_type idx = line.find('#');
    if (idx != string::npos){ // found a ';'
     line = line.substr(0,idx);
    }

    int len = line.length();
    opcode = "";
    if (len < 32){
      return;
    }
    int p = 0; // position in line

    while (p < len && isWhitespace(line.at(p))){
    // cut off the white space before the instruction
     p++;
    }
    line = line.substr(p,32);
    len = line.length();
    int q = 0;
    while (q < len){
    // test that if all 32 bits are either '0' or '1'
      if(line.at(q) != '1' && line.at(q) != '0'){
        myFormatCorrect = false;
      }
      q++;
    }
    // opcode starts: the first 6 char
    int counter = 0;
    while (counter < len && counter < 6)
    {
      opcode = opcode + line.at(counter);
      counter++;
    }

  if(opcode == "000000"){
  // case of RTYPE: has 4 possible kinds of operand
    int ROperand = 5;
    int RFunct = 6;
     
    for(int i = 0; i < 4; i ++){
      for(int j = 0; j < ROperand; j ++){
        operand[i] = operand[i] + line.at(counter);
        counter ++;
      }
    }
      for(int k = 0; k < RFunct; k ++){
          funct = funct + line.at(counter);
          counter ++;
      } 
  } else if(opcode == "000010" || opcode == "000011"){
  // case of JTYPE: has 1 opssible kind of operand
    int JAddress = 26;
    for(int i = 0; i < JAddress;i ++){
      operand[3] = operand[3] + line.at(counter);
      counter ++;
    }
    funct = "000000";
  } else {
  // case of ITYPE: has 3 possible kinds of operand
    int IOperand = 5;
    int IAddress =  16;
    
    for(int i = 0; i < 2; i ++){
      for(int j = 0; j < IOperand; j ++){
        operand[i] = operand[i] + line.at(counter);
        counter ++;
      }
    }
    for(int k = 0; k < IAddress; k ++){
      operand[3] = operand[3] + line.at(counter);
      counter ++;
    }
    funct = "000000";
  }

  return;
}   

bool BitCodeParser::getOperands(Instruction &i, Opcode o, 
          string *operand)
  // Given an Opcode, a string representing the operands, and the number of operands, 
  // breaks operands apart and stores fields into Instruction.
{

  Register rs, rt, rd;
  int imm = 0;
  rs = rt = rd = NumRegisters;

  int rs_p = opcodes.RSposition(o);
  int rt_p = opcodes.RTposition(o);
  int rd_p = opcodes.RDposition(o);
  int imm_p = opcodes.IMMposition(o);

  // the code has at most 4 kinds of operands
  int ops[4];
  for(int j = 0; j < 4; j ++){
  // convert each operand to decimal number
    ops[j] = convertToDecimal(operand[j]);
  }

  if(rs_p != -1){
    rs = (Register)ops[0];
    if(rs == 0){
      return false;
    }   
  }
  if(rt_p != -1){
    rt = (Register)ops[1];
    if(rt == 0){
      return false;
    }   
  }
  if(rd_p != -1){
    rd = (Register)ops[2];
    if(rd == 0){
      return false;
    }   
  }
  if(imm_p != -1){
    imm = ops[3];
    if(imm == 0){
      return false;
    }   
  }

  i.setValues(o, rs, rt, rd, imm);

  return true;
}


string BitCodeParser::decode(Instruction i)
  // Given a valid 32 bit code, returns a string representing the MIPS instruction.
{
  Opcode op = i.getOpcode();
  OpcodeTable opTable;
  InstType instT = opTable.getInstType(op);

  //Consider that there are three types of instruction
  string decodedString;
  if(instT == RTYPE)
  {
    decodedString = decodeR(i);
  }
  else if(instT == JTYPE){
    decodedString = decodeJ(i);
  }
  else{
    decodedString = decodeI(i);
  }
  return decodedString;
}

int BitCodeParser::convertToDecimal(string binaryNumber){
  // convert the string type binary number to the int type decimal number
  int leng = binaryNumber.length();
  bool negative = false;
  if(binaryNumber.length() >= 16){
    if(binaryNumber.at(0) == '1'){
      negative = true;
    }
  }
  if(negative == true){
    for(int index = 0; index < leng; index ++){
      if(binaryNumber.at(index) == '0'){
        binaryNumber.at(index) = '1';
      } else {
        binaryNumber.at(index) = '0';
      }
    }
  }

  int p = 0;
  while(p < leng && binaryNumber.at(p) == '0'){
      p++;
  }

  int sum = 0;
  if(p >= leng){
   return 0;
  }
  int temp = leng-p;
  for(int i = p; i < leng; i ++){
    if(binaryNumber.at(i) == '1'){
      sum = sum + pow(2,temp-1);
    }
    temp --;
  }
  if(negative){
    sum = sum+ 1;
    sum = 0 - sum;
  }
  return sum;
}

string BitCodeParser::convertToString(int decimal){
  // convert a decimal integer to string
  string result;

  bool negative = false;
  if(decimal < 0){
    decimal = 0-decimal;
    negative = true;
  }
  string decTable = "0123456789";
  while(decimal > 0){
    for(int i = 0; i < 10; i ++){
      if(decimal%10 == i){
        result = decTable[i] + result;
      }
    }
    decimal = decimal/10;
  }
  if(negative){
    result = '-' + result;
  }

  return result;
}

string BitCodeParser::convertToHex(int decimal){
  // convert the int type positive or negative decimal number
  // to the string type hexadecimal number
  string hexTable = "0123456789abcdef";
  string result;
  bool negative = false;
  int counter = 0;

  if(decimal < 0){
    negative = true;
    decimal = 0 - decimal;
    int temp = decimal;
    while(temp > 0){
      temp = temp/16;
      counter ++;
    }
    decimal = pow(16,counter) - decimal;
  }

  while(decimal > 0){
    for(int i = 0; i < 16; i ++){
      if(decimal%16 == i){
        result = hexTable.at(i) + result;
      }
    }
    decimal = decimal/16;
  }

  if(negative){
    while(result.length() < 8){
      result = 'f' + result;
    }
  }

  return result;
}

string BitCodeParser::decodeR(Instruction i)
  //Given a string of 32-bit binary representation of R-type, return a valid instruction
{
  Opcode op = i.getOpcode();
  OpcodeTable opTable;

  int length = opTable.numOperands(op);
  string operand[length];

  string name = opTable.getName(op);

  if(opTable.RDposition(op) != -1){
    string rd = registers.getStr(i.getRD());
    operand[opTable.RDposition(op)] = rd;
  }

  if(opTable.RSposition(op) != -1){
    string rs = registers.getStr(i.getRS());
    operand[opTable.RSposition(op)] = rs;
  }

  if(opTable.RTposition(op) != -1){
    string rt = registers.getStr(i.getRT());
    operand[opTable.RTposition(op)] = rt;
  }

  if(opTable.IMMposition(op) != -1){
    string imm = convertToString(i.getImmediate());
    operand[opTable.IMMposition(op)] = imm;
  }
  
  string result = name + "\t";
  for(int i = 0; i < length; i ++){
    result = result + operand[i];
    if(i < length-1){
      result = result + ", ";
    }
  }

  return result;
}

string BitCodeParser::decodeJ(Instruction i)
  //Given a string of 32-bit binary representation of J-type, return a valid instruction
{
  Opcode op = i.getOpcode();
  OpcodeTable opTable;

  int length = opTable.numOperands(op);
  string operand[length];

  string name = opTable.getName(op);

  int decimal = i.getImmediate();
  string imm = convertToHex(decimal*4);
  imm = "0x" + imm;
  operand[opTable.IMMposition(op)] = imm;

  string result = name + "\t" + operand[0];

  return result;
}

string BitCodeParser::decodeI(Instruction i)
  //Given a string of 32-bit binary representation of I-type, return a valid instruction
{
  Opcode op = i.getOpcode();
  OpcodeTable opTable;

  int length = opTable.numOperands(op);
  string operand[length];

  string name = opTable.getName(op);

  if(opTable.RSposition(op) != -1){
    string rs = registers.getStr(i.getRS());
    operand[opTable.RSposition(op)] = rs;
  }

  if(opTable.RTposition(op) != -1){
    string rt = registers.getStr(i.getRT());
    operand[opTable.RTposition(op)] = rt;
  }

  if(opTable.IMMposition(op) != -1){
    int decimal = i.getImmediate();
    string imm;
    if(opcodes.isIMMLabel(op)){
        string Immediate = convertToHex(decimal*4);      
        imm = "0x" + Immediate;
    } else {
        imm = convertToString(decimal);
    }
    operand[opTable.IMMposition(op)] = imm;
  }

  string result = name + "\t";

  if(opcodes.isIMMBracket(op)){
    for(int i = 0; i < length-1; i ++){
      result = result + operand[i];
      if(i < length-2){
        result = result + ", ";
      }
    }
    result = result + "(" + operand[length-1] + ")";
  } else{
    for(int i = 0; i < length; i ++){
      result = result + operand[i];
      if(i < length-1){
        result = result + ", ";
      }
    }
  }

  return result;
}
