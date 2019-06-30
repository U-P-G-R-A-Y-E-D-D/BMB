

#include "BMP.h"

int IntPow(int baze, int exponent)
{
	int i;
	int output = 1;
	for (i = 0; i < exponent; i++)
	{
		output *= baze;
	}
	return output;
}

BMFH::BMFH()
{
	bfType = 19778;
	bfReserved1 = 0;
	bfReserved2 = 0;
}

void BMFH::SwitchEndianess(void)
{
	bfType = FlipWORD(bfType);
	bfSize = FlipDWORD(bfSize);
	bfReserved1 = FlipWORD(bfReserved1);
	bfReserved2 = FlipWORD(bfReserved2);
	bfOffBits = FlipDWORD(bfOffBits);
	return;
}

BMIH::BMIH()
{
	biPlanes = 1;
	biCompression = 0;
	biXPelsPerMeter = DefaultXPelsPerMeter;
	biYPelsPerMeter = DefaultYPelsPerMeter;
	biClrUsed = 0;
	biClrImportant = 0;
}

void BMIH::SwitchEndianess(void)
{
	biSize = FlipDWORD(biSize);
	biWidth = FlipDWORD(biWidth);
	biHeight = FlipDWORD(biHeight);
	biPlanes = FlipWORD(biPlanes);
	biBitCount = FlipWORD(biBitCount);
	biCompression = FlipDWORD(biCompression);
	biSizeImage = FlipDWORD(biSizeImage);
	biXPelsPerMeter = FlipDWORD(biXPelsPerMeter);
	biYPelsPerMeter = FlipDWORD(biYPelsPerMeter);
	biClrUsed = FlipDWORD(biClrUsed);
	biClrImportant = FlipDWORD(biClrImportant);
	return;
}

void BMIH::display(void)
{
	using namespace std;
	cout << "biSize: " << (int)biSize << endl
		<< "biWidth: " << (int)biWidth << endl
		<< "biHeight: " << (int)biHeight << endl
		<< "biPlanes: " << (int)biPlanes << endl
		<< "biBitCount: " << (int)biBitCount << endl
		<< "biCompression: " << (int)biCompression << endl
		<< "biSizeImage: " << (int)biSizeImage << endl
		<< "biXPelsPerMeter: " << (int)biXPelsPerMeter << endl
		<< "biYPelsPerMeter: " << (int)biYPelsPerMeter << endl
		<< "biClrUsed: " << (int)biClrUsed << endl
		<< "biClrImportant: " << (int)biClrImportant << endl << endl;
}

void BMFH::display(void)
{
	using namespace std;
	cout << "bfType: " << (int)bfType << endl
		<< "bfSize: " << (int)bfSize << endl
		<< "bfReserved1: " << (int)bfReserved1 << endl
		<< "bfReserved2: " << (int)bfReserved2 << endl
		<< "bfOffBits: " << (int)bfOffBits << endl << endl;
}

/* Tyto funkce jsou definovány v BMP_BMP.h */

RGBApixel BMP::GetPixel(int i, int j) const
{
	using namespace std;
	bool Warn = false;
	if (i >= Width)
	{
		i = Width - 1; Warn = true;
	}
	if (i < 0)
	{
		i = 0; Warn = true;
	}
	if (j >= Height)
	{
		j = Height - 1; Warn = true;
	}
	if (j < 0)
	{
		j = 0; Warn = true;
	}

	return Pixels[i][j];
}

bool BMP::SetPixel(int i, int j, RGBApixel NewPixel)
{
	Pixels[i][j] = NewPixel;
	return true;
}


bool BMP::SetColor(int ColorNumber, RGBApixel NewColor)
{
	using namespace std;
	if (BitHloubka != 1 && BitHloubka != 4 && BitHloubka != 8)
	{

		return false;
	}
	if (!Colors)
	{

		return false;
	}
	if (ColorNumber >= TellNumberOfColors())
	{
		
		return false;
	}
	Colors[ColorNumber] = NewColor;
	return true;
}

// RGBApixel BMP::GetColor( int ColorNumber ) const
// použitím const prestava program fungovat
RGBApixel BMP::GetColor(int ColorNumber)
{
	RGBApixel Output;
	Output.Red = 255;
	Output.Green = 255;
	Output.Blue = 255;
	Output.Alpha = 0;

	using namespace std;
	if (BitHloubka != 1 && BitHloubka != 4 && BitHloubka != 8)
	{

		return Output;
	}
	if (!Colors)
	{

		return Output;
	}
	if (ColorNumber >= TellNumberOfColors())
	{

		return Output;
	}
	Output = Colors[ColorNumber];
	return Output;
}

BMP::BMP()
{
	Width = 1;
	Height = 1;
	BitHloubka = 24;
	Pixels = new RGBApixel*[Width];
	Pixels[0] = new RGBApixel[Height];
	Colors = NULL;

	XPelsPerMeter = 0;
	YPelsPerMeter = 0;

	MetaData1 = NULL;
	SizeOfMetaData1 = 0;
	MetaData2 = NULL;
	SizeOfMetaData2 = 0;
}

// BMP::BMP( const BMP& Input )
BMP::BMP(BMP& Input)
{
	// nejprve snímek vyprázdnìte.

	Width = 1;
	Height = 1;
	BitHloubka = 24;
	Pixels = new RGBApixel*[Width];
	Pixels[0] = new RGBApixel[Height];
	Colors = NULL;
	XPelsPerMeter = 0;
	YPelsPerMeter = 0;

	MetaData1 = NULL;
	SizeOfMetaData1 = 0;
	MetaData2 = NULL;
	SizeOfMetaData2 = 0;

	// nyní nastavte správnou hloubku bitù

	SetBitHloubka(Input.TellBitHloubka());

	// nastavte správnou velikost pixelu 

	SetSize(Input.TellWidth(), Input.TellHeight());

	// nastavte informace DPI z nabídky Vstup
	//DPI urcuje rozliseni obrázku

	SetDPI(Input.TellHorizontalDPI(), Input.TellVerticalDPI());

	// pokud je tabulka barev, získáte všechny barvy

	if (BitHloubka == 1 || BitHloubka == 4 ||
		BitHloubka == 8)
	{
		for (int k = 0; k < TellNumberOfColors(); k++)
		{
			SetColor(k, Input.GetColor(k));
		}
	}

	// získat všechny pixely

	for (int j = 0; j < Height; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			Pixels[i][j] = *Input(i, j);

		}
	}
}

