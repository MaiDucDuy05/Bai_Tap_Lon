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
#include"ImpTimer.h"
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
	bool get_hoisinh() { return Hoi_sinh; }
	void set_hoisinh(bool h) {
		Hoi_sinh = h; Time_hoisinh.start();
	}
	void HOI_SINH();
	void Load_Threat(SDL_Renderer* screen);
	void Render_Threat(SDL_Renderer* screen,int STT);

	bool Get_is_threat() {
		return is_Threat;
		}
	void Set_is_threat(bool b) {
		is_Threat = b;
	}
private:
	std::map<std::pair<int, int>,std::pair<int, int>>parent;
	std::map<std::pair<int, int>, bool> vet;
	int goal_x; int goal_y;
	int dx[4]; int dy[4];
	
	ImpTimer Time_Delay;

	int status;
	std::vector<BulletObject*> p_bullet_list;
	BaseObject Bullet;
	int random_huong;
	int threat_blood;
	bool Hoi_sinh;
	ImpTimer Time_hoisinh;
	BaseObject Iamge_Threatr[4];
	BaseObject Iamge_Threatl[4];

	bool is_Threat;

};

#endif // !THREAT_OBJECT_H_
