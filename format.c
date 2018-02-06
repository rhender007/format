/*
 * format.c
 *
 *  Created on: Feb 26, 2010
 *      Author: Rob Henderson laptop
 */
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
#include "format.h"

int main(int argc, char** argv) {

	/*format <input bit sequence> <type>*/
/*
	argv[1] = "010000010100001001000011010001000000000000000000";
	argv[2] = "string";
   ./format 010000010100001001000011010001000000000000000000 string
   ./format 010000010100001001000011010001000000000000000000 string
   */

	/*Guard at the door approach*/
	if (/*argc != 3 || */!IsValidInput(argv[1], argv[2])) {

		fprintf(stderr, "ERROR\n");
		return 0;
	}

	if (!strcmp(argv[2], "int")) {
		char* twosCompBE = ConvertBinaryStringToTwosComplement(argv[1], "BE");
		char* twosCompLE = ConvertBinaryStringToTwosComplement(argv[1], "LE");

		if ((twosCompBE != NULL) && (twosCompLE != NULL))
			printf("BE: %s\nLE: %s", twosCompBE, twosCompLE);
		else
			fprintf(stderr, "ERROR\n");
	} else if (!strcmp(argv[2], "float"))
	{

		char* ieeeBE = ConvertBinaryStringToIEEFloatingPointFormat(argv[1],
				"BE");
		char* ieeeLE = ConvertBinaryStringToIEEFloatingPointFormat(argv[1],
				"LE");

		if ((ieeeBE != NULL) && (ieeeBE != NULL))
			printf("BE: %s\nLE: %s", ieeeBE, ieeeLE);

		else
			fprintf(stderr, "ERROR\n");
	}

	else if (!strcmp(argv[2], "string"))
	{

		char* asciiString = ConvertBinaryStringToCharacterStringRepresentation(
				argv[1]);

		if (asciiString != NULL)
			printf("%s", asciiString);
		else
			fprintf(stderr, "ERROR\n");
	}

	return 0;
}

int IsValidInput(char* input, char* type) {

	int valid = 1;/*lets say its valid to start with*/

	if (type == "string" && strlen(input) % 8 != 0)/*string length has to be divisible by 8*/
		valid = 0;

	if ((type == "int" || type == "float") && strlen(input) != 32)
		valid = 0;

	/*Checks the input for invalid characters*/
	int i;
	for (i = 0; i < strlen(input); i++) {
		if (input[i] != '1' && input[i] != '0') {
			valid = 0;
		}
	}

	return valid;
}

char* ConvertToBigEndian(char* input) {
	char* returnString = (char*) malloc(strlen(input) * sizeof(char) + 2);/*input should be 32, but w/e*/
	strcpy(returnString, "");

/*Must be 32 bit input*/
	const int WORDSIZE = 8;
	int length = strlen(input);
	int numberOfWords = length / WORDSIZE;

	int i;
	for (i = 0; i < numberOfWords; i++) { /*Yeah!!*/
		/*strncat(returnString, (input + (length-WORDSIZE) - (WORDSIZE * i)), WORDSIZE);*/
		strncat(returnString, (input + length - (WORDSIZE * (i + 1))), WORDSIZE);
	}

	return returnString;
}

double BinToDec(char* binaryString) {

	int stringLength = strlen(binaryString);
	int sizeOfBase =sizeOfBasePart(binaryString);

	double base10sum = 0;
	int highestPower;
	int highestPow;
	int i;

		i = 0;
		highestPower = sizeOfBase - 1;
		for (i = highestPower; i >= 0; i--) {
			if (binaryString[i] == '1')
				base10sum += pow(2, (highestPower - i));
		}

		highestPow=-1;
		for(i=sizeOfBase+1;i<stringLength;i++,highestPow--)
		{
			if (binaryString[i] == '1')
				base10sum += pow(2, (highestPow));
		}

	return base10sum;

}