BMP::~BMP()
{
	int i;
	for (i = 0; i<Width; i++) {
		delete[] Pixels[i];
	}

	delete[] Pixels;

	if (Colors) {
		delete[] Colors;
	}

	if (MetaData1) {
		delete[] MetaData1;
	}

	if (MetaData2) {
		delete[] MetaData2;
	}
}

RGBApixel* BMP::operator()(int i, int j)
{
	using namespace std;
	bool Warn = false;
	if (i >= Width)
	{
		i = Width - 1; Warn = true;
	}
	if (i < 0)
	{
		i = 0; Warn = true;
	}
	if (j >= Height)
	{
		j = Height - 1; Warn = true;
	}
	if (j < 0)
	{
		j = 0; Warn = true;
	}

	return &(Pixels[i][j]);
}


int BMP::TellBitHloubka(void)
{
	return BitHloubka;
}


int BMP::TellHeight(void)
{
	return Height;
}

// int BMP::TellWidth( void ) const
int BMP::TellWidth(void)
{
	return Width;
}
// int BMP::TellNumberOfColors( void ) const

int BMP::TellNumberOfColors(void)
{
	int output = IntPow(2, BitHloubka);
	if (BitHloubka == 32)
	{
		output = IntPow(2, 24);
	}
	return output;
}

bool BMP::SetBitHloubka(int NewHloubka)
{
	using namespace std;
	if (NewHloubka != 1 && NewHloubka != 4 &&
		NewHloubka != 8 && NewHloubka != 16 &&
		NewHloubka != 24 && NewHloubka != 32)
	{

		return false;
	}

	BitHloubka = NewHloubka;
	if (Colors)
	{
		delete[] Colors;
	}
	int NumberOfColors = IntPow(2, BitHloubka);
	if (BitHloubka == 1 || BitHloubka == 4 || BitHloubka == 8)
	{
		Colors = new RGBApixel[NumberOfColors];
	}
	else
	{
		Colors = NULL;
	}
	if (BitHloubka == 1 || BitHloubka == 4 || BitHloubka == 8)
	{
		CreateStandardColorTable();
	}

	return true;
}

bool BMP::SetSize(int NewWidth, int NewHeight)
{
	using namespace std;
	if (NewWidth <= 0 || NewHeight <= 0)
	{

		return false;
	}

	int i, j;

	for (i = 0; i<Width; i++)
	{
		delete[] Pixels[i];
	}
	delete[] Pixels;

	Width = NewWidth;
	Height = NewHeight;
	Pixels = new RGBApixel*[Width];

	for (i = 0; i<Width; i++)
	{
		Pixels[i] = new RGBApixel[Height];
	}

	for (i = 0; i < Width; i++)
	{
		for (j = 0; j < Height; j++)
		{
			Pixels[i][j].Red = 255;
			Pixels[i][j].Green = 255;
			Pixels[i][j].Blue = 255;
			Pixels[i][j].Alpha = 0;
		}
	}

	return true;
}

//zápis do vystupniho souboru


