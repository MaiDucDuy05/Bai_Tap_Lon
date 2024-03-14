#pragma once
#ifndef THREAT_OBJECT_H_
#define THREAT_OBJECT_H_
#include<vector>
#include<queue>
#include<map>
#include"Common_Function.h"
#include"BaseObject.h"
#include"game_map.h"

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
private:
	std::map<std::pair<int, int>, std::pair<int, int>>parent;
	std::map<std::pair<int, int>, bool> vet;
	int goal_x; int goal_y;
	int dx[4]; int dy[4];
	int start_time;
	int end_time;
};

#endif // !THREAT_OBJECT_H_
