#include "globals.h"

/****************************************************************
 * Code file for the 'Globals' class for global constants and
 * functions.
 *
 * Documentation for the functions is in the header for those
 * functions.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Katherine Haberlin
 * Date: 31 October 2017
 *
**/

/******************************************************************************
 * Function 'BitStringToDec'.
 * Convert a bit string to a decimal value.
 * Source: cplusplus.com
 *
 * Parameters: 
 *   thebits - the binary string to convert
 * Returns:
 *   stoivalue - the decimal equivalent
**/

int Globals::BitStringToDec(const string thebits) const {
#ifdef EBUG
  Utils::log_stream << "enter BitStringToDec\n"; 
#endif

  // Remember that the second parameter is for positioning the pointer
  // after doing the conversion (so we don't care here) and the third
  // says we are converting from a string of digits in binary.

  //The decimal value starts at zero.
  //If the bit is one, add the power of 2 corresponding to the position. 
  int stoivalue = 0;
  int size = thebits.length();

  for (int i = 0; i < size; ++i) {
    if (thebits.substr(i, 1) == "1") {
      stoivalue += pow(2, (size - 1 - i));
    }
  }

#ifdef EBUG
  Utils::log_stream << "leave BitStringToDec\n"; 
#endif

  return stoivalue;
}

/******************************************************************************
 * Function 'DecToBitString'.
 * This function converts a decimal 'int' to a string of 0s and 1s.
 *
 * We only allow conversion to a string of length 12 or of length 16
 * because we only allow an address (lessequal 4096 = 2^12) or a hex
 * operand of 16 bits.
 *
 * Parameters:
 *   value - the value to convert
 *   how_many_bits - the length of the result
 * Return:
 *   bitsetvalue - the binary representation of the decimal value
**/
string Globals::DecToBitString(const int value, const int how_many_bits) const {
#ifdef EBUG
  Utils::log_stream << "enter DecToBitString\n";
#endif
  
  int decimal = value;
  string bitsetvalue = "";
  vector<int> storage;

  //Conver to Binary
  for (int i = 0; i < how_many_bits; ++i) {
    if (decimal > 0) {
      int remainder = decimal % 2;
      decimal /= 2;
      storage.push_back(remainder);
    } else {
      storage.push_back(0);
    }
  }
  for (int i = storage.size() - 1; i >= 0; --i) {
    bitsetvalue += Utils::Format(storage.at(i));
  }

#ifdef EBUG
  Utils::log_stream << "leave DecToBitString\n";
#endif

  return bitsetvalue;
}