bool BMP::WriteToFile(const char* FileName)
{
	using namespace std;
	

	FILE* fp = fopen(FileName, "wb");
	if (fp == NULL)
	{

		fclose(fp);
		return false;
	}

	// nìkteré pøedbìžné otázky

	double dBytesPerPixel = ((double)BitHloubka) / 8.0;
	double dBytesPerRow = dBytesPerPixel * (Width + 0.0);
	dBytesPerRow = ceil(dBytesPerRow);

	int BytePaddingPerRow = 4 - ((int)(dBytesPerRow)) % 4;
	if (BytePaddingPerRow == 4)
	{
		BytePaddingPerRow = 0;
	}

	double dActualBytesPerRow = dBytesPerRow + BytePaddingPerRow;

	double dTotalPixelBytes = Height * dActualBytesPerRow;

	double dPaletteSize = 0;
	if (BitHloubka == 1 || BitHloubka == 4 || BitHloubka == 8)
	{
		dPaletteSize = IntPow(2, BitHloubka)*4.0;
	}

	// necha prostor pro 16bitové masky
	if (BitHloubka == 16)
	{
		dPaletteSize = 3 * 4;
	}

	double dTotalFileSize = 14 + 40 + dPaletteSize + dTotalPixelBytes;

	//napiše hlavièku souboru

	BMFH bmfh;
	bmfh.bfSize = (bmpDWORD)dTotalFileSize;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = (bmpDWORD)(14 + 40 + dPaletteSize);

	if (IsBigEndian())
	{
		bmfh.SwitchEndianess();
	}

	fwrite((char*) &(bmfh.bfType), sizeof(bmpWORD), 1, fp);
	fwrite((char*) &(bmfh.bfSize), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmfh.bfReserved1), sizeof(bmpWORD), 1, fp);
	fwrite((char*) &(bmfh.bfReserved2), sizeof(bmpWORD), 1, fp);
	fwrite((char*) &(bmfh.bfOffBits), sizeof(bmpDWORD), 1, fp);

	// napište záhlaví info 

	BMIH bmih;
	bmih.biSize = 40;
	bmih.biWidth = Width;
	bmih.biHeight = Height;
	bmih.biPlanes = 1;
	bmih.biBitCount = BitHloubka;
	bmih.biCompression = 0;
	bmih.biSizeImage = (bmpDWORD)dTotalPixelBytes;
	if (XPelsPerMeter)
	{
		bmih.biXPelsPerMeter = XPelsPerMeter;
	}
	else
	{
		bmih.biXPelsPerMeter = DefaultXPelsPerMeter;
	}
	if (YPelsPerMeter)
	{
		bmih.biYPelsPerMeter = YPelsPerMeter;
	}
	else
	{
		bmih.biYPelsPerMeter = DefaultYPelsPerMeter;
	}

	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	// znamená, že budeme používat bitové pole pro 16bitové soubory
	if (BitHloubka == 16)
	{
		bmih.biCompression = 3;
	}

	if (IsBigEndian())
	{
		bmih.SwitchEndianess();
	}

	fwrite((char*) &(bmih.biSize), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biWidth), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biHeight), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biPlanes), sizeof(bmpWORD), 1, fp);
	fwrite((char*) &(bmih.biBitCount), sizeof(bmpWORD), 1, fp);
	fwrite((char*) &(bmih.biCompression), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biSizeImage), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biXPelsPerMeter), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biYPelsPerMeter), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biClrUsed), sizeof(bmpDWORD), 1, fp);
	fwrite((char*) &(bmih.biClrImportant), sizeof(bmpDWORD), 1, fp);

	//napište paletu
	if (BitHloubka == 1 || BitHloubka == 4 || BitHloubka == 8)
	{
		int NumberOfColors = IntPow(2, BitHloubka);

		// pokud není žádná paleta, vytvoøte ji
		if (!Colors)
		{
			if (!Colors)
			{
				Colors = new RGBApixel[NumberOfColors];
			}
			CreateStandardColorTable();
		}

		int n;
		for (n = 0; n < NumberOfColors; n++)
		{
			fwrite((char*) &(Colors[n]), 4, 1, fp);
		}
	}

	// napište pixely
	int i, j;
	if (BitHloubka != 16)
	{
		bmpBYTE* Buffer;
		int BufferSize = (int)((Width*BitHloubka) / 8.0);
		while (8 * BufferSize < Width*BitHloubka)
		{
			BufferSize++;
		}
		while (BufferSize % 4)
		{
			BufferSize++;
		}

		Buffer = new bmpBYTE[BufferSize];
		for (j = 0; j < BufferSize; j++)
		{
			Buffer[j] = 0;
		}

		j = Height - 1;

		while (j > -1)
		{
			bool Success = false;
			if (BitHloubka == 32)
			{
				Success = Write32bitRow(Buffer, BufferSize, j);
			}
			if (BitHloubka == 24)
			{
				Success = Write24bitRow(Buffer, BufferSize, j);
			}
			if (BitHloubka == 8)
			{
				Success = Write8bitRow(Buffer, BufferSize, j);
			}
			if (BitHloubka == 4)
			{
				Success = Write4bitRow(Buffer, BufferSize, j);
			}
			if (BitHloubka == 1)
			{
				Success = Write1bitRow(Buffer, BufferSize, j);
			}
			if (Success)
			{
				int BytesWritten = (int)fwrite((char*)Buffer, 1, BufferSize, fp);
				if (BytesWritten != BufferSize)
				{
					Success = false;
				}
			}
			if (!Success)
			{

				j = -1;
			}
			j--;
		}

		delete[] Buffer;
	}

	if (BitHloubka == 16)
	{
		// napište bitové masky

		bmpWORD BlueMask = 31;    // bits 12-16
		bmpWORD GreenMask = 2016; // bits 6-11
		bmpWORD RedMask = 63488;  // bits 1-5
		bmpWORD ZeroWORD;

		if (IsBigEndian())
		{
			RedMask = FlipWORD(RedMask);
		}
		fwrite((char*)&RedMask, 2, 1, fp);
		fwrite((char*)&ZeroWORD, 2, 1, fp);

		if (IsBigEndian())
		{
			GreenMask = FlipWORD(GreenMask);
		}
		fwrite((char*)&GreenMask, 2, 1, fp);
		fwrite((char*)&ZeroWORD, 2, 1, fp);

		if (IsBigEndian())
		{
			BlueMask = FlipWORD(BlueMask);
		}
		fwrite((char*)&BlueMask, 2, 1, fp);
		fwrite((char*)&ZeroWORD, 2, 1, fp);

		int DataBytes = Width * 2;
		int PaddingBytes = (4 - DataBytes % 4) % 4;

		// napište skuteèné pixely

		for (j = Height - 1; j >= 0; j--)
		{
			// zapište všechny údaje obrazových øádkù
			i = 0;
			int WriteNumber = 0;
			while (WriteNumber < DataBytes)
			{
				bmpWORD TempWORD;

				bmpWORD RedWORD = (bmpWORD)((Pixels[i][j]).Red / 8);
				bmpWORD GreenWORD = (bmpWORD)((Pixels[i][j]).Green / 4);
				bmpWORD BlueWORD = (bmpWORD)((Pixels[i][j]).Blue / 8);

				TempWORD = (RedWORD << 11) + (GreenWORD << 5) + BlueWORD;
				if (IsBigEndian())
				{
					TempWORD = FlipWORD(TempWORD);
				}

				fwrite((char*)&TempWORD, 2, 1, fp);
				WriteNumber += 2;
				i++;
			}
			// write any necessary row padding
			WriteNumber = 0;
			while (WriteNumber < PaddingBytes)
			{
				bmpBYTE TempBYTE;
				fwrite((char*)&TempBYTE, 1, 1, fp);
				WriteNumber++;
			}
		}

	}

	fclose(fp);
	return true;
}





