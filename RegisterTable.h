#ifndef _REGISTERTABLE_H
#define _REGISTERTABLE_H

#include <string>

using namespace std;

/* This class stores information about the valid register names for MIPS.
 */

typedef int Register;
const int NumRegisters = 32;

struct RegisterEntry{
  string name;
  Register number;
};

class RegisterTable{
 public:
  RegisterTable();

  // Given a integer representing a MIPS register operand, returns the string associated
  // with that register.  If number is not a valid register, returns "0".
  string getStr(Register reg);

 private:
  RegisterEntry myRegisters[64];

};


#endif
