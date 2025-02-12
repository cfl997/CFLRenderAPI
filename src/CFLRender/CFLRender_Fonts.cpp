#include "CFLRender_Fonts.h"
#include "CFLRenderAPI.h"
#include "CFLPath.h"


#include <ft2build.h>
#include FT_GLYPH_H

#include <iostream>
#include <map>


using namespace CGRender;


static const int MAX_SHORT_VALUE = 65536;

struct CFLRenderGL_Fonts::PrivateData
{
	int Device = -1;
	std::map<wchar_t, Character> Characters;

	FT_Library ft;
	FT_Face face;

	std::wstring fontsName = L"";

};
CFLRenderGL_Fonts::CFLRenderGL_Fonts(int Device) :m_priv(new PrivateData)
{
	auto& d = *m_priv;
	d.Device = Device;

	if (FT_Init_FreeType(&d.ft))
	{
		assert(0);
		std::cout << "FREETYPE: Could not init FreeType Library" << std::endl;
	}
}

CFLRenderGL_Fonts::~CFLRenderGL_Fonts()
{
	auto& d = *m_priv;
	FT_Done_FreeType(d.ft);

	for (auto iter = d.Characters.begin(); iter != d.Characters.end(); iter++)
	{
		CFLRenderGL_DeleteTexture(d.Device, iter->second.TextureID);
	}
	deleteFont();
}

void CGRender::CFLRenderGL_Fonts::loadFont(const std::wstring& font)
{
	auto& d = *m_priv;
	if (d.fontsName == font)
		return;


	if (d.fontsName != L"" && d.fontsName != font)
	{
		deleteFont();
	}
	// Load font as face
	std::wstring pathstr = CGPath_GetPath(CGPathType::CG_PATH_FONTS);
	std::wstring fontPath = pathstr + font;

	if (FT_New_Face(d.ft, wstr2utf8(fontPath).c_str(), 0, &d.face))
	{
		assert(0);
		std::cout << " FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(d.face, g_FontPixelSize, g_FontPixelSize);
	FT_Select_Charmap(d.face, ft_encoding_unicode);

	d.fontsName = font;


}

void CGRender::CFLRenderGL_Fonts::deleteFont()
{
	auto& d = *m_priv;
	assert(0);
	FT_Done_Face(d.face);
}

void CGRender::CFLRenderGL_Fonts::LoadFaces(const wchar_t* text, int size)
{
	auto& d = *m_priv;

	for (int i = 0; i < size; ++i) {
		if (d.Characters.find(text[i]) != d.Characters.end())
			continue;
		if (FT_Load_Glyph(d.face, FT_Get_Char_Index(d.face, text[i]), FT_LOAD_DEFAULT))
		{
			assert(0);
			std::cout << "FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		FT_Glyph glyph;
		FT_Get_Glyph(d.face->glyph, &glyph);

		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		int texture = CFLRenderGL_CreateTextureFromData(d.Device, bitmap.buffer, bitmap.width, bitmap.rows, GLTextureType::GLTexture_RED);
		Character character = {
				(uint32_t)texture,
				glm::ivec2(d.face->glyph->bitmap.width, d.face->glyph->bitmap.rows),
				glm::ivec2(d.face->glyph->bitmap_left, d.face->glyph->bitmap_top),
				static_cast<unsigned int>((glyph->advance.x / MAX_SHORT_VALUE) << 6)//must
		};
		d.Characters.insert(std::pair<wchar_t, Character>(text[i], character));
	}
}

void CGRender::CFLRenderGL_Fonts::LoadCharacters(const std::wstring& text, std::map<wchar_t, Character>& charactors)
{
	auto& d = *m_priv;
	charactors.clear();
	for (int i = 0; i < text.size(); i++)
	{
		auto c = text.c_str()[i];
		charactors[c] = d.Characters[text.c_str()[i]];
	}
}


struct g_FontS
{
	std::map<int, CFLRenderGL_Fonts*>m_fonts;
};
g_FontS gfont;
bool CGRender::Font_Init(int Device)
{
	if (gfont.m_fonts.find(Device) != gfont.m_fonts.end())
		return true;
	CFLRenderGL_Fonts* fonts = new CFLRenderGL_Fonts(Device);
	gfont.m_fonts[Device] = fonts;
	return true;
}

bool CGRender::Font_Delete(int Device)
{
	if (gfont.m_fonts[Device] == nullptr)
		return true;
	auto font = gfont.m_fonts[Device];
	delete font;
	font = nullptr;
	gfont.m_fonts.erase(Device);
	return true;
}
bool CGRender::Font_LoadFont(int Device, const std::wstring& face)
{
	auto font = gfont.m_fonts[Device];
	if (!font)
		return false;

	font->loadFont(face);
	return true;
}

bool CGRender::Font_LoadFaces(int Device, const std::wstring& text)
{
	auto font = gfont.m_fonts[Device];
	if (!font)
		return false;

	font->LoadFaces(text.c_str(), text.size());
	return true;
}
bool CGRender::Font_GetCharacters(int Device, const std::wstring& text, std::map<wchar_t, Character>& charactors)
{
	auto font = gfont.m_fonts[Device];
	if (!font)
		return false;
	charactors.clear();
	font->LoadFaces(text.c_str(), text.size());

	font->LoadCharacters(text, charactors);
	return true;
}