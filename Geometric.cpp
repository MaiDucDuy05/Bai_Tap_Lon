#include"Geometric.h"

void Geometric::RenderRecttangle(const GeometricFormat& geo_size, const ColorData &color_data,SDL_Renderer* screen){
	SDL_Rect fill_rect  = {geo_size.left,geo_size.top,geo_size.width,geo_size.height};
	SDL_SetRenderDrawColor(screen,color_data.red,color_data.green,color_data.blue,255);
	SDL_RenderFillRect(screen,&fill_rect);

}
void Geometric:: RenderOutline(const GeometricFormat& geo_size, const ColorData& color_data, SDL_Renderer* screen) {
	SDL_Rect fill_rect = { geo_size.left,geo_size.top,geo_size.width,geo_size.height };
	SDL_SetRenderDrawColor(screen, color_data.red, color_data.green, color_data.blue, 255);
	SDL_RenderDrawRect(screen, &fill_rect);
}