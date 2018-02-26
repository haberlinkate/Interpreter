#include "pullet16interpreter.h"

/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Class 'Interpreter' for interpreting the Pullet16 code.
 *
 * Author: Duncan A. Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 31 October 2017
**/

/******************************************************************************
 * Constructor
**/
Interpreter::Interpreter() {
}

/******************************************************************************
 * Destructor
**/
Interpreter::~Interpreter() {
}

/******************************************************************************
 * Accessors and Mutators
**/

/******************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'doADD'.
 * This top level function interprets the 'ADD' opcode.
 * Add contents of memory to ACC.
 *
 * Parameter
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
**/
void Interpreter::DoADD(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter doADD\n"; 
#endif
  
  //cout << "ADD" << addr << " " << target << endl;

  int location = this->GetTargetLocation("ADD", addr, target);
  string data = memory_.at(location);
  int value = globals_.BitStringToDec(data);

  //Two's Complement Addition
  //A negative between 32768 and 65536
  accum_ += value;
  if (accum_ >= 65536) {
    accum_ -= 65536;
  }

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doADD\n"; 
#endif
}

/******************************************************************************
 * Function 'doAND'.
 * This top level function interprets the 'AND' opcode.
 * And contents of memory with ACC.
 *
 * Parameter
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
**/
void Interpreter::DoAND(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter doAND\n"; 
#endif

  //cout << "AND"<< addr << " " << target << endl;

  int location = this->GetTargetLocation("AND", addr, target);
  string data = memory_.at(location);
  string accumulator = globals_.DecToBitString(accum_, 16);
  string anded = "";
  for (int i = 0; i < 16; ++i) {
    if ( data.substr(i, 1) == accumulator.substr(i, 1)) {
      anded += data.substr(i, 1);
    } else {
      anded += "0";  
    }
  }
  accum_ = globals_.BitStringToDec(anded);

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doAND\n"; 
#endif
}

/******************************************************************************
 * Function 'doBAN'.
 * This top level function interprets the 'BAN' opcode.
 * Branch on ACC negative.
 *
 * Parameter
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
**/
void Interpreter::DoBAN(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter doBAN\n"; 
#endif

  //cout << "BAN"<< addr << " " << target << endl;

  //Set program counter to one location before target because program 
  //counter automatically iterates in Interpret function.
  if (accum_ >= 32768) {
    pc_ = this->GetTargetLocation("BAN", addr, target) - 1;
  }

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doBAN\n"; 
#endif
}

/******************************************************************************
 * Function 'doBR'.
 * This top level function interprets the 'BR' opcode.
 * Unconditional branch.
 *
 * Parameter
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
**/
void Interpreter::DoBR(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter doBR\n"; 
#endif

  //cout << "BR"<< addr << " " << target << endl;
  
  //Set program counter to one location before target because program
  //counter automatically iterates in Interpret function.
  pc_ = this->GetTargetLocation("BR", addr, target) - 1;

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doBR\n"; 
#endif
}

/******************************************************************************
 * Function 'doLD'.
 * This top level function interprets the 'LD' opcode.
 *
 * Parameter
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
**/
void Interpreter::DoLD(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter doLD\n"; 
#endif

  //cout << "LD" << addr << " " << target<< endl;


  int location = this->GetTargetLocation("ADD", addr, target);
  string data = memory_.at(location);
  int value = globals_.BitStringToDec(data);

  accum_ = value;

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doLD\n"; 
#endif
}

/******************************************************************************
 * Function 'doRD'.
 * This top level function interprets the 'RD' opcode.
 * Read from standard input into ACC.
 * 
 * Parameter
 *   data_scanner - the scanner for data, if necessary.
**/
void Interpreter::DoRD(Scanner& data_scanner) {
#ifdef EBUG
  Utils::log_stream << "enter doRD\n"; 
#endif

  //cout << "RD" << endl; 

  cin >> accum_;

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doRD\n"; 
#endif
}

/******************************************************************************
 * Function 'doSTC'.
 * This top level function interprets the 'STC' opcode.
 * Store ACC then clear ACC
 * 
 * Parameter
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
**/
void Interpreter::DoSTC(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter doSTC\n"; 
#endif

  //cout << "STC" << addr << " " << target<< endl;

  int location = this->GetTargetLocation("STC", addr, target);
  memory_.at(location) = globals_.DecToBitString(accum_, 16);
  accum_ = 0;

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doSTC\n"; 
#endif
}

/******************************************************************************
 * Function 'doSTP'.
 * This top level function interprets the 'STP' opcode.
 * Stop Execution.
**/
void Interpreter::DoSTP() {
#ifdef EBUG
  Utils::log_stream << "enter doSTP\n"; 
#endif

  cout << "STP" << endl;
  
  pc_ = kPCForStop - 1;

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doSTP\n"; 
#endif
}

