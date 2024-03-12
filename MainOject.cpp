#include"MainObject.h"

#define PLAYER_JUMP 25

MainObject::MainObject(){
	frame =0 ;
	x_pos=0;
	y_pos=0;
	x_val=0;
	y_val=0;
	width_frame_=0;
	height_frame=0;
	status=0;
	input_type.down=0;input_type.jump=0;
	input_type.left=0;input_type.right=0;input_type.up=0;
	input_type.bullet_Skill_I=0; input_type.defend=0;
	input_type.speed_up=0; input_type.bullet_Skill_U=0;
	on_ground= false; Move_U=false;input_type.hurt=0;
	map_x = 0; map_y = 0; blood_main = 500; ki_main = 1499;
}
MainObject::~MainObject(){

}
bool MainObject::LoadImag(std::string path,SDL_Renderer* screen){
	bool ret = BaseObject::LoadImag(path,screen);
	if(ret==true){
		width_frame_=rect.w/8;
		height_frame=rect.h;
	}
	return ret;
}
void MainObject::Set_clip(){
	if(width_frame_>0&&height_frame>0){
		for (int i = 0; i <= 7; i++) {
			frame_clip[i].x = i * width_frame_;
			frame_clip[i].y = 0;
			frame_clip[i].h = height_frame;
			frame_clip[i].w = width_frame_;
		}
	}
}
void MainObject::ktImage(SDL_Renderer* des) {
	P_Image[0].LoadImag("img//Suppermen(die)Right.png", des);
	P_Image[1].LoadImag("img//Suppermen(I)left.png", des);
	P_Image[2].LoadImag("img//SuppermenTocbienLeft.png", des);
	P_Image[3].LoadImag("img//Suppermen(U)Left.png", des);
	P_Image[4].LoadImag("img//Suppermenleft.png", des);

	P_Image[5].LoadImag("img//Suppermen(die)Left.png", des);
	P_Image[6].LoadImag("img//Suppermen(I)right.png", des);
	P_Image[7].LoadImag("img//SuppermenTocbienRight.png", des);
	P_Image[8].LoadImag("img//Suppermen(U)Right.png", des);
	P_Image[9].LoadImag("img//Suppermenright.png", des);

	Bullet_Skill_U[0].LoadImag("img//Sunglon.png", des);
	Bullet_Skill_U[1].LoadImag("img//Danroi.png", des);
	Bullet_BigSize.LoadImag("img//DanbanLeft.png", des);
	Bullet_BigSize.LoadImag("img//DanbanRight.png", des);


}
void MainObject::Show(SDL_Renderer *des){
	int i = -1;
	input_type.empty = 0;
	if(status==WALK_LEFT){
		if(input_type.hurt==1){
			i = 0;
		}
		else if(input_type.bullet_Skill_I==1){
			i = 1;
		}
		else if(input_type.speed_up==1){
			i = 2;
		}
		else if(input_type.bullet_Skill_U==1){
			i = 3;
		}
		else if(input_type.left == 1) {
			i = 4;
		}
		else {
			input_type.empty = 1;
		}
	}
	else if(status==WALK_RIGHT ){
		if(input_type.hurt==1){
			i = 5;

		}
		else if(input_type.bullet_Skill_I==1){
			i = 6;
			
		}
		else if(input_type.speed_up==1){
			i = 7;
		}
		else if(input_type.bullet_Skill_U==1){
			i = 8;	
		}
		else if(input_type.right == 1){
			i = 9;
		}
		else {
			input_type.empty = 1;
		}
	}
	if(!Move_U){
		Bullet_Skill_U[1].SetRect(-200,-200);
	}
	if(input_type.left==1||input_type.right==1||input_type.bullet_Skill_I==1
		||input_type.speed_up==1||input_type.hurt==1){
		frame++;
		if(frame==8&&input_type.hurt==1){
			input_type.hurt=0;SDL_Delay(300);
		}
	}
	else frame=0;
	if(frame>=8)frame=0;
	
	rect.x=x_pos-map_x;
	rect.y=y_pos-map_y;
	if (i >= 0) {
		if (input_type.bullet_Skill_U == 0) {
			SDL_Rect* current_clip = &frame_clip[frame];

			SDL_Rect renderQuad = { rect.x,rect.y,width_frame_,height_frame };

			SDL_RenderCopy(des,P_Image[i].get_p_object(), current_clip, &renderQuad);
		}
		else {
			SDL_Rect renderQuad = { rect.x,rect.y,60,95 };
			SDL_RenderCopy(des,P_Image[i].get_p_object(), NULL, &renderQuad);
		}
	}
	SDL_DestroyTexture(get_p_object());
}
void MainObject::HandeInputAction(SDL_Event events,SDL_Renderer * screen,Mix_Chunk*g_sound){
	if(events.type==SDL_KEYDOWN){
		switch (events.key.keysym.sym)
		{
		case SDLK_d:
			status = WALK_RIGHT;
			input_type.right=1;
			input_type.left=0;
			break;
		case SDLK_a:
			status = WALK_LEFT;
			input_type.left=1;
			input_type.right=0;
			break;
		case SDLK_w:
			input_type.jump=1;
			break;
			case SDLK_i:
				if (ki_main >= 500&&input_type.bullet_Skill_I==0) {
					input_type.bullet_Skill_I = 1;
					if (status == WALK_LEFT) {
						//Bullet_BigSize.LoadImag("img//DanbanLeft.png", screen);
						Bullet_BigSize.SetRect(this->rect.x - Bullet_BigSize.GetRect().w, this->rect.y - 50);
					}
					else {
						//Bullet_BigSize.LoadImag("img//DanbanRight.png", screen);
						Bullet_BigSize.SetRect(this->rect.x + width_frame_, this->rect.y - 50);
					}
				}
			break;
		case SDLK_u:
			if (ki_main >= 250&& input_type.bullet_Skill_U == 0) {
				if (Move_U == false) {
					input_type.bullet_Skill_U = 1; Move_U = true;
				}
				//Bullet_Skill_U[0].LoadImag("img//Sunglon.png", screen);
				Bullet_Skill_U[0].SetRect(this->rect.x - width_frame_, this->rect.y - Bullet_Skill_U[0].GetRect().h - 20);
			}
				break;
		case SDLK_s:
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
		case SDLK_l:
			if(on_ground){
				input_type.speed_up=1;
				break;
			}
			break;
		case SDLK_j:
			if(input_type.defend==0){
				ki_main += 50;
			BulletObject *p_bullet = new BulletObject;
			p_bullet->LoadImag("img//bullet_man.png",screen);

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
			break;
			}
		/*default:
			break;*/
		}
	}
	else if(events.type==SDL_KEYUP){
		switch (events.key.keysym.sym)
		{
		case SDLK_d:
			input_type.right=0;
			break;
		case SDLK_a:
			input_type.left=0;
			break;
		case SDLK_w:
			input_type.jump=0;
			break;
		case SDLK_i:
			input_type.bullet_Skill_I=0;
			break;
		case SDLK_s:
			input_type.defend=0;
			break;
		case SDLK_l:
			input_type.speed_up=0;
			break;
		case SDLK_u:
			input_type.bullet_Skill_U=0;
			break;
		default:
			break;
		}
	}
}

