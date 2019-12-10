#include "stdafx.h"

#include "TiffObject.h"
#include "Generator.h"

#include "tiffio.h"

void write_terrain_overlay_file(const std::string &terrain_file, const int &width, const int &height, const TerrainWorld &terrain) {
	TIFF *file = TIFFOpen(terrain_file.c_str(), "w");
	if (!file) {
		printf("Unable to open file %s\n", terrain_file.c_str());
		return;
	}

	TIFFSetField(file, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);

	// Pack bits
	//TIFFSetField(file, TIFFTAG_COMPRESSION, 0);
	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	// TIFFSetField (tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	//TIFFSetField(file, TIFFTAG_XRESOLUTION, 1);
	//TIFFSetField(file, TIFFTAG_YRESOLUTION, 1);
	//TIFFSetField(file, TIFFTAG_RESOLUTIONUNIT, 2);
	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);

	unsigned char  *ptr = new unsigned char[width*height * 4];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int idx = y * width + x;
			int16 type = terrain.elevation[y*width + x];

			int bufferIdx = (y * width + x) * 4;
			/*
			if (type == TerrainType::Terrain_Bottom) {
				ptr[bufferIdx + 0] = 0;
				ptr[bufferIdx + 1] = 0;
				ptr[bufferIdx + 2] = 0;
				ptr[bufferIdx + 3] = 255;
			}
			else if (type == TerrainType::Terrain_Dirt) {
				ptr[bufferIdx + 0] = 185;
				ptr[bufferIdx + 1] = 122;
				ptr[bufferIdx + 2] = 87;
				ptr[bufferIdx + 3] = 255;
			}
			else if (type == TerrainType::Terrain_Ocean) {
				ptr[bufferIdx + 0] = 0;
				ptr[bufferIdx + 1] = 0;
				ptr[bufferIdx + 2] = 128;
				ptr[bufferIdx + 3] = 255;
			}
			else if (type == TerrainType::Terrain_Geode) {
				ptr[bufferIdx + 0] = 79;
				ptr[bufferIdx + 1] = 39;
				ptr[bufferIdx + 2] = 0;
				ptr[bufferIdx + 3] = 255;
			}
			else if (type == TerrainType::Terrain_Hill) {
				ptr[bufferIdx + 0] = 0;
				ptr[bufferIdx + 1] = 255;
				ptr[bufferIdx + 2] = 64;
				ptr[bufferIdx + 3] = 255;
			}
			else if (type == TerrainType::Terrain_River) {
				ptr[bufferIdx + 0] = 0;
				ptr[bufferIdx + 1] = 128;
				ptr[bufferIdx + 2] = 255;
				ptr[bufferIdx + 3] = 255;
			}
			else if (type == TerrainType::Terrain_Mountain) {
				ptr[bufferIdx + 0] = 128;
				ptr[bufferIdx + 1] = 128;
				ptr[bufferIdx + 2] = 128;
				ptr[bufferIdx + 3] = 255;
			}
			*/
		}
	}

	for (int y = 0; y < height; y++) {
		TIFFWriteScanline(file, ptr + y * width * 4, y, 0);
	}

	TIFFClose(file);

	delete[] ptr;
}

void write_terrain_file(const std::string &terrain_file, const int &width, const int &height, const TerrainWorld &terrain) {
	TIFF *file = TIFFOpen(terrain_file.c_str(), "w");
	if (!file) {
		printf("Unable to open file %s\n", terrain_file.c_str());
		return;
	}

	TIFFSetField(file, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 32);

	// Pack bits
	//TIFFSetField(file, TIFFTAG_COMPRESSION, 0);
	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	// TIFFSetField (tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	//TIFFSetField(file, TIFFTAG_XRESOLUTION, 1);
	//TIFFSetField(file, TIFFTAG_YRESOLUTION, 1);
	TIFFSetField(file, TIFFTAG_RESOLUTIONUNIT, 2);
	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, 1);

	int32  *ptr = new int32[width*height];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int idx = y * width + x;
			ptr[idx] = int32(terrain.elevation[y*width + x]);
		}
	}

	uint8 *data = (uint8*)ptr;
	for (int y = 0; y < height; y++) {
		TIFFWriteScanline(file, data + y * width * 4, y, 0);
	}

	TIFFClose(file);

	delete[] ptr;
}

void read_tiff(Generator &g, const std::string &path) {

}

void write_tiff(Generator &g, const std::string &path) {
	// Make directory if it doesn't exist
	if (!directoryExists(path))
		createDirectory(path);

	auto terrain = g.world;

	write_terrain_file(path + "\\terrain.tiff", terrain.world_width, terrain.world_height, terrain);
	write_terrain_overlay_file(path + "\\overlay.tiff", terrain.world_width, terrain.world_height, terrain);
}