/******************************************************************************
 * Function 'doSUB'.
 * This top level function interprets the 'SUB' opcode.
 * Subtract contents of memory from ACC.
 *
 * Parameter
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
**/
void Interpreter::DoSUB(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter doSUB\n"; 
#endif

  //cout << "SUB" << addr << " " << target<< endl;

  int location = this->GetTargetLocation("SUB", addr, target);
  string data = memory_.at(location);
  int value = globals_.BitStringToDec(data);

  //Two's Complement Subtraction
  //A negative between 32768 and 65536
  int sub_value = 65536-value;
  accum_ += sub_value;  

  //cout << "ACCUM: " << accum_ << endl;

#ifdef EBUG
  Utils::log_stream << "leave doSUB\n"; 
#endif
}

/******************************************************************************
 * Function 'doWRT'.
 * This top level function interprets the 'WRT' opcode.
 * Write from ACC to standard output.
 * 
 * Parameters
 *   out_strem - the output file.
**/
void Interpreter::DoWRT(ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter doWRT\n"; 
#endif
 
  cout << "WRT  " << accum_ << " " 
       << globals_.DecToBitString(accum_, 16) << endl;
  
#ifdef EBUG
  Utils::log_stream << "leave doWRT\n"; 
#endif
}

/******************************************************************************
 * Function 'Interpret'.
 * This top level function interprets the code.
 * It will set the accumulator (ACC) and program counter (PC) to 0.
 * It will break an instruction down into the opcode, the address (indirect 
 * or direct), and the target address. Then it will call to execute.
 *
 * Parameters
 *   data_scanner - the scanner for the data, if necessary.
 *   out_stream - the output file.
**/
void Interpreter::Interpret(Scanner& data_scanner, ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Interpret\n"; 
#endif

  ////////////////////////////////////////////////////////////////////////////
  //
  // The only gotcha in this program is that we ALWAYS bump the PC by 1 at
  // the bottom of this loop, so the execute function takes this into account
  // and bumps by one too few.

    // If we have hit the stop we will have returned a flag value that says
    // we should stop execution.

    // If we have executed but the PC is now incremented past the end of
    // memory, we have an execution error.

    // This is an interpreter thing. We prevent infinite loops from being
    // interpreted by having a timeout feature on instruction count.

  //Reset Accumulator and Program Counter to 0
  accum_ = 0;
  pc_ = 0;

  //Loop through memory until you reach a value greater than the memory size.
  while (pc_ < 4096) {
    
    //Unpack the instruction.
    string instruction = memory_.at(pc_);
    if (instruction.size() == 16) {
      string opcode = instruction.substr(0, 3);
      string addr = instruction.substr(3, 1);
      string target = instruction.substr(4, 12);;
      
      //Execute the instruction.
      this->Execute(opcode, addr, target, data_scanner, out_stream);
    } else {
      cout << "Could not find Instruction" << endl;
      this->DoSTP();
    }
    
    //Bump the program counter by 1.
    pc_ += 1;
  }

#ifdef EBUG
  Utils::log_stream << "leave Interpret\n"; 
#endif
}

/******************************************************************************
 * Function 'Execute'.
 * This top level function executes the code.
 * This is called by the interpreter. It looks at the opcode and target and 
 * will determine which instruction is being called. That instruction will
 * be executed by calling a certain function.
 *
 * Parameters
 *   opcode - the 3 digits referring to the specific instruction.
 *   addr - 1 digit referring to direct or indirect addressing.
 *   target - the address of the target of the instruction.
 *   data_scanner - the scanner for the data, if necessary.
 *   out_stream - the output file.
**/
void Interpreter::Execute(string opcode, string addr, string target,
                       Scanner& data_scanner, ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Execute\n"; 
#endif

  if (opcode == "000") {
    this->DoBAN(addr, target);
  } else if (opcode == "001") {
      this->DoSUB(addr, target);
  } else if (opcode == "010") {
      this->DoSTC(addr, target);
  } else if (opcode == "011") {
      this->DoAND(addr, target);
  } else if (opcode == "100") {
      this->DoADD(addr, target);
  } else if (opcode == "101") {
      this->DoLD(addr, target);
  } else if (opcode == "110") {
      this->DoBR(addr, target);
  } else if (opcode == "111") {
    if (target == "000000000001") {
        this->DoRD(data_scanner);
    } else if (target == "000000000010") {
        this->DoSTP();
    } else if (target == "000000000011") {
        this->DoWRT(out_stream);
    } else {
      cout << "Invalid Instruction" << endl;
      this->DoSTP();
    }
  }

#ifdef EBUG
  Utils::log_stream << "leave Execute\n"; 
#endif
}