char* ConvertBinaryStringToCharacterStringRepresentation(char* binaryString) {
	const int SIZE_OF_CHARACTER_BLOCK = 8;
	/*Can take in any size char string*/
	int numberOfReturnCharacters = strlen(binaryString)
			/ SIZE_OF_CHARACTER_BLOCK;

	char* returnString;/*Fill with 8xs less characters*/
	returnString
			= (char*) malloc((numberOfReturnCharacters + 1) * sizeof(char));
	strcpy(returnString, "");

	/*need to break up string into blocks of 8*/
	int AsciiInt = 0;

	char* blockWeNeedToConvert = (char*) malloc((SIZE_OF_CHARACTER_BLOCK + 2)
			* sizeof(char));
	strcpy(blockWeNeedToConvert, "");

	int i;

	for (i = 0; i < numberOfReturnCharacters; i++) {
		strncpy(blockWeNeedToConvert, (binaryString + i
				* SIZE_OF_CHARACTER_BLOCK), SIZE_OF_CHARACTER_BLOCK);
		blockWeNeedToConvert[SIZE_OF_CHARACTER_BLOCK] = '\0';
		AsciiInt = BinToDec(blockWeNeedToConvert);

		if (AsciiInt == 0)/*Null terminator, just break*/
		{
			return returnString;/*Returns current string once null is reached*/
		}

		char* chArr = (char*) malloc(2 * sizeof(char));
		chArr[0] = (char) AsciiInt;
		chArr[1] = '\0';
		strcat(returnString, chArr);

	}
	return NULL;/*returns null if null terminator is not reached*/

}

char* ConvertBinaryStringToTwosComplement(char* binaryString, char* Endian) {
	int sum = 0;
	char* binStr;

	if (Endian == "BE") {
		binStr = binaryString;

	} else if (Endian == "LE") {
		binStr = ConvertToBigEndian(binaryString);
	} else
		return 0;

	int stringLength = strlen(binStr);
	char* stringPositionPtr = binStr;
	char* rep;
	char* stringRepresentation;/*What we will return*/

	int i = 0;
	int add = -1;

	while (*stringPositionPtr != '\0') {
		if (*stringPositionPtr == '1')
			sum += (pow(2, stringLength - 1 - i)) * add;

		i++;
		stringPositionPtr++;
		add = 1;

	}

	rep = ConvertIntToString(sum, 10);
	stringRepresentation = malloc((strlen(rep) + 5) * sizeof(char));

	strcpy(stringRepresentation, "");
	strcat(stringRepresentation, rep);

	return stringRepresentation;

}
char* ConvertDoubleToString(double numberToConvert, int base) {

	int mult;/*used for neg and pos*/
	long long int wholeNumber = numberToConvert;/*the whole portion..long long should do the trick*/

	if (numberToConvert < 0)
		mult = -1;
	else
		mult = 1;

	double decimalPortion = (double) (numberToConvert
			- (long long int) numberToConvert) * mult;

	char* representation;
	int* dec;

	char* hex = "0123456789ABCDEF";/*Shouldnt see the upper ones since the base is 10, but w.e */

	double calculation = wholeNumber;
	int power = 0;

	/*find how much to malloc for*/
	while (calculation * mult > base/*10*/) {
		calculation = calculation / base;
		power++;
	}
	dec = (int*) malloc(((power) + 2) * sizeof(int));

	int i = 0;
	int j = 0;

	long long int absoluteValueOfWholeNumber = numberToConvert * mult;

	while (absoluteValueOfWholeNumber > 0/*1*/) {/***********/
		dec[i] = absoluteValueOfWholeNumber % base;
		absoluteValueOfWholeNumber = absoluteValueOfWholeNumber / base;
		i++;
	}

	char* wholePortionString = (char*) malloc((power + 2) * sizeof(char));
	strcpy(wholePortionString, "");

	if (numberToConvert < 0)
		strncat(wholePortionString, "-", 1);

	char* ch = (char*) malloc(1 * sizeof(char));
	strcpy(ch, "");

	j = i - 1;
	for (i = j; j >= 0; j--) {
		int c = dec[j];
		strncat(wholePortionString, (hex + c), 1);
	}

	int res;
	long double remainder;
	i = 0;

	char* fractionalString = (char*) malloc(21 * sizeof(char));
	strcpy(fractionalString, "");

	i = 0;
	while (((float) decimalPortion / (long long int) decimalPortion) > 1) {

		decimalPortion *= base;
		res = decimalPortion;
		remainder = decimalPortion - (long long int) decimalPortion;
		decimalPortion = remainder;

		int c = res;

		strncat(fractionalString, (hex + c), 1);

		i++;
		if ((i > 19)) {
			break;
		}

	}

	fractionalString[i] = '\0';

	representation = (char*) malloc((power + 2)
			+ (strlen(fractionalString) + 1) * sizeof(char));

	strcpy(representation, "");

	strcat(representation, wholePortionString);
	strcat(representation, ".");
	if (strlen(fractionalString) == 0)
		strcat(representation, "0");
	strcat(representation, fractionalString);

	return representation;

}

