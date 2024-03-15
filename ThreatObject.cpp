#include"ThreatObject.h"
#include<iostream>
Threat_Object::Threat_Object() {
    random_huong = rand() % 4;
    if (random_huong == 0) {
        dx[0] = 1; dx[1] = -1; dx[2] = 0; dx[3] = 0;
        dy[0] = 0; dy[1] = 0; dy[2] = -1; dy[3] = 1;
    }
    else if (random_huong == 1) {
        dx[0] = -1; dx[1] = 1; dx[2] = 0; dx[3] = 0;
        dy[0] = 0; dy[1] = 0; dy[2] = 1; dy[3] = -1;
    }
    else if (random_huong == 2) {
        dx[3] = -1; dx[2] = 1; dx[1] = 0; dx[0] = 0;
        dy[3] = 0; dy[2] = 0; dy[1] = 1; dy[0] = -1;
    }
    else {
        dx[3] = 1; dx[2] = -1; dx[1] = 0; dx[0] = 0;
        dy[3] = 0; dy[2] = 0; dy[1] = -1; dy[0] = 1;
    }
    rect.x = 0; rect.y = 0; status = 0;
    start_time = 0; end_time = 30;
    threat_blood = 50;
    rect.w = 60; rect.h = 30;
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
                if (path.first >= parent[path].first) status = 1;
                else if(path.first < parent[path].first) status = 0;
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
void Threat_Object::HandleBuller(SDL_Renderer* des) {
    for (int i = 0; i < p_bullet_list.size(); i++) {
        BulletObject* p_bullet = p_bullet_list.at(i);
        if (p_bullet != NULL) {
            if (p_bullet->get_is_move() == true) {
                p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_bullet->Render(des);
            }
            else {
                p_bullet_list.erase(p_bullet_list.begin() + i);
                if (p_bullet != NULL) {
                    delete p_bullet;
                    p_bullet = NULL;
                }
                Init_Bullet(des);
            }
        }
    }
}
void Threat_Object::Remove_Bullet(const int& idx) {
    int size_amo = p_bullet_list.size();
    if (size_amo > 0 && size_amo > idx) {
        BulletObject* p_amo = p_bullet_list.at(idx);
        p_bullet_list.erase(p_bullet_list.begin() + idx);
        if (p_amo != NULL) {
            delete p_amo; p_amo = NULL;
        }
    }
}
void Threat_Object::Init_Bullet(SDL_Renderer* des) {
    BulletObject* p_bullet = new BulletObject;
    p_bullet->LoadImag("img//bullet_man.png", des);
    p_bullet->set_is_move(true);
    p_bullet->SetRect(this->rect.x, this->rect.y);
    if (status == 0) p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
    else p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
    p_bullet->set_x_val(20);
    p_bullet_list.push_back(p_bullet);
}
void Threat_Object::Draw_blood(SDL_Renderer  * des) {
    GeometricFormat rectangle_size1(rect.x,rect.y-10,threat_blood, 5);
    ColorData color_data1(255,0, 0);
    Geometric::RenderRecttangle(rectangle_size1, color_data1, des);
}
