/*
 * format.h
 *
 *  Created on: Mar 1, 2010
 *      Author: Rob Henderson laptop
 */

#ifndef FORMAT_H_
#define FORMAT_H_


/*Converts a binary number to decimal
 * Was also modified to handle the mantissa
 * Takes in a binary character string
 * Returns a double in base 10*/
double BinToDec(char*);
/*Takes in a Little Endian Binary String
 * Returns a Big Endian String Representation*/
char* ConvertToBigEndian(char*);
/*Converts an integer to a string representation
 * Takes in an integer number and a base to convert to
 * Returns a character string*/
char* ConvertIntToString(int, int);
/*Converts a double type to a String
 * Takes a double to be converted and a base, base will be 10, but for reusability, this is a good idea
 * Returns a string representation of the Double*/
char* ConvertDoubleToString(double, int);
/*checks for length of string
 * Takes the string to be formatted and also needs the type of formatting we are doing eg:int
 * Returns -1 if invalid */
int IsValidInput(char*,char*);
/*IMPORTANT: Precondition: Length if input string must be divisible by 8
 * Takes in a binary String
*Returns ascii character string representation
*Returns NULL if no null terminator was found*/
char* ConvertBinaryStringToCharacterStringRepresentation(char*);
/* Takes in a binary String
 * Returns a String representation of the 2's comp value*/
char* ConvertBinaryStringToTwosComplement(char*,char*);
/*IMPORTANT: Precondition: Length if input string must be 32. 1 for sign,8 for exponent, 23 for mantissa
 * Takes in a binary string
 * Also takes in whether the format is big endian or little endian
 * Returns the IEEE Floating Point Representation for that String*/
char* ConvertBinaryStringToIEEFloatingPointFormat(char* binaryInput,char*);
/*Takes in a character String
 * Returns the size of the whole portion of the float/int that is represented as a string*/
int sizeOfBasePart(char* input);


#endif /* FORMAT_H_ */
