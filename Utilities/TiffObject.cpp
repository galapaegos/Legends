#include "stdafx.h"

#include "TiffObject.h"
#include "Generator.h"

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

void write_tiff_file(const std::string &terrain_file, const int &width, const int &height, const std::vector<uint8> &grid) {
	TIFF *file = TIFFOpen(terrain_file.c_str(), "w");
	if (!file) {
		printf("Unable to open file %s\n", terrain_file.c_str());
		return;
	}

	TIFFSetField(file, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);

	// Pack bits
	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(file, TIFFTAG_RESOLUTIONUNIT, 2);
	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);

	uint8 *data = (uint8*)&grid[0];
	for (int y = 0; y < height; y++) {
		TIFFWriteScanline(file, data + y * width, y, 0);
	}

	TIFFClose(file);
}

void write_tiff_file(const std::string &terrain_file, const int &width, const int &height, const std::vector<float32> &grid) {
	TIFF *file = TIFFOpen(terrain_file.c_str(), "w");
	if (!file) {
		printf("Unable to open file %s\n", terrain_file.c_str());
		return;
	}

	TIFFSetField(file, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 32);
	TIFFSetField(file, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);

	// Pack bits
	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(file, TIFFTAG_RESOLUTIONUNIT, 2);
	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);

	uint8 *data = (uint8*)&grid[0];
	for (int y = 0; y < height; y++) {
		TIFFWriteScanline(file, data + y * width * 4, y, 0);
	}

	TIFFClose(file);
}

void write_tiff_file(const std::string &terrain_file, const int &width, const int &height, 
	const std::vector<glm::vec<4, uint8>> &grid) {
	TIFF *file = TIFFOpen(terrain_file.c_str(), "w");
	if (!file) {
		printf("Unable to open file %s\n", terrain_file.c_str());
		return;
	}

	TIFFSetField(file, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(file, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);

	// Pack bits
	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(file, TIFFTAG_RESOLUTIONUNIT, 2);
	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);

	//TIFFSetField(file, TIFFTAG_EXTRASAMPLES, 1, EXTRASAMPLE_ASSOCALPHA);

	uint8 *data = (uint8*)&grid[0];
	for (int y = 0; y < height; y++) {
		TIFFWriteScanline(file, data + y * width * 4, y, 0);
	}

	TIFFClose(file);
}
