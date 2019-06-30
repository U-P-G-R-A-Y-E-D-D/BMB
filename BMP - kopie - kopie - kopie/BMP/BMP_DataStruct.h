#pragma once


#ifndef _BMP_Custom_Math_Functions_
#define _BMP_Custom_Math_Functions_
inline double Square(double number)
{
	return number*number;
}

inline int IntSquare(int number)
{
	return number*number;
}
#endif

int IntPow(int base, int exponent);

#ifndef _BMP_Defined_WINGDI
#define _BMP_Defined_WINGDI
typedef unsigned char  bmpBYTE;
typedef unsigned short bmpWORD;
typedef unsigned int  bmpDWORD;
#endif

#ifndef _BMP_DataStructures_h_
#define _BMP_DataStructures_h_

inline bool IsBigEndian()
{
	short word = 0x0001;
	if ((*(char *)& word) != 0x01)
	{
		return true;
	}
	return false;
}

inline bmpWORD FlipWORD(bmpWORD in)
{
	return ((in >> 8) | (in << 8));
}

inline bmpDWORD FlipDWORD(bmpDWORD in)
{
	return (((in & 0xFF000000) >> 24) || ((in & 0x000000FF) << 24) ||
		((in & 0x00FF0000) >> 8) || ((in & 0x0000FF00) << 8));
}

// je jednodušší použít struktur než tøídu
// protože mùžeme èíst / zapisovat všechny ètyøi bajty
// najednou (jak mùžeme poèítat s tím, že jsou spojité
// v pameti

typedef struct RGBApixel {
	bmpBYTE Blue;
	bmpBYTE Green;
	bmpBYTE Red;
	bmpBYTE Alpha;
} RGBApixel;

class BMFH {
public:
	bmpWORD  bfType;
	bmpDWORD bfSize;
	bmpWORD  bfReserved1;
	bmpWORD  bfReserved2;
	bmpDWORD bfOffBits;

	BMFH();
	void display(void);
	void SwitchEndianess(void);
};

class BMIH {
public:
	bmpDWORD biSize;
	bmpDWORD biWidth;
	bmpDWORD biHeight;
	bmpWORD  biPlanes;
	bmpWORD  biBitCount;
	bmpDWORD biCompression;
	bmpDWORD biSizeImage;
	bmpDWORD biXPelsPerMeter;
	bmpDWORD biYPelsPerMeter;
	bmpDWORD biClrUsed;
	bmpDWORD biClrImportant;

	BMIH();
	void display(void);
	void SwitchEndianess(void);
};

#endif