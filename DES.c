typedef unsigned long long int UInt64;
typedef unsigned long int UInt32;
typedef unsigned char UInt8;

enum Side { LEFT, RIGHT };
enum Mode { ENCODE, DECODE };

UInt32 ROL28(UInt32, UInt8); //ROtate Left through carry 28-bit number
UInt32 ROR28(UInt32, UInt8); //ROtate Right through carry 28-bit number

//UInt64 Permutation(UInt8, UInt8, UInt64, UInt8[]); //Permute bits in number in conformity with permutation table

//UInt64 InitialPermutation(UInt64);
//UInt64 KeyPermutation(UInt64);
//UInt64 ShrinkPermutation(UInt64);
//UInt64 ExpansionPermutation(UInt32);
//UInt32 PBlocksPermutation(UInt32);
//UInt64 FinalPermutation(UInt64);

//UInt64 CircularShift(UInt64, UInt8, Side);

//UInt8 SBlocks(UInt8, UInt8);

//UInt64 SingleDES(UInt64, UInt64, Mode); //DES
//UInt64 TripleDES(UInt64, UInt64, UInt64); //Three times DES

UInt64 Permutation(UInt8 outputLength, UInt8 inputLength, UInt64 startSet, UInt8 sequence[])
{
	UInt64 result = 0;

	for (int i = 0; i < inputLength; i++)
	{
		result <<= 1;
		if (((startSet >> (outputLength - sequence[i])) & 1) == 1)
			result++;
	}

	return result;
}

UInt64 InitialPermutation(UInt64 startSet)
{
	UInt8 PermutationTable[64] = { 	58, 50, 42, 34, 26, 18, 10, 2,
					60, 52, 44, 36, 28, 20, 12, 4,
					62, 54, 46, 38, 30, 22, 14, 6,
					64, 56, 48, 40, 32, 24, 16, 8,
					57, 49, 41, 33, 25, 17,  9, 1,
					59, 51, 43, 35, 27, 19, 11, 3,
					61, 53, 45, 37, 29, 21, 13, 5,
					63, 55, 47, 39, 31, 23, 15, 7 };

	return Permutation(64, 64, startSet, PermutationTable);
}

