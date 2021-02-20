// Copyright (C) 2020 Michael Theall
// Copyright (C) 2021 Legit_Magic
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "main.h"

#if ANTI_ALIAS
/// \brief Display transfer scaling
constexpr auto TRANSFER_SCALING = GX_TRANSFER_SCALE_XY;
/// \brief Framebuffer scale
constexpr auto FB_SCALE = 2.0f;
#else
/// \brief Display transfer scaling
constexpr auto TRANSFER_SCALING = GX_TRANSFER_SCALE_NO;
/// \brief Framebuffer scale
constexpr auto FB_SCALE = 1.0f;
#endif

/// \brief Top screen render target
C3D_RenderTarget *s_top = nullptr;
/// \brief Bottom screen render target
C3D_RenderTarget *s_bottom = nullptr;

/// \brief Screen width
constexpr auto SCREEN_WIDTH = 400.0f;
/// \brief Screen height
constexpr auto SCREEN_HEIGHT = 480.0f;
/// \brief Framebuffer width
constexpr auto FB_WIDTH = SCREEN_WIDTH * FB_SCALE;
/// \brief Framebuffer height
constexpr auto FB_HEIGHT = SCREEN_HEIGHT * FB_SCALE;

/// \brief Display transfer flags
constexpr auto DISPLAY_TRANSFER_FLAGS =
	GX_TRANSFER_FLIP_VERT (0) | GX_TRANSFER_OUT_TILED (0) | GX_TRANSFER_RAW_COPY (0) |
	GX_TRANSFER_IN_FORMAT (GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT (GX_TRANSFER_FMT_RGB8) |
	GX_TRANSFER_SCALING (TRANSFER_SCALING);

///// \brief Texture atlas
//C3D_Tex s_gfxTexture;
///// \brief Texture atlas metadata
//Tex3DS_Texture s_gfxT3x;

/// \brief Clear color
constexpr auto CLEAR_COLOR = 0x0080FFFF;

int main (int argc_, char *argv_[]) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// enable New 3DS speedup
	osSetSpeedupEnable (true);

	// init services
	romfsInit();
	gfxInitDefault();
	gfxSet3D(false);

	// initialize citro3d
	C3D_Init (2 * C3D_DEFAULT_CMDBUF_SIZE);

	// create top screen render target
	s_top = C3D_RenderTargetCreate(FB_HEIGHT * 0.5f, FB_WIDTH, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(s_top, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// create bottom screen render target
	s_bottom = C3D_RenderTargetCreate(FB_HEIGHT * 0.5f, FB_WIDTH * 0.8f, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(s_bottom, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	if (!imgui::ctru::init())
		return false;

	imgui::citro3d::init();

	// load texture atlas
	//FILE *file = fopen("test.bin","rb");
	//if (!fopen("romfs:/gfx.t3x", "r"))
	//	svcBreak(USERBREAK_PANIC);
	//
	//s_gfxT3x = Tex3DS_TextureImportStdio (file, &s_gfxTexture, nullptr, true);
	//if (!s_gfxT3x)
	//	svcBreak(USERBREAK_PANIC);
	//
	//C3D_TexSetFilter (&s_gfxTexture, GPU_LINEAR, GPU_LINEAR);

	auto &io    = ImGui::GetIO();
	auto &style = ImGui::GetStyle();

	// disable imgui.ini file
	io.IniFilename = nullptr;

	style.Colors[ImGuiCol_WindowBg].w = 0.9f;
	style.Colors[ImGuiCol_PopupBg].w = 1.0f;
	style.ScaleAllSizes(0.5f);
	
	// bottom screen safe area. number from trial/error
	//style.DisplaySafeAreaPadding = ImVec2(146.8f, 0.0f);
	//style.DisplaySafeAreaPadding = ImVec2(55.0f, 0.0f);

	// turn off window rounding
	style.WindowRounding = 0.0f;

	// setup display metrics
	io.DisplaySize = ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT);
	io.DisplayFramebufferScale = ImVec2(FB_SCALE, FB_SCALE);
	auto const width = io.DisplaySize.x;
	auto const height = io.DisplaySize.y;

	FILE *source = fopen("sdmc:/luma/titles/0004003000008F02/romfs/sleep_LZ.bin","rb");
	FileBuffer uncompressed = UncompressLz77(source);
	fclose(source);

	static ImVec4 top_sleep_background			= getColor(0x00001CD4, uncompressed);
	static ImVec4 top_sleep_background_glow		= getColor(0x00001CD8, uncompressed);
	static ImVec4 top_sleep_background_stripes	= getColor(0x00001CD0, uncompressed);
	static ImVec4 top_sleep_header				= getColor(0x00001D70, uncompressed);
	static ImVec4 top_sleep_text				= getColor(0x00001DA4, uncompressed);
	static ImVec4 top_sleep_footer				= getColor(0x00001EA8, uncompressed);
	static ImVec4 top_sleep_line				= getColor(0x00001E24, uncompressed);

	while (aptMainLoop()) {

		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			return false;

		imgui::ctru::newFrame();
		ImGui::NewFrame();

		// top screen
		//ImGui::SetNextWindowSize(ImVec2(width, height * 0.5f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(width*0.03f, height*0.20f), ImGuiCond_FirstUseEver);

			ImGui::Begin("Preview of home/sleep menu here?", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::Text("sdmc:/luma/titles/0004003000008F02/romfs/sleep_LZ.bin");
			ImGui::End();

			//ImGui::ShowStyleEditor();
			//ImGui::ShowDemoWindow();

		// bottom screen
		ImGui::SetNextWindowSize(ImVec2(width * 0.8f, height * 0.5f));
		ImGui::SetNextWindowPos(ImVec2(width * 0.1f, height * 0.5f), ImGuiCond_FirstUseEver);

			ImGui::Begin("Window Title (Top Screen Sleep)", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			static bool color_wheel = false;
			ImGuiColorEditFlags color_flags = color_wheel ? ImGuiColorEditFlags_PickerHueWheel : 0;

			ImGui::BeginChild("##ColorSelection", ImVec2(315.0f, 180.0f));
			ImGui::ColorPicker3ButtonSized("Background", top_sleep_background, color_flags);
			ImGui::Separator();
			ImGui::ColorPicker3ButtonSized("Background Glow", top_sleep_background_glow, color_flags);
			ImGui::Separator();
			ImGui::ColorPicker3ButtonSized("Background Stripes", top_sleep_background_stripes, color_flags);
			ImGui::Separator();
			ImGui::ColorPicker3ButtonSized("Header Color", top_sleep_header, color_flags);
			ImGui::Separator();
			ImGui::ColorPicker3ButtonSized("Text Color", top_sleep_text, color_flags);
			ImGui::Separator();
			ImGui::ColorPicker3ButtonSized("Footer Color", top_sleep_footer, color_flags);
			ImGui::Separator();
			ImGui::ColorPicker3ButtonSized("Line Color", top_sleep_line, color_flags);
			ImGui::Separator();
			ImGui::Checkbox("Color Wheel", &color_wheel);
			ImGui::EndChild();
			ImGui::Dummy(ImVec2(0.0f, 14.0f));
			if (ImGui::Button("Apply")) {
				writeColor(top_sleep_background, 0x00001CD4, uncompressed);
				writeColor(top_sleep_background_glow, 0x00001CD8, uncompressed);
				writeColor(top_sleep_background_stripes, 0x00001CD0, uncompressed);
				writeColor(top_sleep_header, 0x00001D70, uncompressed);
				writeColor(top_sleep_text, 0x00001DA4, uncompressed);
				writeColor(top_sleep_footer, 0x0001EA8, uncompressed);
				writeColor(top_sleep_line, 0x00001E24, uncompressed);
				FILE *source = fopen("sdmc:/luma/titles/0004003000008F02/romfs/sleep_LZ.bin","wb");
				CompressLzExFile(uncompressed, source);
				fclose(source);
			}
			//ImGui::SameLine();
			//ImGui::Button("Back");
			ImGui::SameLine();
			if (ImGui::Button("Default Colors")) { //Default Top Screen Menu Colors
				top_sleep_background = ImVec4(70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 255.0f / 255.0f);
				top_sleep_background_glow = ImVec4(60.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f, 255.0f / 255.0f);
				top_sleep_background_stripes = ImVec4(35.0f / 255.0f, 35.0f / 255.0f, 45.0f / 255.0f, 255.0f / 255.0f);
				top_sleep_header = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
				top_sleep_text = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
				top_sleep_footer = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
				top_sleep_line = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
			};
			ImGui::End();

		// render frame
		ImGui::Render();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		// clear frame/depth buffers
		C3D_RenderTargetClear(s_top, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
		C3D_RenderTargetClear(s_bottom, C3D_CLEAR_ALL, CLEAR_COLOR, 0);

		imgui::citro3d::render(s_top, s_bottom);

		C3D_FrameEnd(0);
	}

	// clean up resources
	imgui::citro3d::exit();

	// free graphics
	//Tex3DS_TextureFree(s_gfxT3x);
	//C3D_TexDelete(&s_gfxTexture);

	// free render targets
	C3D_RenderTargetDelete(s_bottom);
	C3D_RenderTargetDelete(s_top);

	// deinitialize citro3d
	C3D_Fini();

	gfxExit();
	acExit();

	ImGui::DestroyContext();
}


FileBuffer UncompressLz77(FILE* fp) {
	//bool ret = fp != nullptr;
	//if (!ret) return nullptr;

	fseek(fp, 0, SEEK_END);
	uint32_t uCompressedSize = static_cast<uint32_t>(ftell(fp));
	fseek(fp, 0, SEEK_SET);
	uint8_t* pCompressed = new uint8_t[uCompressedSize];
	fread(pCompressed, 1, uCompressedSize, fp);
	u32 uUncompressedSize = 0;
	/*bResult = */CLz77::GetUncompressedSize(pCompressed, uCompressedSize, uUncompressedSize);
	//Add error handling

	uint8_t* pUncompressed = new uint8_t[uUncompressedSize];
	/*bResult = */CLz77::Uncompress(pCompressed, uCompressedSize, pUncompressed, uUncompressedSize);
	//Add error handling
	FileBuffer file = {pUncompressed, uUncompressedSize};
	return file;
}

bool CompressLzExFile(FileBuffer input, FILE* output) {
	uint32_t uCompressedSize = CLz77::GetCompressBoundSize(input.bufferSize, 1);

	uint8_t* pCompressed = new uint8_t[uCompressedSize];

	CLz77::CompressLzEx(input.buffer, input.bufferSize, pCompressed, uCompressedSize, 1);
	if (output != nullptr) {
		fwrite(pCompressed, 1, uCompressedSize, output);
	} else {
		return false;
	} //else error
	return true;
}


//TEMPORARY - Ignore alpha values
void writeColor(ImVec4 color, unsigned int offset, FileBuffer file) {
	float max = 255.0f;
	file.buffer[offset] = (uint8_t)(round(color.x * max));
	file.buffer[offset + 1] = (uint8_t)(round(color.y * max));
	file.buffer[offset + 2] = (uint8_t)(round(color.z * max));
}

ImVec4 getColor(unsigned int offset, FileBuffer file) {
	float max = 255.0f;
	float red = (float)file.buffer[offset];
	float green = (float)file.buffer[offset + 1];
	float blue = (float)file.buffer[offset + 2];
	//float alpha = (float)file.buffer[offset + 3];
	float alpha = 255.0f;
	return ImVec4(red/max, green/max, blue/max, alpha/max);
}