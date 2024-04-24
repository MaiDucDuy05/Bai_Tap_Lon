#include"BulletObject.h"

BulletObject::BulletObject(){
	x_val=0;
	y_val=0;
	is_move= false;
	bullet_dir=DIR_RIGHT;
}

BulletObject::~ BulletObject(){

}

void BulletObject::HandleMove(const int& x_border, const int & y_border){
	if(bullet_dir==DIR_RIGHT){
	rect.x+=x_val;
	if(rect.x>x_border){
		is_move=false;
		}
	}
	else{
		rect.x-=x_val;
	if(rect.x<0){
			is_move=false;
		}
	}
}
bool BulletObject::Check_Map(Map map_data) {
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	int height_min = rect.h < TILE_SIZE ? rect.h : TILE_SIZE;

	x1 = (rect.x + x_val) / TILE_SIZE;
	x2 = (rect.x + x_val + rect.w - 1) / TILE_SIZE;

	y1 = (rect.y) / TILE_SIZE;
	y2 = (rect.y + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if (x_val > 0) {
			if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE) {
				return true;
			}
		}
		else if (x_val < 0) {
			if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE) {
				return true;
			}
		}
	}
	return false;
}