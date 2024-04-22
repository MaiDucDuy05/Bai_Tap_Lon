#ifndef MAIN_P2_OBJECT_H_
#define MAIN_P2_OBJECT_H_
#include<vector>
#include"BaseObject.h"
#include"Common_Function.h"
#include"BulletObject.h"
#include"ImpTimer.h"
#define  GRAVITY_SPEED 2
#define MAX_FALL_SPEED 10 
#define PLAYER_SPEED 8
#define PLAYER_SPEED_UP 30
class Main_P2_Object : public BaseObject
{
public:
	Main_P2_Object();
	~Main_P2_Object();

	enum WalkType{
		WALK_RIGHT=0,
		WALK_LEFT=1,
	};

	bool LoadImag(std::string path,SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void Show_Defend(SDL_Renderer* des);
	void HandeInputAction(SDL_Event events,SDL_Renderer * screen,Mix_Chunk*g_sound);
	void Set_clip();
	void Doplayer(Map &map_data);
	void Check_map(Map &map_data);
	void SetMapXY(const int x,const int y) {map_x=x;map_y=y;}

	Input Get_Input_type(){
		return input_type;
	}
	void set_bullet_list(std::vector<BulletObject*> list){
		p_bullet_list=list;
	}
	std::vector<BulletObject*> get_bullet_list(){
		return p_bullet_list;
	}

	void HandleBuller(SDL_Renderer * des);

	void Remove_Bullet(const int &idx);

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
	int get_status() {
		return status;
	}
	void set_status(int x) {
		status = x;
	}
	void ktImage(SDL_Renderer* des);
	void Auto_(SDL_Rect Vitri, Input input, SDL_Renderer* screen, Mix_Chunk* g_sound);
	void exchange_input(const int x, SDL_Renderer* screen);
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
	BaseObject p_object_defend;
	std::vector<BulletObject*> p_bullet_list;
	
	
	int blood_main;
	int ki_main;
	BaseObject P_Image[13];
	

	ImpTimer Time_Delay[10];
	bool SKILL_U;

	bool is_wall;
};

#endif