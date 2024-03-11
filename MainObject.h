#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_
#include<vector>
#include"BaseObject.h"
#include"Common_Function.h"
#include"BulletObject.h"


#define  GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10 
#define PLAYER_SPEED 8
#define PLAYER_SPEED_UP 30
class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType{
		WALK_RIGHT=0,
		WALK_LEFT=1,
	};

	bool LoadImag(std::string path,SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandeInputAction(SDL_Event events,SDL_Renderer * screen,Mix_Chunk*g_sound);
	void Set_clip();
	void Doplayer(Map &map_data);
	void Check_map(Map &map_data);
	void SetMapXY(const int x,const int y) {map_x=x;map_y=y;}
	void set_bullet_list(std::vector<BulletObject*> list){
		p_bullet_list=list;
	}
	std::vector<BulletObject*> get_bullet_list(){
		return p_bullet_list;
	}

	void HandleBuller(SDL_Renderer * des);

	Input Get_Input_type(){
		return input_type;
	}
	void Show_Bullet_Size(SDL_Renderer * screen);
	void Show_Defend(SDL_Renderer* screen);
	void Show_Bullet_Skill_U(SDL_Renderer * screen);

	void Set_Move_U(const bool u){
		Move_U=u;
	}
	bool Get_Move_u(){
		return Move_U;
	}
	void Buller_move_U(Map,int);
	void MainObject::Remove_Bullet(const int &x);

	BaseObject Get_Bullet_Skill_U(const int &i){
		if(i==0||i==1)
		return Bullet_Skill_U[i];
	}
	BaseObject Get_Bullet_BigSize(){
		return Bullet_BigSize;
	}
	void Setinput_hurt(const int & x){
		input_type.hurt=x;
	}

	int Get_blood_main(){
		return blood_main;
	}

	void Set_blood_main(int x){
		blood_main=x;
	}
	int Get_ki_main() {
		return ki_main;
	}

	void Set_ki_main(int x) {
		ki_main = x;
	}
private:
	//
	float x_val;
	float y_val;

	float x_pos;
	float y_pos;

	int width_frame_;
	int height_frame;

	SDL_Rect frame_clip[8];

	Input input_type;
	int frame;
	int status;

	bool  on_ground;
	int map_x;
	int map_y;
	//
	std::vector<BulletObject*> p_bullet_list;

	BaseObject Bullet_BigSize;
	BaseObject p_object_defend;
	BaseObject Bullet_Skill_U[2];
	bool Move_U; 

	int blood_main;
	int ki_main;
};



#endif