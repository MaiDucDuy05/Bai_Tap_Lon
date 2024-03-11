#include"Main_P2_Object.h"

#define PLAYER_JUMP 30

Main_P2_Object::Main_P2_Object(){
	frame =0 ;
	x_pos=SCREEN_WIDTH-100;
	y_pos=0;
	x_val=0;
	y_val=0;
	width_frame_=0;
	height_frame=0;
	status=1;
	input_type.down=0;input_type.jump=0;
	input_type.left=0;input_type.right=0;input_type.up=0;
	input_type.bullet_Skill_U=0; input_type.bullet_Skill_J=0;
	input_type.defend=0;input_type.bullet_Skill_I=0;
	input_type.speed_up=0;input_type.hurt=0;
	on_ground = false; blood_main = 500; ki_main = 1499;
	map_x=0;map_y=0;
}
Main_P2_Object::~Main_P2_Object(){

}
bool Main_P2_Object::LoadImag(std::string path,SDL_Renderer* screen){
	bool ret = BaseObject::LoadImag(path,screen);
	if(ret==true){
		width_frame_=rect.w/8;
		height_frame=rect.h;
	}
	return ret;
}
void Main_P2_Object::Set_clip(){
	if(width_frame_>0&&height_frame>0){
		frame_clip[0].x=0;
		frame_clip[0].y=0;
		frame_clip[0].h=height_frame;
		frame_clip[0].w=width_frame_;

		frame_clip[1].x=1*width_frame_;
		frame_clip[1].y=0;
		frame_clip[1].h=height_frame;
		frame_clip[1].w=width_frame_;

		frame_clip[2].x=2*width_frame_;
		frame_clip[2].y=0;
		frame_clip[2].h=height_frame;
		frame_clip[2].w=width_frame_;

		frame_clip[3].x=3*width_frame_;
		frame_clip[3].y=0;
		frame_clip[3].h=height_frame;
		frame_clip[3].w=width_frame_;

		frame_clip[4].x=4*width_frame_;
		frame_clip[4].y=0;
		frame_clip[4].h=height_frame;
		frame_clip[4].w=width_frame_;

		frame_clip[5].x=5*width_frame_;
		frame_clip[5].y=0;
		frame_clip[5].h=height_frame;
		frame_clip[5].w=width_frame_;

		frame_clip[6].x=6*width_frame_;
		frame_clip[6].y=0;
		frame_clip[6].h=height_frame;
		frame_clip[6].w=width_frame_;

		frame_clip[7].x=7*width_frame_;
		frame_clip[7].y=0;
		frame_clip[7].h=height_frame;
		frame_clip[7].w=width_frame_;
	}
}

void Main_P2_Object::Show(SDL_Renderer *des){
	if(status==WALK_LEFT){
		if(input_type.hurt==1){
			LoadImag("img//Sieunhan(die)Right.png",des);
		}
		else if(input_type.speed_up==1){
			LoadImag("img//SieunhantocbienLeft.png",des);
		}
		else if(input_type.bullet_Skill_U==1){
			LoadImag("img//Sieunhan(U)Left.png",des);
		}
		else if(input_type.bullet_Skill_J==1){
			LoadImag("img//Sieunhan(J)Left.png",des);
		}
		else if(input_type.bullet_Skill_I==1){
			LoadImag("img//Sieunhan(I)Left.png",des);
		}
		else {
		LoadImag("img//SieunhanLeft.png",des);
		}
	}
	else if(status==WALK_RIGHT ){
		if(input_type.hurt==1){
			LoadImag("img//Sieunhan(die)Left.png",des);
		}
		else if(input_type.speed_up==1){
			LoadImag("img//SieunhantocbienRight.png",des);
		}
		else if(input_type.bullet_Skill_U==1){
			LoadImag("img//Sieunhan(U)Right.png",des);
		}
		else if(input_type.bullet_Skill_J==1){
			LoadImag("img//Sieunhan(J)Right.png",des);
		}
		else if(input_type.bullet_Skill_I==1){
			LoadImag("img//Sieunhan(I)Right.png",des);
		}
		else{
		LoadImag("img//SieunhanRight.png",des);
		}
	}
	if(input_type.left==1||input_type.right==1||input_type.speed_up==1||input_type.bullet_Skill_U==1||
		input_type.bullet_Skill_J==1||input_type.bullet_Skill_I==1||input_type.hurt==1){
		frame++;
		if(frame==8&&input_type.hurt==1){
			input_type.hurt=0;SDL_Delay(300);
		}
	}
	else frame=0;
	if(frame>=8) frame=0;
	
	if(input_type.bullet_Skill_U==0){
	rect.x=x_pos-map_x;
	rect.y=y_pos-map_y;
	}
	SDL_Rect* current_clip = &frame_clip[frame];

		SDL_Rect renderQuad ={rect.x,rect.y,width_frame_,height_frame};
		SDL_RenderCopy(des,p_object,current_clip,&renderQuad);

}

