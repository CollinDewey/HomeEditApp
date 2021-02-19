#include "3ds/imgui_citro3d.h"
#include "3ds/imgui_ctru.h"
#include "imgui/imgui.h"
#include "widgets.h"
#include "lz77.h"

#include <cstdio>
#include <cstdlib>

#include <citro3d.h>
#include <tex3ds.h>

FileBuffer UncompressLz77(FILE* fp);
bool CompressLzExFile(FileBuffer input, FILE* output);
void writeColor(ImVec4 color, unsigned int offset, FileBuffer file);
ImVec4 getColor(unsigned int offset, FileBuffer file);