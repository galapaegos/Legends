// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <functional>
#include <algorithm>
#include <cmath>
#include <random>
#include <map>
#include <numeric>
#include <vector>

// TODO: reference additional headers your program requires here
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Tiff.h renames int/char, etc
#include "Utilities.h"
#include "StringFormat.h"
#include "TiffObject.h"

typedef float float32;
typedef double float64;
