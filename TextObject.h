#pragma once
#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_
#include"BaseObject.h"
#include<string>
class TextObject : public BaseObject {
public:
	enum TextColor
	{
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2
	};
	TextObject();
	~TextObject();
	void SetText(const std::string& s) { str_val = s; };
	void SetColor(const int& type);

	void CreateGameText(TTF_Font* font, SDL_Renderer* des);
private:

	std::string str_val;
	SDL_Color text_color;
};


#endif