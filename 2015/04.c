/*
--- Day 4: The Ideal Stocking Stuffer ---
Santa needs help mining some AdventCoins (very similar to bitcoins) to use as gifts for all the economically forward-thinking little girls and boys.

To do this, he needs to find MD5 hashes which, in hexadecimal, start with at least five zeroes. The input to the MD5 hash is some secret key (your puzzle input, given below) followed by a number in decimal. To mine AdventCoins, you must find Santa the lowest positive number (no leading zeroes: 1, 2, 3, ...) that produces such a hash.

For example:

If your secret key is abcdef, the answer is 609043, because the MD5 hash of abcdef609043 starts with five zeroes (000001dbbfa...), and it is the lowest such number to do so.
If your secret key is pqrstuv, the lowest number it combines with to make an MD5 hash starting with five zeroes is 1048970; that is, the MD5 hash of pqrstuv1048970 looks like 000006136ef....
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define BASE_16_ASCII     "0123456789abcdef"
#define BASE_16_ASCII_UP  "0123456789abcdef"
#define BASE_10_ASCII     "0123456789"
#define BASE_BIN_ASCII    "01"

#define BASE_16_PREFIX      "0X"
#define BASE_16_PREFIX_UP   "0x"
#define BASE_BIN_PREFIX     "0b"

int is_base_valid(char *base)
{
	int	i;
	int	j;

	i = 0;
	while (base[i]) {
		j = i + 1;
		while (base[j]) {
			if (base[j] == base[i])
				return (0);
			j++;
		}
		i++;
	}
	return (i);
}

int digit_length_base(long num, int base)
{
	int n = 0;
	if (num < 0)
	{
		num = -num;
		n++;
	}
	if (num == 0)
		return (1);

	while (num >= 0)
	{
		if (num == 0)
			return (n);
		num /= base;
		n++;
	}
	return (n);
}

char  *np_itoa_base(int i, char *base)
{
	uint64_t num, blength;
	char *asci;
	int nsz;

	if (base == NULL)
		base = BASE_10_ASCII;  
	blength = is_base_valid(base);
	if (blength < 2)
		return strdup("");
	if (i == 0)
		return strdup("0");
	num = i;
	nsz = digit_length_base(num, blength);
	if (i < 0)
		num = -num;
	asci = malloc(nsz + 1);
	asci[nsz--] = 0;
	while (num > 0) {
		if (num == 0)
			break;
		asci[nsz--] = 
			base[(num % blength)];
		num /= blength;
	}
	if (i < 0)
		asci[nsz] = '-';
	return (asci);
}
typedef union uwb {
	unsigned w;
	unsigned char b[4];
} MD5union;

typedef unsigned DigestArray[4];

unsigned func0(unsigned abcd[]) {
	return (abcd[1] & abcd[2]) | (~abcd[1] & abcd[3]);
}

unsigned func1(unsigned abcd[]) {
	return (abcd[3] & abcd[1]) | (~abcd[3] & abcd[2]);
}

unsigned func2(unsigned abcd[]) {
	return  abcd[1] ^ abcd[2] ^ abcd[3];
}

unsigned func3(unsigned abcd[]) {
	return abcd[2] ^ (abcd[1] | ~abcd[3]);
}

typedef unsigned(*DgstFctn)(unsigned a[]);

unsigned *calctable(unsigned *k)
{
	double s, pwr;
	int i;

	pwr = pow(2.0, 32);
	for (i = 0; i<64; i++) {
		s = fabs(sin(1.0 + i));
		k[i] = (unsigned)(s * pwr);
	}
	return k;
}

unsigned rol(unsigned r, short N)
{
	unsigned  mask1 = (1 << N) - 1;
	return ((r >> (32 - N)) & mask1) | ((r << N) & ~mask1);
}

unsigned* Algorithms_Hash_MD5(const char *msg, int mlen)
{
	static DigestArray h0 = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };
	static DgstFctn ff[] = { &func0, &func1, &func2, &func3 };
	static short M[] = { 1, 5, 3, 7 };
	static short O[] = { 0, 1, 5, 0 };
	static short rot0[] = { 7, 12, 17, 22 };
	static short rot1[] = { 5, 9, 14, 20 };
	static short rot2[] = { 4, 11, 16, 23 };
	static short rot3[] = { 6, 10, 15, 21 };
	static short *rots[] = { rot0, rot1, rot2, rot3 };
	static unsigned kspace[64];
	static unsigned *k;

	static DigestArray h;
	DigestArray abcd;
	DgstFctn fctn;
	short m, o, g;
	unsigned f;
	short *rotn;
	union {
		unsigned w[16];
		char     b[64];
	}mm;
	int os = 0;
	int grp, grps, q, p;
	unsigned char *msg2;

	if (k == NULL) k = calctable(kspace);

	for (q = 0; q<4; q++) h[q] = h0[q];

	{
		grps = 1 + (mlen + 8) / 64;
		msg2 = (unsigned char*)malloc(64 * grps);
		memcpy(msg2, msg, mlen);
		msg2[mlen] = (unsigned char)0x80;
		q = mlen + 1;
		while (q < 64 * grps) { msg2[q] = 0; q++; }
		{
			MD5union u;
			u.w = 8 * mlen;
			q -= 8;
			memcpy(msg2 + q, &u.w, 4);
		}
	}

	for (grp = 0; grp<grps; grp++)
	{
		memcpy(mm.b, msg2 + os, 64);
		for (q = 0; q<4; q++) abcd[q] = h[q];
		for (p = 0; p<4; p++) {
			fctn = ff[p];
			rotn = rots[p];
			m = M[p]; o = O[p];
			for (q = 0; q<16; q++) {
				g = (m*q + o) % 16;
				f = abcd[1] + rol(abcd[0] + fctn(abcd) + k[q + 16 * p] + mm.w[g], rotn[q % 4]);

				abcd[0] = abcd[3];
				abcd[3] = abcd[2];
				abcd[2] = abcd[1];
				abcd[1] = f;
			}
		}
		for (p = 0; p<4; p++)
			h[p] += abcd[p];
		os += 64;
	}
	return h;
}

char* GetMD5String(const char *msg, int mlen) {
	char *str = (char *)malloc(40);
	strcpy(str, "");
	int j;
	unsigned *d = Algorithms_Hash_MD5(msg, mlen);
	MD5union u;
	char* s = (char *)malloc(100 * sizeof(char));

	for (j = 0; j<4; j++) {
		u.w = d[j];
		memset(s, 0, 100);
		sprintf(s, "%02x%02x%02x%02x", u.b[0], u.b[1], u.b[2], u.b[3]);
		strcat(str, s);
	}
	free(s);
	return str;
}
// #define FIVE_ZEROS 5
#define SIX_ZEROS  6

int main(int argc, char **argv)
{

	if (argc < 2) {
		fprintf(stderr, "An input file was not given.\n");
		return (1);
	}
	FILE *fp = fopen(argv[1], "r");

	if (!fp) {
		fprintf(stderr, "Could not open `%s` for reading.\n", argv[1]);
		return (1);
	}

	char *last_addr;
	unsigned long number = 0;
	char *num_ascii = NULL;
	ssize_t nread = 1;
	size_t n = 0;
	char *Line = NULL;

	while (1) {
		nread = getline(&Line, &n, fp);
		if (nread <= 0)
			break;
		Line[nread-1] = 0;

		while (1) {

			num_ascii = np_itoa_base(number, NULL); // NULL FOR BASE 10
			last_addr = Line + nread - 1;
			Line = strcat(Line, num_ascii);
			char *r = GetMD5String(Line, nread-1);
			printf("STRING: %s\n", Line);
			printf("MD5: %s\n", r); 
#ifdef SIX_ZEROS
			if (strncmp(r, "000000", 6) == 0) {
				printf("FOUND: %s\n", Line);
				printf("HASH: %s\n", r);
				free(r);
				break;
			}
#else
			if (strncmp(r, "00000", 5) == 0) {
				printf("FOUND: %s\n", Line);
				printf("HASH: %s\n", r);
				free(r);
				break;
			}
#endif /* ifdef SIX_ZEROS  */ 
			*last_addr = 0;
			number++;
			free(r);
			// sleep(1);
		}
		free(Line);
		Line = NULL;
	}
	// printf("visited_houses_: %zu\n", FinalM.size());
	return (0);
}
