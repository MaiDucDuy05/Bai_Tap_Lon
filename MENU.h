#pragma once
#ifndef MENU_H_
#define MENU_H_
#include"Common_Function.h"
#include"BaseObject.h"
#include<string>

#define MENU_WIDTH 1280
#define MNEU_HIGH 640

class MenuObject : public BaseObject {
public:
	int ShowMenu(SDL_Renderer* des, TTF_Font* font, std::string DANH_SACH[], int So_luong, int x[],int y[]);
};
#endif