UInt64 ShrinkPermutation(UInt64 startSet)
{
	UInt8 PermutationTable[48] = { 	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
					23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
					41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
					44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

	return Permutation(56, 48, startSet, PermutationTable);
}

UInt64 ExpansionPermutation(UInt32 startSet)
{
	UInt8 PermutationTable[48] = { 	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
					 8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
					16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
					24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1 };

	return Permutation(32, 48, startSet, PermutationTable);
}

UInt32 PBlocksPermutation(UInt32 startSet)
{
	UInt8 PermutationTable[32] = { 	16,  7, 20, 21, 29, 12, 28, 17,
					 1, 15, 23, 26,  5, 18, 31, 10,
					 2,  8, 24, 14, 32, 27,  3,  9,
					19, 13, 30,  6, 22, 11,  4, 25 };

	return Permutation(32, 32, startSet, PermutationTable);
}

UInt64 FinalPermutation(UInt64 startSet)
{
	UInt8 PermutationTable[64] = {  8, 40, 16, 48, 24, 56, 32, 64,
					7, 39, 15, 47, 23, 55, 31, 63,
					6, 38, 14, 46, 22, 54, 30, 62,
					5, 37, 13, 45, 21, 53, 29, 61,
					4, 36, 12, 44, 20, 52, 28, 60,
					3, 35, 11, 43, 19, 51, 27, 59,
					2, 34, 10, 42, 18, 50, 26, 58,
					1, 33,  9, 41, 17, 49, 25, 57 };

	return Permutation(64, 64, startSet, PermutationTable);
}

UInt64 CircularShift(UInt64 startSet, UInt8 roundCount, Side side)
{
	UInt8 shift;
	if ((roundCount == 1) || (roundCount == 2) || (roundCount == 9) || (roundCount == 16))
		shift = 1;
	else
		shift = 2;

	UInt32 highSet = startSet >> 28;
	UInt32 lowSet = startSet & 0xFFFFFFF;

	if (side == LEFT)
	{
		highSet = ROL28(highSet, shift);
		lowSet = ROL28(lowSet, shift);
	}
	else
	{
		highSet = ROR28(highSet, shift);
		lowSet = ROR28(lowSet, shift);
	}

	startSet = highSet;
	startSet <<= 28;
	startSet ^= lowSet;

	return startSet;
}

UInt32 ROL28(UInt32 startSet, UInt8 shift)
{
	UInt32 highSet = startSet >> (28 - shift);
	startSet <<= shift;
	startSet &= 0xFFFFFFF;

	return startSet ^ highSet;
}

UInt32 ROR28(UInt32 startSet, UInt8 shift)
{
	UInt32 highSet = startSet << (28 - shift);
	startSet >>= shift;
	highSet &= 0xFFFFFFF;

	return startSet ^ highSet;
}

UInt64 KeyPermutation(UInt64 startSet)
{
	UInt8 PermutationTable[56] = { 	57, 49, 41, 33, 25, 17,  9, 1,
					58, 50, 42, 34, 26, 18, 10, 2,
					59, 51, 43, 35, 27, 19, 11, 3,
					60, 52, 44, 36,
					63, 55, 47, 39, 31, 23, 15, 7,
					62, 54, 46, 38, 30, 22, 14, 6,
					61, 53, 45, 37, 29, 21, 13, 5,
					28, 20, 12,  4 };

	return Permutation(64, 56, startSet, PermutationTable);
}

UInt8 SBlocks(UInt8 startBits, UInt8 SBlockNumber)
{
	UInt8 SBlocksTable[8][64] = {
					{ 14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1,
					   3, 10, 10,  6,  6, 12, 12, 11,  5,  9,  9,  5,  0,  3,  7,  8,
					   4, 15,  1, 12, 14,  8,  8,  2, 13,  4,  6,  9,  2,  1, 11,  7,
					  15,  5, 12, 11,  9,  3,  7, 14,  3, 10, 10,  0,  5,  6,  0, 13 },
	
					{ 15,  3,  1, 13,  8,  4, 14,  7,  6, 15, 11,  2,  3,  8,  4, 14,
					   9, 12,  7,  0,  2,  1, 13, 10, 12,  6,  0,  9,  5, 11, 10,  5,
					   0, 13, 14,  8,  7, 10, 11,  1, 10,  3,  4, 15, 13,  4,  1,  2, 
					   5, 11,  8,  6, 12,  7,  6, 12,  9,  0,  3,  5,  2, 14, 15,  9 },

					{ 10, 13,  0,  7,  9,  0, 14,  9,  6,  3,  3,  4, 15,  6,  5, 10,
					   1,  2, 13,  8, 12,  5,  7, 14, 11, 12,  4, 11,  2, 15,  8,  1,
					  13,  1,  6, 10,  4, 13,  9,  0,  8,  6, 15,  9,  3,  8,  0,  7,
					  11,  4,  1, 15,  2, 14, 12,  3,  5, 11, 10,  5, 14,  2,  7, 12 },

					{  7, 13, 13,  8, 14, 11,  3,  5,  0,  6,  6, 15,  9,  0, 10,  3,
					   1,  4,  2,  7,  8,  2,  5, 12, 11,  1, 12, 10,  4, 14, 15,  9,
					  10,  3,  6, 15,  9,  0,  0,  6, 12, 10, 11,  1,  7, 13, 13,  8,
					  15,  9,  1,  4,  3,  5, 14, 11,  5, 12,  2,  7,  8,  2,  4, 14 },

					{  2, 14, 12, 11,  4,  2,  1, 12,  7,  4, 10,  7, 11, 13,  6,  1,
					   8,  5,  5,  0,  3, 15, 15, 10, 13,  3,  0,  9, 14,  8,  9,  6,
					   4, 11,  2,  8,  1, 12, 11,  7, 10,  1, 13, 14,  7,  2,  8, 13,
					  15,  6,  9, 15, 12,  0,  5,  9,  6, 10,  3,  4,  0,  5, 14,  3 },

					{ 12, 10,  1, 15, 10,  4, 15,  2,  9,  7,  2, 12,  6,  9,  8,  5,
					   0,  6, 13,  1,  3, 13,  4, 14, 14,  0,  7, 11,  5,  3, 11,  8,
					   9,  4, 14,  3, 15,  2,  5, 12,  2,  9,  8,  5, 12, 15,  3, 10,
					   7, 11,  0, 14,  4,  1, 10,  7,  1,  6, 13,  0, 11,  8,  6, 13 },

					{  4, 13, 11,  0,  2, 11, 14,  7, 15,  4,  0,  9,  8,  1, 13, 10,
					   3, 14, 12,  3,  9,  5,  7, 12,  5,  2, 10, 15,  6,  8,  1,  6,
					   1,  6,  4, 11, 11, 13, 13,  8, 12,  1,  3,  4,  7, 10, 14,  7,
					  10,  9, 15,  5,  6,  0,  8, 15,  0, 14,  5,  2,  9,  3,  2, 12 },

					{ 13,  1,  2, 15,  8, 13,  4,  8,  6, 10, 15,  3, 11,  7,  1,  4,
					  10, 12,  9,  5,  3,  6, 14, 11,  5,  0,  0, 14, 12,  9,  7,  2,
					   7,  2, 11,  1,  4, 14,  1,  7,  9,  4, 12, 10, 14,  8,  2, 13,
					   0, 15,  6, 12, 10,  9, 13,  0, 15,  3,  3,  5,  5,  6,  8, 11 }
				    };

	return SBlocksTable[SBlockNumber][startBits];
}

UInt64 SingleDES(UInt64 plainText, UInt64 key, Mode mode)
{
	plainText = InitialPermutation(plainText);

	key = KeyPermutation(key);

	UInt64 roundkey = 0;
	UInt32 lowPlainText = plainText & 0xFFFFFFFF;
	UInt32 highPlainText = plainText >> 32;
	
	for (UInt8 i = 1; i <= 16; i++)
	{
		if (mode == ENCODE)
			key = CircularShift(key, i, LEFT);
		else if (i != 1)
			key = CircularShift(key, i, RIGHT);

		roundkey = ShrinkPermutation(key);

		UInt64 roundLowPlainText = ExpansionPermutation(lowPlainText);

		roundLowPlainText ^= roundkey;

		UInt32 SBlocksResult= 0;

		for (UInt8 j = 0; j <= 7; j++)
		{
			UInt8 inputBits = roundLowPlainText & 0x3F;
			roundLowPlainText >>= 6;
			UInt32 SBlockValue = SBlocks(inputBits, 7 - j);
			SBlockValue <<= 4 * j;
			SBlocksResult ^= SBlockValue;
		}

		SBlocksResult = PBlocksPermutation(SBlocksResult);
		
		SBlocksResult ^= highPlainText;
		highPlainText = lowPlainText;
		lowPlainText = SBlocksResult;
	}

	plainText = highPlainText;
	plainText <<= 32;
	plainText ^= lowPlainText;
	plainText = FinalPermutation(plainText);

	return plainText;
}

UInt64 TripleDES(UInt64 plainText, UInt64 key1, UInt64 key2)
{
	plainText = SingleDES(plainText, key1, ENCODE);
	plainText = SingleDES(plainText, key2, DECODE);
	plainText = SingleDES(plainText, key1, ENCODE);

	return plainText;
}
