#include<SDL.h>
#include<SDL_image.h>
#include"Common_Function.h"
#include"BaseObject.h"
#include"game_map.h"
#include"MainObject.h"
#include"Main_P2_Object.h"
#include"ImpTimer.h"
#include"BulletObject.h"
#include"Geometric.h"
#include"MENU.h"
BaseObject g_background;
BaseObject menu_BK;
bool InitData(){
	bool success= true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if(ret<0) return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
	g_window=SDL_CreateWindow("GAME SDL 2.0",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	if(g_window==NULL){
		success=false;
		}
	else{
		g_screen= SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
		if(g_screen==NULL){
			success=false;
		}
		else{
			SDL_SetRenderDrawColor(g_screen,255,255,255,255);
			int imgFlags= IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags)&&imgFlags))
				success=false;
		}
		}
	if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1) success=false;
	else{
		g_sound_main_P1[0]=Mix_LoadWAV("music//Sung(J).wav");
		g_sound_main_P1[1]=Mix_LoadWAV("music//Sung(U).wav");
		g_sound_main_P1[2]=Mix_LoadWAV("music//Sung(I).wav");
		
		g_sound_main_P2[0]=Mix_LoadWAV("music//Kiem(J).wav");
		g_sound_main_P2[1]=Mix_LoadWAV("music//Kiem(U).wav");
		g_sound_main_P2[2]=Mix_LoadWAV("music//Kiem(I).wav");
		g_nhacnen[0]=Mix_LoadWAV("music//Tiengtho.wav");
		if(g_nhacnen[0]==NULL) success = false;
		for(int i=0;i<3;i++){
			if(g_sound_main_P1[i]==NULL||g_sound_main_P2[i]==NULL) success=false;
		}
	}
	if (TTF_Init() == -1) success = false;
	g_font_text = TTF_OpenFont("fornchu.ttf", 30);
	g_font_text_1 = TTF_OpenFont("fornchu.ttf", 60);
	if (g_font_text == NULL|| g_font_text_1==NULL) success = false;
	return success;
}
	