char* ConvertIntToString(int numberToConvert, int base) {

	int mult;/*used for neg and pos*/
	char* representation;/*What we will return*/
	int* dec;/*our decimal array*/

	if (numberToConvert < 0)
		mult = -1;
	else
		mult = 1;

	int number = numberToConvert;
	char* hex = "0123456789ABCDEF";

	double calculation = number;
	int power = 0;

	while (calculation * mult > 10) {
		calculation = calculation / base;
		power++;
	}

	dec = (int*) malloc(((power) + 1) * sizeof(int));/*1 for buffer*/

	int i = 0;
	int j = 0;

	while (mult * number > 0) {
		dec[i] = mult * number % base;
		number = number / base;
		i++;
	}

	representation = (char*) malloc((power + 2) * sizeof(char));/*buffer*/
	strcpy(representation, "");

	if (numberToConvert < 0)
		strncat(representation, "-", 1);

	char* ch = (char*) malloc(1 * sizeof(char));
	strcpy(ch, "");

	j = i - 1;
	for (i = j; j >= 0; j--) {
		int c = dec[j];
		strncat(representation, (hex + c), 1);
	}

	return representation;
}

char* ConvertBinaryStringToIEEFloatingPointFormat(char* binaryInput,
		char* Endian) {
	char* binaryToConvert;

	if (Endian == "BE") {
		binaryToConvert = binaryInput;

	} else if (Endian == "LE") {
		binaryToConvert = ConvertToBigEndian(binaryInput);
	} else
		return 0;

	const int SIZE_OF_EXPONENT_STRING = 8;
	const int SIZE_OF_MANTISSA_STRING = 23;

	char signBit = binaryToConvert[0];
	char* exponent;
	char* mantissa;

	double decimalReturnValue;
	char* returnString;/*What we will return*/

	int normal;/* 0 for other,1 for normal, 2 for denormal*/

	int exp;
	int Exponent;/*exponent used in calculation*/
	double mant;

	exponent = (char*) malloc((SIZE_OF_EXPONENT_STRING + 1) * sizeof(char));
	strcpy(exponent, "");

	strncpy(exponent, (binaryToConvert + 1), SIZE_OF_EXPONENT_STRING);
	exponent[SIZE_OF_EXPONENT_STRING] = '\0';

	mantissa = (char*) malloc((SIZE_OF_MANTISSA_STRING + 2) * sizeof(char));/*for . and Null*/

	strcpy(mantissa, ".");
	strncat(mantissa, (binaryToConvert + 1 + SIZE_OF_EXPONENT_STRING),
			SIZE_OF_MANTISSA_STRING);
	mantissa[SIZE_OF_MANTISSA_STRING] = '\0';

	exp = BinToDec(exponent);
	mant = BinToDec(mantissa);

	if (exp == 255 && signBit == '0' && mant == .5)/*not a number*/
		normal = 0;
	else if (exp == 255 && signBit == '1' && mant == 0)/*negative infinity*/
		normal = -1;
	else if (exp == 255 && signBit == '0' && mant == 0)/*positive infinity*/
		normal = -2;
	else if (exp > 0 && mant > 0)/*normalized*/
		normal = 1;/*normalized*/

	else if (exp == 0 && mantissa > 0)/*Denormalized*/
		normal = 2;

	switch (normal) {
	case -2:/*Positive infinity*/
		return "pinf";
	case -1:/*Negative infinity*/
		return "ninf";
	case 0:/*Not a number*/
		return "NaN";
	case 1:/*Normalized*/
		mant += 1;
		Exponent = exp - 126;/*By adding 1 to the mantissa*/
	case 2:/*Denormalized*/
		Exponent = exp - 127;
	}

	decimalReturnValue = mant * pow(2, Exponent);

	if (signBit == '1')
		decimalReturnValue *= -1;
	returnString = ConvertDoubleToString(decimalReturnValue, 10);

	return returnString;
}

int sizeOfBasePart(char* input) {
	char* where = input;

	while (*(where) != '.' && *where != '\0') {
		where++;
	}

	return (where - input);
}