/******************************************************************************
 * Function 'FlagAddressOutOfBounds'.
 * Check to see if an address is between 0 and 'kMaxMemory' inclusive and
 * die if this isn't the case.
 *
 * Parameter:
 *   address - the address to check for out of bounds
**/
void Interpreter::FlagAddressOutOfBounds(int address) {
#ifdef EBUG
  Utils::log_stream << "enter FlagAddressOutOfBounds\n"; 
#endif

#ifdef EBUG
  Utils::log_stream << "leave FlagAddressOutOfBounds\n"; 
#endif
}

/******************************************************************************
 * Function 'GetTargetLocation'.
 * Get the target location, perhaps through indirect addressing.
 * If direct addressing, take target address and convert to decimal value.
 * 
 * If indirect addressing, take target address and retrieve binary value
 * at that address. Go to that address and convert the value there to
 * decimal.
 *
 * Parameter:
 *   label - the label for our tracing output
 *   address - is this indirect or not?
 *   target - the target to look up
**/
int Interpreter::GetTargetLocation(string label, string address, string target) {
#ifdef EBUG
  Utils::log_stream << "enter GetTargetLocation\n"; 
#endif
  
  //Instantiate variables
  globals_ = Globals();  
  int location;

  //Direct
  location = globals_.BitStringToDec(target);
  //cout << "LOCATION: " << location << endl; 
 
  if (address == "1") {
    //Indirect
    string first_four = target.substr(0, 4);
    if (first_four == "0000") {
      string new_target = memory_.at(location);
      location = globals_.BitStringToDec(new_target);
    } else {
      cout << "Invalid Address" << endl;
      this->DoSTP();
    }  
  }

#ifdef EBUG
  Utils::log_stream << "leave GetTargetLocation\n"; 
#endif

  return location;
}

/******************************************************************************
 * Function 'Load'.
 * This top level function loads the machine code into memory.
 *
 * We also open and read the binary executable file, and we verify that the
 * binary that we read matches the ASCII 0 and 1 file that we read as the
 * "machine code" into the memory.
 *
 * Parameters:
 *   in_scanner - the scanner to read for source code
 *   binary_filename - the name of the file of the binary executable
**/
void Interpreter::Load(Scanner& in_scanner, string binary_filename) {
#ifdef EBUG
  Utils::log_stream << "enter Load\n"; 
#endif
  
  //Memory vector set to hard coded size for on 16-bit interpreter.
  //Initiate the program counter.
  for (int i = 0; i < 4096; ++i) {
    memory_.push_back("1111111111111111");
  }
  pc_ = 0;

  //Read a line of ASCII characters and store it in memory.
  //Keep count of how the amount of lines stored.
  while (in_scanner.HasNext()) {
    string s = in_scanner.NextLine();
    memory_.at(pc_) = s;
    pc_ ++;
  }

  globals_ = Globals();

  // Open the binary file and get the size in bytes of the file.

  // Open the binary file and read the characters, pushing them into a
  // 'vector' of 'char' data.

  // Decode the binary, two bytes at a time, and compare to the ASCII that
  // we read in earlier. Die if there's a discrepancy.
  // CAVEAT: We take extra care here because a 'char' is actually an 'int'
  // and thus we need to just pick off the 8 low bits and the next 8 bits
  // and avoid issues of sign propagation. 

#ifdef EBUG
  Utils::log_stream << "leave Load\n"; 
#endif
}

/******************************************************************************
 * Function 'ToString'.
 *
 * Returns:
 *   the prettyprint string for printing
**/
string Interpreter::ToString() {
#ifdef EBUG
  Utils::log_stream << "enter ToString\n"; 
#endif

  string s = "";

#ifdef EBUG
  Utils::log_stream << "leave ToString\n"; 
#endif

  return s;
}
/******************************************************************************
 * Function 'TwosComplementInteger'.
 *
 * This converts a 16 bit integer into the plus or minus 15 bit integer
 * that is the integer of the 2s complement bit pattern.
 *
 * Parameter:
 *   what - the integer value to convert to the integer that is the 2s compl
 *
 * Returns:
 *   the converted value
**/
int Interpreter::TwosComplementInteger(int what) {
#ifdef EBUG
  Utils::log_stream << "enter TwosComplementInteger\n"; 
#endif

  int twoscomplement = 0;

#ifdef EBUG
  Utils::log_stream << "leave TwosComplementInteger\n"; 
#endif

  return twoscomplement;
}