bool BMP::ReadFromFile(const char* FileName)
{
	using namespace std;
	

	FILE* fp = fopen(FileName, "rb");
	if (fp == NULL)
	{

		SetBitHloubka(1);
		SetSize(1, 1);
		return false;
	}

	// èíst záhlaví souboru

	BMFH bmfh;
	bool NotCorrupted = true;

	NotCorrupted &= SafeFread((char*) &(bmfh.bfType), sizeof(bmpWORD), 1, fp);

	bool IsBitmap = false;

	if (IsBigEndian() && bmfh.bfType == 16973)
	{
		IsBitmap = true;
	}
	if (!IsBigEndian() && bmfh.bfType == 19778)
	{
		IsBitmap = true;
	}

	if (!IsBitmap)
	{

		fclose(fp);
		return false;
	}

	NotCorrupted &= SafeFread((char*) &(bmfh.bfSize), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmfh.bfReserved1), sizeof(bmpWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmfh.bfReserved2), sizeof(bmpWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmfh.bfOffBits), sizeof(bmpDWORD), 1, fp);

	if (IsBigEndian())
	{
		bmfh.SwitchEndianess();
	}

	// pøeètìte si hlavièku informací
	BMIH bmih;

	NotCorrupted &= SafeFread((char*) &(bmih.biSize), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biWidth), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biHeight), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biPlanes), sizeof(bmpWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biBitCount), sizeof(bmpWORD), 1, fp);

	NotCorrupted &= SafeFread((char*) &(bmih.biCompression), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biSizeImage), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biXPelsPerMeter), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biYPelsPerMeter), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biClrUsed), sizeof(bmpDWORD), 1, fp);
	NotCorrupted &= SafeFread((char*) &(bmih.biClrImportant), sizeof(bmpDWORD), 1, fp);

	if (IsBigEndian())
	{
		bmih.SwitchEndianess();
	}

	// bezpeènostní úchyt: pokud nìkterá z informací o záhlaví nebyla správnì pøeètena, zrušte
	// budoucí nápad: zkontrolujte, zda je alespoò nejvíce dùvìrné

	if (!NotCorrupted)
	{

		SetSize(1, 1);
		SetBitHloubka(1);
		fclose(fp);
		return false;
	}





	if (bmih.biCompression == 3 && bmih.biBitCount != 16)
	{

		SetSize(1, 1);
		SetBitHloubka(1);
		fclose(fp);
		return false;
	}

	// nastavte bit Hloubka

	int TempBitHloubka = (int)bmih.biBitCount;
	if (TempBitHloubka != 1 && TempBitHloubka != 4
		&& TempBitHloubka != 8 && TempBitHloubka != 16
		&& TempBitHloubka != 24 && TempBitHloubka != 32)
	{

		SetSize(1, 1);
		SetBitHloubka(1);
		fclose(fp);
		return false;
	}
	SetBitHloubka((int)bmih.biBitCount);

	// nastav velikost

	if ((int)bmih.biWidth <= 0 || (int)bmih.biHeight <= 0)
	{

		SetSize(1, 1);
		SetBitHloubka(1);
		fclose(fp);
		return false;
	}
	SetSize((int)bmih.biWidth, (int)bmih.biHeight);

	// nìkteré pøedbìžné otázky

	double dBytesPerPixel = ((double)BitHloubka) / 8.0;
	double dBytesPerRow = dBytesPerPixel * (Width + 0.0);
	dBytesPerRow = ceil(dBytesPerRow);

	int BytePaddingPerRow = 4 - ((int)(dBytesPerRow)) % 4;
	if (BytePaddingPerRow == 4)
	{
		BytePaddingPerRow = 0;
	}

	// Pokud <16 bitù, pøeètìte si paletu

	if (BitHloubka < 16)
	{
		// urèete poèet barev urèených v
		// tabulka barev

		int NumberOfColorsToRead = ((int)bmfh.bfOffBits - 54) / 4;
		if (NumberOfColorsToRead > IntPow(2, BitHloubka))
		{
			NumberOfColorsToRead = IntPow(2, BitHloubka);
		}

		if (NumberOfColorsToRead < TellNumberOfColors())
		{

		}

		int n;
		for (n = 0; n < NumberOfColorsToRead; n++)
		{
			SafeFread((char*) &(Colors[n]), 4, 1, fp);
		}
		for (n = NumberOfColorsToRead; n < TellNumberOfColors(); n++)
		{
			RGBApixel WHITE;
			WHITE.Red = 255;
			WHITE.Green = 255;
			WHITE.Blue = 255;
			WHITE.Alpha = 0;
			SetColor(n, WHITE);
		}


	}

	// Pokud bfOffBits  oznaèi, pøeskoèi prázdná data

	int BytesToSkip = bmfh.bfOffBits - 54;;
	if (BitHloubka < 16)
	{
		BytesToSkip -= 4 * IntPow(2, BitHloubka);
	}
	if (BitHloubka == 16 && bmih.biCompression == 3)
	{
		BytesToSkip -= 3 * 4;
	}
	if (BytesToSkip < 0)
	{
		BytesToSkip = 0;
	}
	if (BytesToSkip > 0 && BitHloubka != 16)
	{

		bmpBYTE* TempSkipBYTE;
		TempSkipBYTE = new bmpBYTE[BytesToSkip];
		SafeFread((char*)TempSkipBYTE, BytesToSkip, 1, fp);
		delete[] TempSkipBYTE;
	}

	// Tento kód pøeète soubory 1, 4, 8, 24 a 32-bpp
	// s úèinnìjší vyrovnávací technikou.

	int i, j;
	if (BitHloubka != 16)
	{
		int BufferSize = (int)((Width*BitHloubka) / 8.0);
		while (8 * BufferSize < Width*BitHloubka)
		{
			BufferSize++;
		}
		while (BufferSize % 4)
		{
			BufferSize++;
		}
		bmpBYTE* Buffer;
		Buffer = new bmpBYTE[BufferSize];
		j = Height - 1;
		while (j > -1)
		{
			int BytesRead = (int)fread((char*)Buffer, 1, BufferSize, fp);
			if (BytesRead < BufferSize)
			{
				j = -1;

			}
			else
			{
				bool Success = false;
				if (BitHloubka == 1)
				{
					Success = Read1bitRow(Buffer, BufferSize, j);
				}
				if (BitHloubka == 4)
				{
					Success = Read4bitRow(Buffer, BufferSize, j);
				}
				if (BitHloubka == 8)
				{
					Success = Read8bitRow(Buffer, BufferSize, j);
				}
				if (BitHloubka == 24)
				{
					Success = Read24bitRow(Buffer, BufferSize, j);
				}
				if (BitHloubka == 32)
				{
					Success = Read32bitRow(Buffer, BufferSize, j);
				}
				if (!Success)
				{

					j = -1;
				}
			}
			j--;
		}
		delete[] Buffer;
	}

	if (BitHloubka == 16)
	{
		int DataBytes = Width * 2;
		int PaddingBytes = (4 - DataBytes % 4) % 4;

		// nastavi vychozi masku

		bmpWORD BlueMask = 31; // bits 12-16
		bmpWORD GreenMask = 992; // bits 7-11
		bmpWORD RedMask = 31744; // bits 2-6

								 // pøeètìte bitové pole, pokud je to nutné, na
								 // pøepsat výchozí masku 5-5-5

		if (bmih.biCompression != 0)
		{
			// cte 3 bit masku

			bmpWORD TempMaskWORD;
			bmpWORD ZeroWORD;

			SafeFread((char*)&RedMask, 2, 1, fp);
			if (IsBigEndian())
			{
				RedMask = FlipWORD(RedMask);
			}
			SafeFread((char*)&TempMaskWORD, 2, 1, fp);

			SafeFread((char*)&GreenMask, 2, 1, fp);
			if (IsBigEndian())
			{
				GreenMask = FlipWORD(GreenMask);
			}
			SafeFread((char*)&TempMaskWORD, 2, 1, fp);

			SafeFread((char*)&BlueMask, 2, 1, fp);
			if (IsBigEndian())
			{
				BlueMask = FlipWORD(BlueMask);
			}
			SafeFread((char*)&TempMaskWORD, 2, 1, fp);
		}

		//èíst a pøeskoèit všechny meta data

		if (BytesToSkip > 0)
		{

			bmpBYTE* TempSkipBYTE;
			TempSkipBYTE = new bmpBYTE[BytesToSkip];
			SafeFread((char*)TempSkipBYTE, BytesToSkip, 1, fp);
			delete[] TempSkipBYTE;
		}

		// urèit èervené, zelené a modré posuny

		int GreenShift = 0;
		bmpWORD TempShiftWORD = GreenMask;
		while (TempShiftWORD > 31)
		{
			TempShiftWORD = TempShiftWORD >> 1; GreenShift++;
		}
		int BlueShift = 0;
		TempShiftWORD = BlueMask;
		while (TempShiftWORD > 31)
		{
			TempShiftWORD = TempShiftWORD >> 1; BlueShift++;
		}
		int RedShift = 0;
		TempShiftWORD = RedMask;
		while (TempShiftWORD > 31)
		{
			TempShiftWORD = TempShiftWORD >> 1; RedShift++;
		}

		// ctení aktualniho pixelu

		for (j = Height - 1; j >= 0; j--)
		{
			i = 0;
			int ReadNumber = 0;
			while (ReadNumber < DataBytes)
			{
				bmpWORD TempWORD;
				SafeFread((char*)&TempWORD, 2, 1, fp);
				if (IsBigEndian())
				{
					TempWORD = FlipWORD(TempWORD);
				}
				ReadNumber += 2;

				bmpWORD Red = RedMask & TempWORD;
				bmpWORD Green = GreenMask & TempWORD;
				bmpWORD Blue = BlueMask & TempWORD;

				bmpBYTE BlueBYTE = (bmpBYTE)8 * (Blue >> BlueShift);
				bmpBYTE GreenBYTE = (bmpBYTE)8 * (Green >> GreenShift);
				bmpBYTE RedBYTE = (bmpBYTE)8 * (Red >> RedShift);

				(Pixels[i][j]).Red = RedBYTE;
				(Pixels[i][j]).Green = GreenBYTE;
				(Pixels[i][j]).Blue = BlueBYTE;

				i++;
			}
			ReadNumber = 0;
			while (ReadNumber < PaddingBytes)
			{
				bmpBYTE TempBYTE;
				SafeFread((char*)&TempBYTE, 1, 1, fp);
				ReadNumber++;
			}
		}

	}

	fclose(fp);
	return true;
}

