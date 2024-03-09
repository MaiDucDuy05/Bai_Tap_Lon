#include"TextObject.h"

TextObject::TextObject() {
	rect.x = 500;
	rect.y = 10;
	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;
}
TextObject::~TextObject() {
}

void TextObject::SetColor(const int& type) {
	if (type == RED_TEXT) {
		SDL_Color color = { 255,0,0 };
		text_color = color;
	}
	else if (type == WHITE_TEXT) {
		SDL_Color color = { 255,255,255 };
		text_color = color;
	}
	else {
		SDL_Color color = { 0,0,0 };
		text_color = color;
	}
}

void TextObject::CreateGameText(TTF_Font* font, SDL_Renderer* des) {
	SDL_Surface * textSurface = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(des, textSurface);
	p_object = textTexture;
	Render(des);
}