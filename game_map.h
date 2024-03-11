#ifndef GAME_MAP_H_
#define GAME_MAP_H_

#include"Common_Function.h"
#include"BaseObject.h"

#define MAX_TILE 20
class TileMat: public BaseObject{
	public:
		TileMat(){;}
		~TileMat(){;}
};
class GameMap{
public:
	//GameMap();
	//~GameMap();

	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer *screen);
	void LoadMap(char* name);

	Map getMap() const { return game_map;}
	void SetMap(Map &map_data){ game_map=map_data;}
	void clear_tile_map();

private:
	Map game_map;
	TileMat tile_mat[MAX_TILE];

};

#endif