bool BMP::CreateStandardColorTable(void)
{
	using namespace std;
	if (BitHloubka != 1 && BitHloubka != 4 && BitHloubka != 8)
	{

		return false;
	}

	if (BitHloubka == 1)
	{
		int i;
		for (i = 0; i < 2; i++)
		{
			Colors[i].Red = i * 255;
			Colors[i].Green = i * 255;
			Colors[i].Blue = i * 255;
			Colors[i].Alpha = 0;
		}
		return true;
	}
	if (BitHloubka == 4)
	{
		int i = 0;
		int j, k, ell;

		// Zjednodušte kód prvních 8 barev
		for (ell = 0; ell < 2; ell++)
		{
			for (k = 0; k < 2; k++)
			{
				for (j = 0; j < 2; j++)
				{
					Colors[i].Red = j * 128;
					Colors[i].Green = k * 128;
					Colors[i].Blue = ell * 128;
					i++;
				}
			}
		}

		// Zjednodušte kód posledních 8 barev
		for (ell = 0; ell < 2; ell++)
		{
			for (k = 0; k < 2; k++)
			{
				for (j = 0; j < 2; j++)
				{
					Colors[i].Red = j * 255;
					Colors[i].Green = k * 255;
					Colors[i].Blue = ell * 255;
					i++;
				}
			}
		}

		// pøepsat duplicitní barvu
		i = 8;
		Colors[i].Red = 192;
		Colors[i].Green = 192;
		Colors[i].Blue = 192;

		for (i = 0; i < 16; i++)
		{
			Colors[i].Alpha = 0;
		}
		return true;
	}

	if (BitHloubka == 8)
	{
		int i = 0;
		int j, k, ell;

		// dìlat jednoduchou smyèku, která funguje pro všechny barvy
		// 0 až 9 a 246 až 255
		for (ell = 0; ell < 4; ell++)
		{
			for (k = 0; k < 8; k++)
			{
				for (j = 0; j < 8; j++)
				{
					Colors[i].Red = j * 32;
					Colors[i].Green = k * 32;
					Colors[i].Blue = ell * 64;
					Colors[i].Alpha = 0;
					i++;
				}
			}
		}

		// nyní znovu upravte prvních 8 barev 
		i = 0;
		for (ell = 0; ell < 2; ell++)
		{
			for (k = 0; k < 2; k++)
			{
				for (j = 0; j < 2; j++)
				{
					Colors[i].Red = j * 128;
					Colors[i].Green = k * 128;
					Colors[i].Blue = ell * 128;
					i++;
				}
			}
		}

		//pøepsat barvy 7, 8, 9
		i = 7;
		Colors[i].Red = 192;
		Colors[i].Green = 192;
		Colors[i].Blue = 192;
		i++; // 8
		Colors[i].Red = 192;
		Colors[i].Green = 220;
		Colors[i].Blue = 192;
		i++; // 9
		Colors[i].Red = 166;
		Colors[i].Green = 202;
		Colors[i].Blue = 240;

		// pøepsat barvy 246 az 255 
		i = 246;
		Colors[i].Red = 255;
		Colors[i].Green = 251;
		Colors[i].Blue = 240;
		i++; // 247
		Colors[i].Red = 160;
		Colors[i].Green = 160;
		Colors[i].Blue = 164;
		i++; // 248
		Colors[i].Red = 128;
		Colors[i].Green = 128;
		Colors[i].Blue = 128;
		i++; // 249
		Colors[i].Red = 255;
		Colors[i].Green = 0;
		Colors[i].Blue = 0;
		i++; // 250
		Colors[i].Red = 0;
		Colors[i].Green = 255;
		Colors[i].Blue = 0;
		i++; // 251
		Colors[i].Red = 255;
		Colors[i].Green = 255;
		Colors[i].Blue = 0;
		i++; // 252
		Colors[i].Red = 0;
		Colors[i].Green = 0;
		Colors[i].Blue = 255;
		i++; // 253
		Colors[i].Red = 255;
		Colors[i].Green = 0;
		Colors[i].Blue = 255;
		i++; // 254
		Colors[i].Red = 0;
		Colors[i].Green = 255;
		Colors[i].Blue = 255;
		i++; // 255
		Colors[i].Red = 255;
		Colors[i].Green = 255;
		Colors[i].Blue = 255;

		return true;
	}
	return true;
}

bool SafeFread(char* buffer, int size, int number, FILE* fp)
{
	using namespace std;
	int ItemsRead;
	if (feof(fp))
	{
		return false;
	}
	ItemsRead = (int)fread(buffer, size, number, fp);
	if (ItemsRead < number)
	{
		return false;
	}
	return true;
}

