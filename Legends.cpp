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

	TerrainSettings ts;

	Generator gen;
	gen.generate(ts, 256, 256, 0, 2019);

	write_tiff(gen, getCurrentDirectory() + "\\world");

    return 0;
}

