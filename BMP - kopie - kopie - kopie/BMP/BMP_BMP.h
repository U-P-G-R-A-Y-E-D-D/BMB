#pragma once


#ifndef _BMP_BMP_h_
#define _BMP_BMP_h_

bool SafeFread(char* buffer, int size, int number, FILE* fp);
bool EasyBMPcheckDataSize(void);

class BMP
{
private:

	int BitHloubka;
	int Width;
	int Height;
	RGBApixel** Pixels;
	RGBApixel* Colors;
	int XPelsPerMeter;
	int YPelsPerMeter;

	bmpBYTE* MetaData1;
	int SizeOfMetaData1;
	bmpBYTE* MetaData2;
	int SizeOfMetaData2;

	bool Read32bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Read24bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Read8bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Read4bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Read1bitRow(bmpBYTE* Buffer, int BufferSize, int Row);

	bool Write32bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Write24bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Write8bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Write4bitRow(bmpBYTE* Buffer, int BufferSize, int Row);
	bool Write1bitRow(bmpBYTE* Buffer, int BufferSize, int Row);

	bmpBYTE FindClosestColor(RGBApixel& input);

public:

	int TellBitHloubka(void);
	int TellWidth(void);
	int TellHeight(void);
	int TellNumberOfColors(void);
	void SetDPI(int HorizontalDPI, int VerticalDPI);
	int TellVerticalDPI(void);
	int TellHorizontalDPI(void);

	BMP();
	BMP(BMP& Input);
	~BMP();
	RGBApixel* operator()(int i, int j);

	RGBApixel GetPixel(int i, int j) const;
	bool SetPixel(int i, int j, RGBApixel NewPixel);

	bool CreateStandardColorTable(void);

	bool SetSize(int NewWidth, int NewHeight);
	bool SetBitHloubka(int NewHloubka);
	bool WriteToFile(const char* FileName);
	bool ReadFromFile(const char* FileName);

	RGBApixel GetColor(int ColorNumber);
	bool SetColor(int ColorNumber, RGBApixel NewColor);
};

#endif