void BMP::SetDPI(int HorizontalDPI, int VerticalDPI)
{
	XPelsPerMeter = (int)(HorizontalDPI * 39.37007874015748);
	YPelsPerMeter = (int)(VerticalDPI * 39.37007874015748);
}

// int BMP::TellVerticalDPI( void ) const
int BMP::TellVerticalDPI(void)
{
	if (!YPelsPerMeter)
	{
		YPelsPerMeter = DefaultYPelsPerMeter;
	}
	return (int)(YPelsPerMeter / (double) 39.37007874015748);
}

// int BMP::TellHorizontalDPI( void ) const
int BMP::TellHorizontalDPI(void)
{
	if (!XPelsPerMeter)
	{
		XPelsPerMeter = DefaultXPelsPerMeter;
	}
	return (int)(XPelsPerMeter / (double) 39.37007874015748);
}

/* Tyto funkce jsou definovány v souboru BMP_VariousBMPutilities.h */

BMFH GetBMFH(const char* szFileNameIn)
{
	using namespace std;
	BMFH bmfh;

	FILE* fp;
	fp = fopen(szFileNameIn, "rb");

	if (!fp)
	{

		bmfh.bfType = 0;
		return bmfh;
	}

	SafeFread((char*) &(bmfh.bfType), sizeof(bmpWORD), 1, fp);
	SafeFread((char*) &(bmfh.bfSize), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmfh.bfReserved1), sizeof(bmpWORD), 1, fp);
	SafeFread((char*) &(bmfh.bfReserved2), sizeof(bmpWORD), 1, fp);
	SafeFread((char*) &(bmfh.bfOffBits), sizeof(bmpDWORD), 1, fp);

	fclose(fp);

	if (IsBigEndian())
	{
		bmfh.SwitchEndianess();
	}

	return bmfh;
}

BMIH GetBMIH(const char* szFileNameIn)
{
	using namespace std;
	BMFH bmfh;
	BMIH bmih;

	FILE* fp;
	fp = fopen(szFileNameIn, "rb");

	if (!fp)
	{

		return bmih;
	}

	// pøeètìte si bmfh, tj. první 14 bajtù (jen aby se dostali mimo cestu);

	bmpBYTE TempBYTE;
	int i;
	for (i = 14; i > 0; i--)
	{
		SafeFread((char*)&TempBYTE, sizeof(bmpBYTE), 1, fp);
	}

	// read the bmih 

	SafeFread((char*) &(bmih.biSize), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmih.biWidth), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmih.biHeight), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmih.biPlanes), sizeof(bmpWORD), 1, fp);

	SafeFread((char*) &(bmih.biBitCount), sizeof(bmpWORD), 1, fp);
	SafeFread((char*) &(bmih.biCompression), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmih.biSizeImage), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmih.biXPelsPerMeter), sizeof(bmpDWORD), 1, fp);

	SafeFread((char*) &(bmih.biYPelsPerMeter), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmih.biClrUsed), sizeof(bmpDWORD), 1, fp);
	SafeFread((char*) &(bmih.biClrImportant), sizeof(bmpDWORD), 1, fp);

	fclose(fp);

	if (IsBigEndian())
	{
		bmih.SwitchEndianess();
	}

	return bmih;
}

void DisplayBitmapInfo(const char* szFileNameIn)
{
	using namespace std;
	FILE* fp;
	fp = fopen(szFileNameIn, "rb");

	if (!fp)
	{

		return;
	}
	fclose(fp);

	// nepoužívejte duplicitní práci! Použijte funkce shora!

	BMFH bmfh = GetBMFH(szFileNameIn);
	BMIH bmih = GetBMIH(szFileNameIn);

	cout << "Informace o souboru pro soubor " << szFileNameIn
		<< ":" << endl << endl;

	cout << "BITMAPFILEHEADER:" << endl
		<< "bfType: " << bmfh.bfType << endl
		<< "bfSize: " << bmfh.bfSize << endl
		<< "bfReserved1: " << bmfh.bfReserved1 << endl
		<< "bfReserved2: " << bmfh.bfReserved2 << endl
		<< "bfOffBits: " << bmfh.bfOffBits << endl << endl;

	cout << "BITMAPINFOHEADER:" << endl
		<< "biSize: " << bmih.biSize << endl
		<< "biWidth: " << bmih.biWidth << endl
		<< "biHeight: " << bmih.biHeight << endl
		<< "biPlanes: " << bmih.biPlanes << endl
		<< "biBitCount: " << bmih.biBitCount << endl
		<< "biCompression: " << bmih.biCompression << endl
		<< "biSizeImage: " << bmih.biSizeImage << endl
		<< "biXPelsPerMeter: " << bmih.biXPelsPerMeter << endl
		<< "biYPelsPerMeter: " << bmih.biYPelsPerMeter << endl
		<< "biClrUsed: " << bmih.biClrUsed << endl
		<< "biClrImportant: " << bmih.biClrImportant << endl << endl;
	return;
}

int GetBitmapColorHloubka(const char* szFileNameIn)
{
	BMIH bmih = GetBMIH(szFileNameIn);
	return (int)bmih.biBitCount;
}

void PixelToPixelCopy(BMP& From, int FromX, int FromY,
	BMP& To, int ToX, int ToY)
{
	*To(ToX, ToY) = *From(FromX, FromY);
	return;
}

void PixelToPixelCopyTransparent(BMP& From, int FromX, int FromY,
	BMP& To, int ToX, int ToY,
	RGBApixel& Transparent)
{
	if (From(FromX, FromY)->Red != Transparent.Red ||
		From(FromX, FromY)->Green != Transparent.Green ||
		From(FromX, FromY)->Blue != Transparent.Blue)
	{
		*To(ToX, ToY) = *From(FromX, FromY);
	}
	return;
}

void RangedPixelToPixelCopy(BMP& From, int FromL, int FromR, int FromB, int FromT,
	BMP& To, int ToX, int ToY)
{
	// ujisti se, že jsou dodržovány konvence
	if (FromB < FromT)
	{
		int Temp = FromT; FromT = FromB; FromB = Temp;
	}

	// ujisti se, že kopírované oblasti existují v obou bitmapách
	if (FromR >= From.TellWidth())
	{
		FromR = From.TellWidth() - 1;
	}
	if (FromL < 0) { FromL = 0; }

	if (FromB >= From.TellHeight())
	{
		FromB = From.TellHeight() - 1;
	}
	if (FromT < 0) { FromT = 0; }

	if (ToX + (FromR - FromL) >= To.TellWidth())
	{
		FromR = To.TellWidth() - 1 + FromL - ToX;
	}
	if (ToY + (FromB - FromT) >= To.TellHeight())
	{
		FromB = To.TellHeight() - 1 + FromT - ToY;
	}

	int i, j;
	for (j = FromT; j <= FromB; j++)
	{
		for (i = FromL; i <= FromR; i++)
		{
			PixelToPixelCopy(From, i, j,
				To, ToX + (i - FromL), ToY + (j - FromT));
		}
	}

	return;
}

