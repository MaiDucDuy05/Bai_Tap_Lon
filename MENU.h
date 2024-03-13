#pragma once
#ifndef MENU_H_
#define MENU_H_
#include"Common_Function.h"
#include"BaseObject.h"
#include<string>

#define MENU_WIDTH 1280
#define MNEU_HIGH 640

class MenuObject : public BaseObject {
private:
	SDL_Color textColor[10];
	SDL_Surface* textSurface[10];
	SDL_Texture* textTexture[10];
	SDL_Rect dstRect[10];
public:
	int ShowMenu(SDL_Renderer* des, TTF_Font* font,const std::string DANH_SACH[],const int So_luong,const int x[],const int y[],Mix_Chunk*);
};
#endif