#ifndef _CGRENDER_FONTS_H
#define _CGRENDER_FONTS_H

#include <string>
#include <vector>
#include <map>

struct Character;
namespace CGRender
{
	class CGRender_Fonts
	{
	public:
		CGRender_Fonts(int Device);
		~CGRender_Fonts();

		void loadFont(const std::wstring& face = L"/ARIALUNI.TTF");
		void deleteFont();

		void LoadFaces(const wchar_t* text, int size);

		void LoadCharacters(const std::wstring& text, std::map<wchar_t, Character>& charactors);

	private:
		CGRender_Fonts() = delete;
	private:
		struct PrivateData;
		PrivateData* m_priv = nullptr;
	};

	bool Font_Init(int Device);
	bool Font_Delete(int Device);
	bool Font_LoadFont(int Device, const std::wstring& face = L"/ARIALUNI.TTF");
	bool Font_LoadFaces(int Device, const std::wstring& text);
	bool Font_GetCharacters(int Device, const std::wstring& text, std::map<wchar_t, Character>& charactors);
}
#endif // !_CGRENDER_FONTS_H