void Main_P2_Object::HandeInputAction(SDL_Event events,SDL_Renderer * screen,Mix_Chunk*g_sound){
	if(events.type==SDL_KEYDOWN){
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
			status = WALK_RIGHT;
			input_type.right=1;
			input_type.left=0;
			break;
		case SDLK_LEFT:
			status = WALK_LEFT;
			input_type.left=1;
			input_type.right=0;
			break;
		case SDLK_UP:
			input_type.jump=1;
			break;
		case SDLK_DOWN:
			if(on_ground&&ki_main>=100){
			input_type.defend=1;
			if(status==WALK_LEFT){
			p_object_defend.LoadImag("img//SuppermenphongthuLeft.png",screen);
			p_object_defend.SetRect(this->rect.x, this->rect.y );
			}
			else{
			p_object_defend.LoadImag("img//SuppermenphongthuRight.png",screen);
			p_object_defend.SetRect(this->rect.x, this->rect.y );
			}
			}
			break;
		case SDLK_KP_2:
			if(on_ground){
				input_type.speed_up=1;
				break;
			}
		case SDLK_KP_4:
			if (ki_main >= 200&& input_type.bullet_Skill_U == 0)
				input_type.bullet_Skill_U=1;
			break;
		case SDLK_KP_6:
			if(ki_main>=300&& input_type.bullet_Skill_I == 0)
				input_type.bullet_Skill_I=1;
			break;
		case SDLK_KP_1:
			input_type.bullet_Skill_J=1;
			BulletObject *p_bullet = new BulletObject;
			if(input_type.defend==0){
				ki_main += 50;
			if(status== WALK_LEFT){
				p_bullet->LoadImag("img//KiemL.png",screen);
				}
			else{
				p_bullet->LoadImag("img//KiemR.png",screen);
			}
			if(status==WALK_LEFT){
				p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
				p_bullet->SetRect(this->rect.x-20, this->rect.y + height_frame*0.3);
			}
			else{
				p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
				p_bullet->SetRect(this->rect.x+width_frame_-20, this->rect.y + height_frame*0.3);
			}
			Mix_PlayChannel(-1,g_sound,0);
			p_bullet->set_x_val(20);
			p_bullet->set_is_move(true);
			p_bullet_list.push_back(p_bullet);
			}
			break;
		}
	}
	else if(events.type==SDL_KEYUP){
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
			input_type.right=0;
			break;
		case SDLK_LEFT:
			input_type.left=0;
			break;
		case SDLK_UP:
			input_type.jump=0;
			break;
		case SDLK_DOWN:
			input_type.defend=0;
			break;
		case SDLK_KP_2:
			input_type.speed_up=0;
			break;
		case SDLK_KP_4:
			input_type.bullet_Skill_U=0;
			break;
		case SDLK_KP_6:
			input_type.bullet_Skill_I=0;
			break;
		case SDLK_KP_1:
			input_type.bullet_Skill_J=0;
			break;
		default:
			break;
		}
	}
}

