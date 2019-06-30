#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include "BMP.h"
# define M_PI 
# define M_PIl          3.141592653589793238462643383279502884L
using namespace std;



/*void rotate(int h, int w, int size) {
	unsigned char *pixels = new unsigned char[size];

	double radians = (degree * M_PI) / 180;
	int sinf = (int)sin(radians);
	int cosf = (int)cos(radians);

	double x0 = 0.5 * (w - 1);
	double y0 = 0.5 * (h - 1);


	for (int g = 0; g < w; g++) {
		for (int d = 0; d < h; d++) {
			long double a = g - x0;
			long double b = d - y0;
			int xx = (int)(+a * cosf - b * sinf + x0);
			int yy = (int)(+a * sinf + b * cosf + y0);

			if (xx >= 0 && xx < image.width && yy >= 0 && yy < image.height) {
				pixels[(d * h + g) * 3 + 0] = size[(yy * h + xx) * 3 + 0];
				pixels[(d * h + g) * 3 + 1] = size[(yy * h + xx) * 3 + 1];
				pixels[(d * h + g) * 3 + 2] = size[(yy * h + xx) * 3 + 2];
			}
		}
	}
	size = pixels;
}*/


int main(int argc, char* argv[])
{
	cout << endl
		<< "Program slucuje obrazky do jednoho " << endl;


	BMP Text;
	Text.ReadFromFile("maryline.bmp");

	BMP Text1;
	Text1.ReadFromFile("mickey.bmp");



	//nacitaní hlavního obrázku lenna.bmp, který slouží pøi slouèení obrákù jako pozadi
	BMP Background;
	Background.ReadFromFile("lenna.bmp");

	BMP Output;
	Output.SetSize(Background.TellWidth(), Background.TellHeight());
	Output.SetBitHloubka(24);

	RangedPixelToPixelCopy(Background, 0, Output.TellWidth() - 1,
		Output.TellHeight() - 1, 0,
		Output, 0, 0);

	RangedPixelToPixelCopyTransparent(Text, 0, 380,
		43, 0,
		Output, 110, 5,
		*Text(0, 0));

	RangedPixelToPixelCopyTransparent(Text, 0, Text.TellWidth() - 1,
		Text.TellWidth() - 1, 50,
		Output, 10, 100,
		*Text(0, 49));


	RangedPixelToPixelCopyTransparent(Text1, 0, 380,
		43, 0,
		Output, 110, 5,
		*Text1(0, 0));

	RangedPixelToPixelCopyTransparent(Text1, 0, Text1.TellWidth() - 1,
		Text1.TellWidth() - 1, 50,
		Output, 100, 100,
		*Text1(0, 49));

	Output.SetBitHloubka(32);
	cout << "Vytvarim soubor 32bpp ... " << endl;
	Output.WriteToFile("vystup 32bpp.bmp");

	Output.SetBitHloubka(24);
	cout << "Vytvarim soubor 24bpp ... " << endl;
	Output.WriteToFile("vystup 24bpp.bmp");

	Output.SetBitHloubka(8);
	cout << "Vytvarim soubor 8bpp ... " << endl;
	Output.WriteToFile("vystup 8bpp.bmp");

	Output.SetBitHloubka(4);
	cout << "Vytvarim soubor 4bpp ... " << endl;
	Output.WriteToFile("vystup 4bpp.bmp");

	Output.SetBitHloubka(1);
	cout << "Vytvarim soubor 1bpp ... " << endl;
	Output.WriteToFile("vystup 1bpp.bmp");

	Output.SetBitHloubka(24);
	Rescale(Output, 'p', 50);
	cout << "Vytvarim soubor 24bpp zmmneny obraz ..." << endl;
	Output.WriteToFile("EasyBMPoutput24bpp_rescaled.bmp");


	getchar();
	return 0;
}