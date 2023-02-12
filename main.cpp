/* Solution to course project # 6
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Emilian Yanev
* @idnumber 0MI0600273* @compiler GCC
*
* 
*/

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

void mySwap (char& a, char& b)
{
	char temp = a;
	a = b;
	b = temp;
}

int myStrLen(char* s)
{
	int count = 0;
	while (s[count] != '\0')
	{
		count++;
	}
	return count;
}

int stringIndexLenght(unsigned char hashIndex)
{
	int cast = hashIndex;
	int count = 0;
	while (cast != 0)
	{
		count++;
		cast = cast / 10;
	}
	return count;
}

void decToHexaDec(unsigned char hashIndex)
{
	int cast = hashIndex;
	char arrHex[3];
	int arrIndex = 0;

	while (cast != 0)
	{
		int remainder = 0;
		char storeDigit;

		remainder = cast % 16;

		if (remainder < 10) {
			storeDigit = remainder + 48;
		}
		else {
			storeDigit = remainder + 55;
		}

		arrHex[arrIndex] = storeDigit;
		arrIndex++;
		cast = cast / 16;
	}
	arrHex[arrIndex] = '\0';
	int j = myStrLen(arrHex) - 1;
	int i = 0;

	while (i <= j)
	{
		mySwap(arrHex[i], arrHex[j]);
		i++;
		j--;
	}

	for (int i = 0; i < myStrLen(arrHex); i++)
	{
		cout << arrHex[i];
	}
}

#define uchar unsigned char
#define uint unsigned int

#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b)))) //define left bitwise rotation
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b)))) //define right bitwise rotation

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z))) //define "CHOOSE" if bit of x is 0 - choose the corresponding bit of z, if bit of x is 1 - choose the corresponding bit of y 
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z))) // define "MAJORITY" - the output from the majority function is determined by the number of equal bits in each column of numbers x,y,z in bits
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22)) //defined fixed functions
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25)) //defined fixed functions
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3)) //defined fixed functions
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10)) //defined fixed functions

uint InitConst[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
}; //constants of the first 32 bits of the fractional parts of the cubic root of the first 64 primes

void SHA256Init(uchar data[64], uint datalen, uint bitlen[2], uint state[8])
{
	datalen = 0;
	bitlen[0] = 0;
	bitlen[1] = 0;
	state[0] = 0x6a09e667;
	state[1] = 0xbb67ae85;
	state[2] = 0x3c6ef372;
	state[3] = 0xa54ff53a;
	state[4] = 0x510e527f;
	state[5] = 0x9b05688c;
	state[6] = 0x1f83d9ab;
	state[7] = 0x5be0cd19;
} //"state" array is filled with the fractional part of the first 32 bits of the first 8 primes

void SHA256Transform(uchar data[64], uint &datalen, uint bitlen[2], uint* state)
{
	uint a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]); //calculate first 16 "words"
	for (; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16]; //calculate remaining 48 "words"

	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	f = state[5];
	g = state[6];
	h = state[7];

	for (i = 0; i < 64; ++i) { //compression loop
		t1 = h + EP1(e) + CH(e, f, g) + InitConst[i] + m[i]; //t1 formula
		t2 = EP0(a) + MAJ(a, b, c); //t2 formula
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	state[0] += a; //sum constants with calculated values
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

void SHA256Update(uchar data[64], uint& datalen, uint bitlen[2], uint* state, uchar dataarg[], uint len)
{
	for (uint i = 0; i < len; ++i) {
		data[datalen] = dataarg[i];
		datalen++;
		if (datalen == 64) {
			SHA256Transform(data, datalen, bitlen, state);
			DBL_INT_ADD(bitlen[0], bitlen[1], 512);
			datalen = 0;
		}
	}
}

void SHA256Final(uchar data[64], uint datalen, uint bitlen[2], uint state[8], uchar hash[])
{
	uint i = datalen;

	if (datalen < 56) {
		data[i++] = 0x80; //128

		while (i < 56)
			data[i++] = 0x00; //0
	}
	else {
		data[i++] = 0x80; //128

		while (i < 64)
			data[i++] = 0x00; // 0

		SHA256Transform(data,datalen,bitlen,state);
	}

	DBL_INT_ADD(bitlen[0], bitlen[1], datalen * 8);
	data[63] = bitlen[0];
	data[62] = bitlen[0] >> 8;
	data[61] = bitlen[0] >> 16;
	data[60] = bitlen[0] >> 24;
	data[59] = bitlen[1];
	data[58] = bitlen[1] >> 8;
	data[57] = bitlen[1] >> 16;
	data[56] = bitlen[1] >> 24;
	SHA256Transform(data,datalen,bitlen,state);

	for (i = 0; i < 4; ++i) {
		hash[i] = (state[0] >> (24 - i * 8)) 
		hash[i + 4] = (state[1] >> (24 - i * 8)) 
		hash[i + 8] = (state[2] >> (24 - i * 8)) 
		hash[i + 12] = (state[3] >> (24 - i * 8)) 
		hash[i + 16] = (state[4] >> (24 - i * 8))
		hash[i + 20] = (state[5] >> (24 - i * 8)) 
		hash[i + 24] = (state[6] >> (24 - i * 8)) 
		hash[i + 28] = (state[7] >> (24 - i * 8))
	}
}

void SHA256(char* dataarg) {
	 int strLen = strlen(dataarg);
	 
	uchar data[64];
    uint datalen = 0;
    uint bitlen[2];
    uint state[8];

	 unsigned char hash[32];
	
	SHA256Init(data, datalen, bitlen, state); // initialize arrays
	SHA256Update(data, datalen, bitlen, state, (unsigned char*)dataarg, strLen); //place the input string into array
	SHA256Final(data, datalen, bitlen, state, hash); //transform string into hashed value and store it into array

char s[3];
	for (int i = 0; i < 32; i++) 
	{
		decToHexaDec(hash[i]); //print transformed string in hexadecimal
	}
}

int main(int argc, char* argv[])
{	

	if (argc < 2) {
		cout << "Error: No file path provided" << endl;
		return 1;
	} 
	ifstream file(argv[1]);

	if (!file) {
		cout << "Error: Could not open file" << endl;
		return 1;
	}

	file.seekg(0, file.end);
	int size = file.tellg();
	file.seekg(0, file.beg);

	char* fileContent = new char[size];

	file.read(fileContent, size);
	
	file.close();

	fileContent[size] = '\0';
	
	SHA256(fileContent);
	return 0;
}