void RangedPixelToPixelCopyTransparent(
	BMP& From, int FromL, int FromR, int FromB, int FromT,
	BMP& To, int ToX, int ToY,
	RGBApixel& Transparent)
{
	// ujisti se, že jsou dodržovány konvence
	if (FromB < FromT)
	{
		int Temp = FromT; FromT = FromB; FromB = Temp;
	}

	// ujisti se, že kopírované oblasti existují v obou bitmapách
	if (FromR >= From.TellWidth())
	{
		FromR = From.TellWidth() - 1;
	}
	if (FromL < 0) { FromL = 0; }

	if (FromB >= From.TellHeight())
	{
		FromB = From.TellHeight() - 1;
	}
	if (FromT < 0) { FromT = 0; }

	if (ToX + (FromR - FromL) >= To.TellWidth())
	{
		FromR = To.TellWidth() - 1 + FromL - ToX;
	}
	if (ToY + (FromB - FromT) >= To.TellHeight())
	{
		FromB = To.TellHeight() - 1 + FromT - ToY;
	}

	int i, j;
	for (j = FromT; j <= FromB; j++)
	{
		for (i = FromL; i <= FromR; i++)
		{
			PixelToPixelCopyTransparent(From, i, j,
				To, ToX + (i - FromL), ToY + (j - FromT),
				Transparent);
		}
	}

	return;
}

bool CreateGrayscaleColorTable(BMP& InputImage)
{
	using namespace std;
	int BitHloubka = InputImage.TellBitHloubka();
	if (BitHloubka != 1 && BitHloubka != 4 && BitHloubka != 8)
	{

		return false;
	}
	int i;
	int NumberOfColors = InputImage.TellNumberOfColors();

	bmpBYTE StepSize;
	if (BitHloubka != 1)
	{
		StepSize = 255 / (NumberOfColors - 1);
	}
	else
	{
		StepSize = 255;
	}

	for (i = 0; i < NumberOfColors; i++)
	{
		bmpBYTE TempBYTE = i*StepSize;
		RGBApixel TempColor;
		TempColor.Red = TempBYTE;
		TempColor.Green = TempBYTE;
		TempColor.Blue = TempBYTE;
		TempColor.Alpha = 0;
		InputImage.SetColor(i, TempColor);
	}
	return true;
}

bool BMP::Read32bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int i;
	if (Width * 4 > BufferSize)
	{
		return false;
	}
	for (i = 0; i < Width; i++)
	{
		memcpy((char*) &(Pixels[i][Row]), (char*)Buffer + 4 * i, 4);
	}
	return true;
}

bool BMP::Read24bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int i;
	if (Width * 3 > BufferSize)
	{
		return false;
	}
	for (i = 0; i < Width; i++)
	{
		memcpy((char*) &(Pixels[i][Row]), Buffer + 3 * i, 3);
	}
	return true;
}

bool BMP::Read8bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int i;
	if (Width > BufferSize)
	{
		return false;
	}
	for (i = 0; i < Width; i++)
	{
		int Index = Buffer[i];
		*(this->operator()(i, Row)) = GetColor(Index);
	}
	return true;
}

bool BMP::Read4bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int Shifts[2] = { 4  ,0 };
	int Masks[2] = { 240,15 };

	int i = 0;
	int j;
	int k = 0;
	if (Width > 2 * BufferSize)
	{
		return false;
	}
	while (i < Width)
	{
		j = 0;
		while (j < 2 && i < Width)
		{
			int Index = (int)((Buffer[k] & Masks[j]) >> Shifts[j]);
			*(this->operator()(i, Row)) = GetColor(Index);
			i++; j++;
		}
		k++;
	}
	return true;
}
bool BMP::Read1bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int Shifts[8] = { 7  ,6 ,5 ,4 ,3,2,1,0 };
	int Masks[8] = { 128,64,32,16,8,4,2,1 };

	int i = 0;
	int j;
	int k = 0;

	if (Width > 8 * BufferSize)
	{
		return false;
	}
	while (i < Width)
	{
		j = 0;
		while (j < 8 && i < Width)
		{
			int Index = (int)((Buffer[k] & Masks[j]) >> Shifts[j]);
			*(this->operator()(i, Row)) = GetColor(Index);
			i++; j++;
		}
		k++;
	}
	return true;
}

bool BMP::Write32bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int i;
	if (Width * 4 > BufferSize)
	{
		return false;
	}
	for (i = 0; i < Width; i++)
	{
		memcpy((char*)Buffer + 4 * i, (char*) &(Pixels[i][Row]), 4);
	}
	return true;
}

bool BMP::Write24bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int i;
	if (Width * 3 > BufferSize)
	{
		return false;
	}
	for (i = 0; i < Width; i++)
	{
		memcpy((char*)Buffer + 3 * i, (char*) &(Pixels[i][Row]), 3);
	}
	return true;
}

bool BMP::Write8bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int i;
	if (Width > BufferSize)
	{
		return false;
	}
	for (i = 0; i < Width; i++)
	{
		Buffer[i] = FindClosestColor(Pixels[i][Row]);
	}
	return true;
}

bool BMP::Write4bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int PositionWeights[2] = { 16,1 };

	int i = 0;
	int j;
	int k = 0;
	if (Width > 2 * BufferSize)
	{
		return false;
	}
	while (i < Width)
	{
		j = 0;
		int Index = 0;
		while (j < 2 && i < Width)
		{
			Index += (PositionWeights[j] * (int)FindClosestColor(Pixels[i][Row]));
			i++; j++;
		}
		Buffer[k] = (bmpBYTE)Index;
		k++;
	}
	return true;
}

bool BMP::Write1bitRow(bmpBYTE* Buffer, int BufferSize, int Row)
{
	int PositionWeights[8] = { 128,64,32,16,8,4,2,1 };

	int i = 0;
	int j;
	int k = 0;
	if (Width > 8 * BufferSize)
	{
		return false;
	}
	while (i < Width)
	{
		j = 0;
		int Index = 0;
		while (j < 8 && i < Width)
		{
			Index += (PositionWeights[j] * (int)FindClosestColor(Pixels[i][Row]));
			i++; j++;
		}
		Buffer[k] = (bmpBYTE)Index;
		k++;
	}
	return true;
}

