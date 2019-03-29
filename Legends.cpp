// Legends.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Generator.h"
#include "TiffObject.h"

#include "tiffio.h"

void warningHandler(const char *module, const char *fmt, va_list ap)
{
	char buffer[2048];
	memset(buffer, 0, 2048);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, ap);

	printf("Warning: module - %s: %s\n", module, buffer);
}

void errorHandler(const char *module, const char *fmt, va_list ap)
{
	char buffer[2048];
	memset(buffer, 0, 2048);

	_vsnprintf_s(buffer, sizeof(buffer), fmt, ap);

	printf("Error: module - %s: %s\n", module, buffer);
}

int main()
{
	TIFFSetWarningHandler(warningHandler);
	TIFFSetErrorHandler(errorHandler);

	Generator gen;

	gen.generate(1000, 1000, 0, 2018);

	write_tiff(gen, getCurrentDirectory() + "\\world");

	auto terrain = gen.getTerrain();

	int width, height;
	gen.getGridSize(&width, &height);

#if 0
	// Console version
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%f,", terrain[y*width + x].terrain_value);
		}
		printf("\n");
	}
#endif

    return 0;
}

