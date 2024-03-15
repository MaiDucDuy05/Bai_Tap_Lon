#pragma once
#ifndef THREAT_OBJECT_H_
#define THREAT_OBJECT_H_
#include<vector>
#include<queue>
#include<map>
#include"Common_Function.h"
#include"BaseObject.h"
#include"game_map.h"
#include"BulletObject.h"
#include"Geometric.h"
class Threat_Object :public BaseObject {
public:
	Threat_Object();
	~Threat_Object();
	void Search(int i, int j, const Map& map);

	void Set_goal_x(int x) {
		goal_x = x;
	}
	void Set_goal_y(int y) {
		goal_y = y;
	}
	void Move();
	bool kt(int,int);
	int get_status() {
		return status;
	}
	void set_bullet_list(std::vector<BulletObject*> list) {
		p_bullet_list = list;
	}
	std::vector<BulletObject*> get_bullet_list() {
		return p_bullet_list;
	}

	void HandleBuller(SDL_Renderer* des);
	void Remove_Bullet(const int& idx);
	void Init_Bullet(SDL_Renderer* des);
	void Set_blood(int x) {
		threat_blood = x;
	}
	int Get_blood() {
		return threat_blood;
	}
	void Draw_blood(SDL_Renderer* des);
	
private:
	std::map<std::pair<int, int>,std::pair<int, int>>parent;
	std::map<std::pair<int, int>, bool> vet;
	int goal_x; int goal_y;
	int dx[4]; int dy[4];
	int start_time;
	int end_time;
	int status;
	std::vector<BulletObject*> p_bullet_list;
	BaseObject Bullet;
	int random_huong;
	int threat_blood;
};

#endif // !THREAT_OBJECT_H_