void MainObject:: Doplayer(Map &map_data){
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
	else if(input_type.speed_up==1){
			if(status==WALK_LEFT) x_val-=PLAYER_SPEED_UP;
			else x_val+=PLAYER_SPEED_UP;
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

void MainObject:: Check_map(Map &map_data){
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
		else if(y_val<0){
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

void MainObject:: HandleBuller(SDL_Renderer * des){
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
void MainObject::Show_Bullet_Size(SDL_Renderer * screen){
	Bullet_BigSize.Render(screen);
}
void MainObject::Show_Defend(SDL_Renderer* screen){
	p_object_defend.Render(screen);
}
void MainObject::Show_Bullet_Skill_U(SDL_Renderer* screen){
	if(input_type.bullet_Skill_U==1) Bullet_Skill_U[0].Render(screen);
	Bullet_Skill_U[1].Render(screen);
}
 void MainObject:: Buller_move_U(Map map_data, int vitri){
	 Bullet_Skill_U[1].SetRect(vitri,Bullet_Skill_U[1].GetRect().y+15);
	 int x=Bullet_Skill_U[1].GetRect().x/TILE_SIZE;
	 int y=(Bullet_Skill_U[1].GetRect().y+211)/TILE_SIZE;
	 if(map_data.tile[y][x]!=BLANK_TILE||Bullet_Skill_U[1].GetRect().y+Bullet_Skill_U[1].GetRect().h>SCREEN_HEIGHT){
		 Move_U=false;
		 Bullet_Skill_U[1].SetRect(-200,-200);
	 }
}
void MainObject::Remove_Bullet(const int &idx){
	int size_amo=p_bullet_list.size();
	if(size_amo>0&&size_amo>idx){
		BulletObject *p_amo=p_bullet_list.at(idx);
		p_bullet_list.erase(p_bullet_list.begin()+idx);
		if(p_amo!=NULL){
			delete p_amo;p_amo=NULL;
		}
	}
}