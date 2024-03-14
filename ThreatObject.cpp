#include"ThreatObject.h"
#include<iostream>
Threat_Object::Threat_Object() {
    dx[0] = 1; dx[1] = -1; dx[2] = 0; dx[3] = 0;
    dy[0] = 0; dy[1] = 0; dy[2] = -1; dy[3] = 1;
    rect.x = 1000; rect.y = 640;
    start_time = 0; end_time = 30;
}
Threat_Object::~Threat_Object() {

}
bool Threat_Object::kt(int i,int j) {
    std::pair<int, int>my_pair = std::make_pair(i,j);
    std::pair<int, int>path = std::make_pair(goal_x, goal_y);
    if (my_pair == path)
        return false;
    return true;
}
void Threat_Object::Search(int i, int j, const Map& map) {
    if (kt(i,j) == false) return;
   std:: queue<std::pair<int, int>>qe;
   std::pair<int, int>my_pair = std::make_pair(i,j);
    vet[my_pair] = true; std::pair<int,int>v;
    qe.push(my_pair);
    while (!qe.empty()) {
        v = qe.front(); qe.pop();
        for (int k = 0; k < 4; k++) {
            int i1 = v.first + dx[k]; int j1 = v.second + dy[k];
            std::pair<int, int>my_pair_1 = std::make_pair(i1,j1);
            std::pair<int, int>my_pair_2 = std::make_pair(v.first,v.second);
            if (i1 >= 0 && j1 >= 0 && i1 < MAX_MAP_X && j1 < MAX_MAP_Y && vet[my_pair_1] == false
                &&map.tile[j1][i1]==BLANK_TILE) {
                qe.push(my_pair_1); parent[my_pair_1] = my_pair_2;
                std::pair<int, int>my_pair_3 = std::make_pair(goal_x,goal_y);
                if ( vet[my_pair_3] ==true ) {
                    Move();
                    return;
                }
                vet[my_pair_1] = true;
            }
        }
    }
    for (auto& pair : vet) {
        pair.second = false;
    }
}
void Threat_Object::Move() {
    Uint32 TIME = SDL_GetTicks()/100;
    std::pair<int, int>my_pair = std::make_pair(rect.x / TILE_SIZE, rect.y / TILE_SIZE);
    std::pair<int, int>path = std::make_pair(goal_x, goal_y);
    start_time = TIME;
    while (1) {
        if (parent[path] == my_pair) {
            if (start_time >= end_time) {
                rect.x = path.first * TILE_SIZE; rect.y = path.second * TILE_SIZE;
                 end_time = start_time +5; 
            }
            break;
        }path = parent[path];
    }
    for (auto& pair : vet) {
        pair.second = false;
    }
}