bmpBYTE BMP::FindClosestColor(RGBApixel& input)
{
	using namespace std;

	int i = 0;
	int NumberOfColors = TellNumberOfColors();
	bmpBYTE BestI = 0;
	int BestMatch = 999999;

	while (i < NumberOfColors)
	{
		RGBApixel Attempt = GetColor(i);
		int TempMatch = IntSquare((int)Attempt.Red - (int)input.Red)
			+ IntSquare((int)Attempt.Green - (int)input.Green)
			+ IntSquare((int)Attempt.Blue - (int)input.Blue);
		if (TempMatch < BestMatch)
		{
			BestI = (bmpBYTE)i; BestMatch = TempMatch;
		}
		if (BestMatch < 1)
		{
			i = NumberOfColors;
		}
		i++;
	}
	return BestI;
}



bool Rescale(BMP& InputImage, char mode, int NewDimension)
{
	using namespace std;
	int CapMode = toupper(mode);

	BMP OldImage(InputImage);

	if (CapMode != 'P' &&
		CapMode != 'W' &&
		CapMode != 'H' &&
		CapMode != 'F')
	{

		return false;
	}

	int NewWidth = 0;
	int NewHeight = 0;

	int OldWidth = OldImage.TellWidth();
	int OldHeight = OldImage.TellHeight();

	if (CapMode == 'P')
	{
		NewWidth = (int)floor(OldWidth * NewDimension / 100.0);
		NewHeight = (int)floor(OldHeight * NewDimension / 100.0);
	}
	if (CapMode == 'F')
	{
		if (OldWidth > OldHeight)
		{
			CapMode = 'W';
		}
		else
		{
			CapMode = 'H';
		}
	}

	if (CapMode == 'W')
	{
		double percent = (double)NewDimension / (double)OldWidth;
		NewWidth = NewDimension;
		NewHeight = (int)floor(OldHeight * percent);
	}
	if (CapMode == 'H')
	{
		double percent = (double)NewDimension / (double)OldHeight;
		NewHeight = NewDimension;
		NewWidth = (int)floor(OldWidth * percent);
	}

	if (NewWidth < 1)
	{
		NewWidth = 1;
	}
	if (NewHeight < 1)
	{
		NewHeight = 1;
	}

	InputImage.SetSize(NewWidth, NewHeight);
	InputImage.SetBitHloubka(24);

	int I, J;
	double ThetaI, ThetaJ;

	for (int j = 0; j < NewHeight - 1; j++)
	{
		ThetaJ = (double)(j*(OldHeight - 1.0))
			/ (double)(NewHeight - 1.0);
		J = (int)floor(ThetaJ);
		ThetaJ -= J;

		for (int i = 0; i < NewWidth - 1; i++)
		{
			ThetaI = (double)(i*(OldWidth - 1.0))
				/ (double)(NewWidth - 1.0);
			I = (int)floor(ThetaI);
			ThetaI -= I;

			InputImage(i, j)->Red = (bmpBYTE)
				((1.0 - ThetaI - ThetaJ + ThetaI*ThetaJ)*(OldImage(I, J)->Red)
					+ (ThetaI - ThetaI*ThetaJ)*(OldImage(I + 1, J)->Red)
					+ (ThetaJ - ThetaI*ThetaJ)*(OldImage(I, J + 1)->Red)
					+ (ThetaI*ThetaJ)*(OldImage(I + 1, J + 1)->Red));
			InputImage(i, j)->Green = (bmpBYTE)
				((1.0 - ThetaI - ThetaJ + ThetaI*ThetaJ)*OldImage(I, J)->Green
					+ (ThetaI - ThetaI*ThetaJ)*OldImage(I + 1, J)->Green
					+ (ThetaJ - ThetaI*ThetaJ)*OldImage(I, J + 1)->Green
					+ (ThetaI*ThetaJ)*OldImage(I + 1, J + 1)->Green);
			InputImage(i, j)->Blue = (bmpBYTE)
				((1.0 - ThetaI - ThetaJ + ThetaI*ThetaJ)*OldImage(I, J)->Blue
					+ (ThetaI - ThetaI*ThetaJ)*OldImage(I + 1, J)->Blue
					+ (ThetaJ - ThetaI*ThetaJ)*OldImage(I, J + 1)->Blue
					+ (ThetaI*ThetaJ)*OldImage(I + 1, J + 1)->Blue);
		}
		InputImage(NewWidth - 1, j)->Red = (bmpBYTE)
			((1.0 - ThetaJ)*(OldImage(OldWidth - 1, J)->Red)
				+ ThetaJ*(OldImage(OldWidth - 1, J + 1)->Red));
		InputImage(NewWidth - 1, j)->Green = (bmpBYTE)
			((1.0 - ThetaJ)*(OldImage(OldWidth - 1, J)->Green)
				+ ThetaJ*(OldImage(OldWidth - 1, J + 1)->Green));
		InputImage(NewWidth - 1, j)->Blue = (bmpBYTE)
			((1.0 - ThetaJ)*(OldImage(OldWidth - 1, J)->Blue)
				+ ThetaJ*(OldImage(OldWidth - 1, J + 1)->Blue));
	}

	for (int i = 0; i < NewWidth - 1; i++)
	{
		ThetaI = (double)(i*(OldWidth - 1.0))
			/ (double)(NewWidth - 1.0);
		I = (int)floor(ThetaI);
		ThetaI -= I;
		InputImage(i, NewHeight - 1)->Red = (bmpBYTE)
			((1.0 - ThetaI)*(OldImage(I, OldHeight - 1)->Red)
				+ ThetaI*(OldImage(I, OldHeight - 1)->Red));
		InputImage(i, NewHeight - 1)->Green = (bmpBYTE)
			((1.0 - ThetaI)*(OldImage(I, OldHeight - 1)->Green)
				+ ThetaI*(OldImage(I, OldHeight - 1)->Green));
		InputImage(i, NewHeight - 1)->Blue = (bmpBYTE)
			((1.0 - ThetaI)*(OldImage(I, OldHeight - 1)->Blue)
				+ ThetaI*(OldImage(I, OldHeight - 1)->Blue));
	}

	*InputImage(NewWidth - 1, NewHeight - 1) = *OldImage(OldWidth - 1, OldHeight - 1);
	return true;
}

















