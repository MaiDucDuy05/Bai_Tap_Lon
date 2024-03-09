#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include"Common_Function.h"

typedef struct GeometricFormat
{
	public:
	GeometricFormat(int l,int t,int w,int h){
		left=l;top=t;width=w;height=h;
	}
	int left;int top;int width;int height;
};
typedef struct ColorData
{
public:
	Uint8 red;
	Uint8 green;
	Uint8 blue;

	ColorData(Uint8 r,Uint8 g,Uint8 b){
		red=r;green=g;blue=b;
	}

};

class Geometric{
public:
	static void RenderRecttangle(const GeometricFormat& geo_size, const ColorData &color_data,SDL_Renderer* screen );
	static void RenderOutline(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen);
};


#endif