void Main_P2_Object:: HandleBuller(SDL_Renderer * des){
	for(int i=0;i<p_bullet_list.size();i++){
		BulletObject * p_bullet = p_bullet_list.at(i);
		if(p_bullet!=NULL){
			if(p_bullet->get_is_move()==true){
			p_bullet->HandleMove(SCREEN_WIDTH,SCREEN_HEIGHT);
			p_bullet->Render(des);
			}
			else{
				p_bullet_list.erase(p_bullet_list.begin()+i);
				if(p_bullet!=NULL){
					delete p_bullet;
					p_bullet= NULL;
				}
			}
		}
	}
}
void Main_P2_Object:: Doplayer(Map &map_data){
	x_val=0;
	y_val+= GRAVITY_SPEED;

	if(y_val>= MAX_FALL_SPEED ){
		y_val=MAX_FALL_SPEED;
	}
	if(input_type.left==1){
		x_val-=PLAYER_SPEED;
	}
	else if(input_type.right==1){
		x_val+=PLAYER_SPEED;
	}
	if(input_type.speed_up==1){
			if(status==WALK_LEFT) x_val-=PLAYER_SPEED_UP;
			else x_val+=PLAYER_SPEED_UP;
		}
	else if(input_type.bullet_Skill_I==1){
		if(status==WALK_LEFT) x_val-=30;
		else x_val+=30;
	}
	else if(input_type.hurt==1){
		if(status==WALK_LEFT) x_val+=30;
		else x_val-=30;
	}
	if(input_type.jump==1){
		if(on_ground) y_val-= PLAYER_JUMP;
		input_type.jump=0;
	}
	Check_map(map_data);
}
void Main_P2_Object:: Check_map(Map &map_data){
	int x1=0;
	int x2=0;

	int y1=0;
	int y2=0;

	int height_min = height_frame < TILE_SIZE ? height_frame: TILE_SIZE;

	x1=(x_pos + x_val)/TILE_SIZE;
	x2=(x_pos + x_val+width_frame_-1)/TILE_SIZE;

	y1 = (y_pos)/TILE_SIZE;
	y2 = (y_pos +height_min-1)/TILE_SIZE;

	if(x1>=0&&x2<MAX_MAP_X&&y1>=0&&y2<MAX_MAP_Y){
		if(x_val>0){
			if(map_data.tile[y1][x2]!=BLANK_TILE||map_data.tile[y2][x2]!=BLANK_TILE){
				x_pos = x2*TILE_SIZE;
				x_pos -= width_frame_+1;
				x_val=0;
			}
		}
		else if(x_val<0){
			if(map_data.tile[y1][x1]!=BLANK_TILE||map_data.tile[y2][x1]!=BLANK_TILE){
				x_pos = (x1+1)*TILE_SIZE;
				x_val=0;
			}
		}
	}
	
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ :TILE_SIZE;
	x1=(x_pos)/TILE_SIZE;
	x2=(x_pos+width_min)/TILE_SIZE;

	y1=(y_pos+y_val) /TILE_SIZE;
	y2=(y_pos+y_val+ height_frame-1)/TILE_SIZE;

	if(x1>=0&&x2<MAX_MAP_X&&y1>=0&&y2<=MAX_MAP_Y){
		if(y_val>0){
			if(map_data.tile[y2][x1]!=BLANK_TILE||map_data.tile[y2][x2]!=BLANK_TILE){
				y_pos = y2 * TILE_SIZE;
				y_pos -= (height_frame+1);
				y_val=0;
				on_ground=true;
			}
		}
		else if (y_val<0) {
			on_ground=false;
			/*if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE) {
				y_pos = (y1+1)*TILE_SIZE;
				y_val = 0;
			
			}*/
		}
	}
	x_pos+=x_val;
	y_pos+=y_val;
	if(x_pos < 0) {
		x_pos=0;
	}
	else if(x_pos+width_frame_>map_data.max_x){
		x_pos= map_data.max_x-width_frame_-1;
	}

	if(y_pos >= map_data.max_y ){
		y_pos = map_data.max_y;
		on_ground = true;
	}
}
void Main_P2_Object::Remove_Bullet(const int& idx) {
	int size_amo = p_bullet_list.size();
	if (size_amo > 0 && size_amo > idx) {
		BulletObject* p_amo = p_bullet_list.at(idx);
		p_bullet_list.erase(p_bullet_list.begin() + idx);
		if (p_amo != NULL) {
			delete p_amo; p_amo = NULL;
		}
	}

}