int chosen_map() {
	bool ret = menu_BK.LoadImag("img//Menu_bk.png", g_screen);
	if (ret == false) return 0;
	int x_m; int y_m;
	SDL_Event m_even;
	SDL_Rect RECT[4] = { {168,53,320,160},{691,62,320,160},{162,316,320,180},{695,316,320,180} };
	while (true) {
		menu_BK.Render(g_screen);
		while (SDL_PollEvent(&m_even)) {
			switch (m_even.type) {
			case SDL_QUIT:
				return 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				x_m = m_even.motion.x;
				y_m = m_even.motion.y;
				for (int i = 0; i < 4; i++) {
					if (SDLCommonFunc::Checkvitri(x_m, y_m, RECT[i])) {
						return i + 1;
					}
				}
				break;
			default:
				break;
			}
			SDL_RenderPresent(g_screen);
		}
	}
}
bool LoadBackground(int i){
	char c[30];
	sprintf_s(c, "img/background%d.png", i);
	bool ret = g_background.LoadImag(c,g_screen);
	if(ret==false) return false;
	return true;
}
void close(){
	TTF_CloseFont(g_font_text);
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen==NULL;
	SDL_DestroyWindow(g_window);
	g_window=NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
int main(int argc,char *argv[]){
	if(InitData()==false) return -1;
	BaseObject menu_screen;
	menu_screen.LoadImag("img//Menu.png", g_screen);
	MenuObject p_menu;
	std::string p_chu[] = { "Play Game","Exit","Help" };
	int x_menu[] = { 550,510,470 };
	int y_menu[] = { 200,300,400 };
	int kt = -1;
	do {
		menu_screen.Render(g_screen);
		kt = p_menu.ShowMenu(g_screen, g_font_text_1, p_chu, 3, x_menu, y_menu);
	} while (kt == -1);
	if (kt == 1) {
		close();
		return 0;
	}
	menu_screen.Free();
	kt = chosen_map();
	
	ImpTimer fps_time;
	if(LoadBackground(kt)==false) return -1;
	GameMap game_map;
	char c[30];
	sprintf_s(c, "map/map%d.dat", kt);
	game_map.LoadMap(c);
	//game_map.LoadMap("map/map1.dat");
	game_map.LoadTiles(g_screen);
	// Khoi tao nhan vat 1
	MainObject p_player;
	p_player.LoadImag("img//Suppermenright.png",g_screen);
	p_player.Set_clip();
	
	//Khoi tao nhan vat 2
	Main_P2_Object P2_Player;
	P2_Player.LoadImag("img//SieunhanLeft.png",g_screen);
	P2_Player.Set_clip();
	int ret_P1_x=0;
	
	bool is_quit=false;
	while (!is_quit) {
		fps_time.start();
	while(SDL_PollEvent(& g_event)!=0){
		if(g_event.type==SDL_QUIT){
			is_quit=true;break;
			
		}
		p_player.HandeInputAction(g_event,g_screen,g_sound_main_P1[0]);
		P2_Player.HandeInputAction(g_event,g_screen,g_sound_main_P2[0]);
	}
	SDL_SetRenderDrawColor(g_screen,255,255,255,255);
	SDL_RenderClear(g_screen);
	g_background.Render(g_screen,NULL);

	Map map_data = game_map.getMap();
	//--------------------------------------------------
	// nhan vat 1
	p_player.SetMapXY(map_data.start_x,map_data.start_y);
	p_player.Doplayer(map_data);
	p_player.HandleBuller(g_screen);
	if(p_player.Get_Input_type().defend==1){
		p_player.Show_Defend(g_screen);
	}
	else{
		p_player.Show(g_screen);
	if(!p_player.Get_Move_u()){
		ret_P1_x=P2_Player.GetRect().x-20;
	}
	if(p_player.Get_Input_type().bullet_Skill_I==1){
		//Am thanh Sung I
		p_player.Set_ki_main(p_player.Get_ki_main() - 50);
		Mix_PlayChannel(-1,g_sound_main_P1[2],0);
		p_player.Show_Bullet_Size(g_screen);
		}
	}
	if(p_player.Get_Move_u()){
		//Am thanh Sung U
		if (p_player.Get_Input_type().bullet_Skill_U == 1) {
			p_player.Set_ki_main(p_player.Get_ki_main() - 50);
		}
		Mix_PlayChannel(-1,g_sound_main_P1[1],0);
		p_player.Show_Bullet_Skill_U(g_screen);
		p_player.Buller_move_U(map_data,ret_P1_x);
		}


	//--------------------------------------------------
	// nhan vat 2
	P2_Player.SetMapXY(map_data.start_x,map_data.start_y);
	P2_Player.Doplayer(map_data);
	P2_Player.HandleBuller(g_screen);
	if(P2_Player.Get_Input_type().defend==1){
	}
	else{
		P2_Player.Show(g_screen);

		if(P2_Player.Get_Input_type().bullet_Skill_U==1){
			// Am thanh  U
			P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 10);
			Mix_PlayChannel(-1,g_sound_main_P2[1],0);
			P2_Player.SetRect(p_player.GetRect().x,p_player.GetRect().y-5);
		}
		else if(P2_Player.Get_Input_type().bullet_Skill_I==1){
			// Am thanh I
			P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 20);
			Mix_PlayChannel(-1,g_sound_main_P2[2],0);
		}

	}

		//--------------------------------------------------
	game_map.SetMap(map_data);
	game_map.DrawMap(g_screen);
	//--------------------------------------------------------------SU LY VA CHAM-------------------------------------------
	SDL_Rect P1_Rect = p_player.GetRect();
				P1_Rect.w/=8;
	SDL_Rect P2_Rect = P2_Player.GetRect();
				P2_Rect.w/=8;
	// su ly va cham giua DAN Main 1 voi Main 2 
	std::vector<BulletObject*> bullet_list=p_player.get_bullet_list();
		for(int im=0;im<bullet_list.size();im++){
			BulletObject*p_amo=bullet_list.at(im);
			if(p_amo!=NULL){
				bool ret_col=SDLCommonFunc::CheckCollision(p_amo->GetRect(),P2_Rect);
				if(P2_Player.Get_Input_type().defend==1|| P2_Player.Get_Input_type().bullet_Skill_I==1) ret_col= false;
				if(ret_col){
						P2_Player.Set_blood_main(P2_Player.Get_blood_main()-5);
						Mix_PlayChannel(-1,g_nhacnen[0],0);
					P2_Player.Setinput_hurt(1);
				 SDL_RenderPresent(g_screen) ;
					p_player.Remove_Bullet(im);
			}
		}
	}

	// su ly va cham giua Dan Main 2 va Main 1
		std::vector<BulletObject*> bullet_list2=P2_Player.get_bullet_list();
		for(int im=0;im<bullet_list2.size();im++){
			BulletObject*p_amo=bullet_list2.at(im);
			if(p_amo!=NULL){
				bool ret_col=SDLCommonFunc::CheckCollision(p_amo->GetRect(),P1_Rect);
				if(p_player.Get_Input_type().defend==1&&ret_col){ 
					ret_col= false;P2_Player.Remove_Bullet(im);
				}
				if(ret_col){
					p_player.Set_blood_main(p_player.Get_blood_main()-5);
					Mix_PlayChannel(-1,g_nhacnen[0],0);
					p_player.Setinput_hurt(1);
				 SDL_RenderPresent(g_screen) ;
					P2_Player.Remove_Bullet(im);
			}
		}
	}
	//Su ly va cham giua Skill U p1 vs Main 2
			bool ret_col;
			ret_col=SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(1).GetRect(),P2_Rect);
			if(P2_Player.Get_Input_type().defend==1) ret_col= false;
			if(ret_col==true){
				P2_Player.Set_blood_main(P2_Player.Get_blood_main()-50);
				Mix_PlayChannel(-1,g_nhacnen[0],0);
				p_player.Set_Move_U(false);
				P2_Player.Setinput_hurt(1);

			}
			ret_col=SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(0).GetRect(),P2_Rect);
			if(p_player.Get_Input_type().bullet_Skill_U==0) ret_col=false;
			if(ret_col==true){
				P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 50);
				Mix_PlayChannel(-1,g_nhacnen[0],0);
				p_player.Set_Move_U(false);
				P2_Player.Setinput_hurt(1);

			}
	// Su ly va cham giua Skill I p1 vs Main P2
			ret_col=SDLCommonFunc::CheckCollision(p_player.Get_Bullet_BigSize().GetRect(),P2_Rect);
			if(P2_Player.Get_Input_type().defend==1) ret_col= false;
			if(p_player.Get_Input_type().bullet_Skill_I==0) ret_col=false;
			if(ret_col==true){
				P2_Player.Set_blood_main(P2_Player.Get_blood_main()-10);
				Mix_PlayChannel(-1,g_nhacnen[0],0);
				P2_Player.Setinput_hurt(1);
			}

	// Su ly va cham giua SKill U p2 vs Main 1
			ret_col=false;
			if(P2_Player.Get_Input_type().bullet_Skill_U==1) ret_col=true;
			if(p_player.Get_Input_type().defend==1) ret_col= false;
			if(ret_col==true){
				p_player.Set_blood_main(p_player.Get_blood_main()-2);
				Mix_PlayChannel(-1,g_nhacnen[0],0);
				p_player.Setinput_hurt(1);
			}

	// Su ly va cham Giua SKill I p2 vs Main 1
			ret_col=SDLCommonFunc::CheckCollision(P2_Rect,P1_Rect);
			if(P2_Player.Get_Input_type().bullet_Skill_I==0) ret_col=false;
			if(p_player.Get_Input_type().defend==1) ret_col= false;
			if(ret_col==true){
				p_player.Set_blood_main(p_player.Get_blood_main()-20);
				Mix_PlayChannel(-1,g_nhacnen[0],0);
				p_player.Setinput_hurt(1);

			}
		//--------------------------------------------------------------------------------------
	// Ve mau // ki cho nhan vat P1
			// ve mau
			GeometricFormat rectangle_size1(50,20,p_player.Get_blood_main(),20);
			ColorData color_data1(37,121,28);
			Geometric::RenderRecttangle(rectangle_size1,color_data1,g_screen);
			
			GeometricFormat outline_size1(50, 20, 501 ,22);
			ColorData color1(37, 121, 28);
			Geometric::RenderOutline(outline_size1, color1,g_screen);
			// ve ki
			if (p_player.Get_ki_main() <= 0) p_player.Set_ki_main(0) ;
			if(p_player.Get_ki_main()>=1500)  p_player.Set_ki_main(1499);
			int ki1 = p_player.Get_ki_main()%500; 
			GeometricFormat rectangle_size_ki1(50, SCREEN_HEIGHT - 50, ki1, 20);
			ColorData color_data_ki1(0, 162, 232);
			Geometric::RenderRecttangle(rectangle_size_ki1, color_data_ki1, g_screen);

			GeometricFormat outline_size_ki1(50, SCREEN_HEIGHT-50, 501, 22);
			ColorData color_ki1(0, 162, 232);
			Geometric::RenderOutline(outline_size_ki1, color_ki1, g_screen);


	//Ve mau // ki cho nhan vat P2
			// ve mau 

			GeometricFormat rectangle_size2(SCREEN_WIDTH/2+100,20, P2_Player.Get_blood_main(),20);
			ColorData color_data2(37, 121, 28);
			Geometric::RenderRecttangle(rectangle_size2,color_data2,g_screen);

			GeometricFormat outline_size2(SCREEN_WIDTH / 2 + 100, 20, 501, 22);
			ColorData color2(37, 121, 28);
			Geometric::RenderOutline(outline_size2, color2, g_screen);
			// ve ki
			if (P2_Player.Get_ki_main() <= 0) P2_Player.Set_ki_main(0);
			if (P2_Player.Get_ki_main() >= 1500)  P2_Player.Set_ki_main(1499);
			int ki2 = P2_Player.Get_ki_main()%500;
			GeometricFormat rectangle_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50,ki2, 20);
			ColorData color_data_ki2(0, 162, 232);
			Geometric::RenderRecttangle(rectangle_size_ki2, color_data_ki2, g_screen);

			GeometricFormat outline_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, 501, 22);
			ColorData color_ki2(0, 162, 232);
			Geometric::RenderOutline(outline_size_ki2, color_ki2, g_screen);
			//------------------------------------------------------------------------------------
			// SU ly thoi gian
			/*std::string str_time = "Time : ";
			Uint32 time_val = SDL_GetTicks() / 1000;
			std::string str_val = std::to_string(time_val);
			str_time += str_val;
			
			SDL_Color textColor = { 0, 0, 0 }; 

			SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text, str_time.c_str(), textColor);
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
			SDL_Rect dstRect = {SCREEN_WIDTH/2-70, 20, textSurface->w, textSurface->h };
			SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);*/
			//-------------------------------------------------------------------------------------


	SDL_RenderPresent(g_screen);

	int real_imp_time = fps_time.get_ticks();
	int time_one_frame = 1000/FRAME_PER_SECOND;
	if(real_imp_time < time_one_frame){
		int delay_time= time_one_frame - real_imp_time;
		if(delay_time>=0) SDL_Delay(delay_time);
	}
	}
	
	close();
	return 0;

}
