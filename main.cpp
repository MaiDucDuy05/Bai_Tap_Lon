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
#include"ThreatObject.h"
BaseObject g_background;
BaseObject menu_BK;
BaseObject menu_screen;
MenuObject p_menu;
ImpTimer fps_time;
GameMap game_map;
Uint32 time_val_start;
Threat_Object p_threat[Num_Threat];
BaseObject p_threatr[Num_Threat], p_threatl[Num_Threat];
BaseObject p_vuno[Num_Threat];
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
	else {
		g_sound_main_P1[0]=Mix_LoadWAV("music//Sung(J).wav");
		g_sound_main_P1[1]=Mix_LoadWAV("music//Sung(U).wav");
		g_sound_main_P1[2]=Mix_LoadWAV("music//Sung(I).wav");
		
		g_sound_main_P2[0]=Mix_LoadWAV("music//Kiem(J).wav");
		g_sound_main_P2[1]=Mix_LoadWAV("music//Kiem(U).wav");
		g_sound_main_P2[2]=Mix_LoadWAV("music//Kiem(I).wav");
		g_nhacnen[0]=Mix_LoadWAV("music//Tiengtho.wav");
		g_nhacnen[1] = Mix_LoadWAV("music//nhacdao.wav");
		if(g_nhacnen[0]==NULL) success = false;
		for(int i=0;i<3;i++){
			if(g_sound_main_P1[i]==NULL||g_sound_main_P2[i]==NULL) success=false;
		}
	}
	if (TTF_Init() == -1) success = false;
	g_font_text = TTF_OpenFont("fornchu.ttf", 30);
	g_font_text_1 = TTF_OpenFont("fornchu.ttf", 60);
	g_font_text_2 = TTF_OpenFont("fornchu.ttf", 120);
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
			menu_BK.Render(g_screen);
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
void Begin_Threat(int i) {
	int x_, y_;
	p_threat[i].Init_Bullet(g_screen);
	do {
		 x_ = rand() % MAX_MAP_X;
		 y_ = rand() % MAX_MAP_Y;
	}
	while (game_map.getMap().tile[y_][x_] != BLANK_TILE);
	p_threat[i].SetRect(x_ * TILE_SIZE, y_ * TILE_SIZE);
	p_threat[i].Set_blood(100);
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
int Play(int luachon) {
	bool ret_b;
	BaseObject sieunhanr, sieunhanl, suppermenr, suppermenl;
	ret_b= sieunhanr.LoadImag("img//Sieunhan(r).png",g_screen);
	if (ret_b == false) return 1;
	ret_b=sieunhanl.LoadImag("img//Sieunhan(l).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenr.LoadImag("img//Suppermen(r).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenl.LoadImag("img//Suppermen(l).png", g_screen);
	if (ret_b == false) return 1;

	
	//-----------------------------------------------------------------
		
	int kt;// chon map---------------------------------
	kt = chosen_map();
	//---------------------------------------------
						// Load Map----------------------------------------------
	if (LoadBackground(kt) == false) return -1;
	char c[30];
	sprintf_s(c, "map/map%d.dat", kt);
	game_map.LoadMap(c);
				
	game_map.LoadTiles(g_screen);
	//-----------------------------------------------------------------------------------
						// Khoi tao nhan vat 1
	MainObject p_player;
	p_player.LoadImag("img//Suppermenright.png", g_screen);
	p_player.Set_clip();
	p_player.ktImage(g_screen);

							//Khoi tao nhan vat 2
	Main_P2_Object P2_Player;
	P2_Player.LoadImag("img//SieunhanLeft.png", g_screen);
	P2_Player.Set_clip();
	int ret_P1_x = 0;
	P2_Player.ktImage(g_screen);
								// vong lap while---------------------------------------------------------------
	time_val_start = SDL_GetTicks() / 1000;
	bool is_quit = false;
	while (!is_quit) {  
		fps_time.start();
		//-----------------------------------------------------------------------------------
		while ( SDL_PollEvent(&g_event)!=0 ) {
			if (g_event.type == SDL_QUIT) {
				is_quit = true; break;
			}
			p_player.HandeInputAction(g_event, g_screen, g_sound_main_P1[0]);
			if(luachon==0) P2_Player.HandeInputAction(g_event, g_screen, g_sound_main_P2[0]);
		}

		if(luachon==2)P2_Player.Auto_(p_player.GetRect(),p_player.Get_Input_type(), g_screen, g_sound_main_P2[0]);


		SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
		SDL_RenderClear(g_screen);
		g_background.Render(g_screen, NULL);
		// ve nhan vat-----------------------------------------------------
		if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 0) {
			suppermenl.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			suppermenl.Render(g_screen);
		}
		else if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 1) {
			suppermenr.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			suppermenr.Render(g_screen);
		}
		if (P2_Player.Get_Input_type().empty == 1 && P2_Player.get_status() == 0) {
			sieunhanl.SetRect(P2_Player.GetRect().x, P2_Player.GetRect().y);
			if (P2_Player.Get_Input_type().defend == 0)
			sieunhanl.Render(g_screen);
		}
		else if (P2_Player.Get_Input_type().empty == 1 && P2_Player.get_status() == 1) {
			sieunhanr.SetRect(P2_Player.GetRect().x, P2_Player.GetRect().y);
			if (P2_Player.Get_Input_type().defend == 0)
			sieunhanr.Render(g_screen);
		}
		
		Map map_data = game_map.getMap();
		//--------------------------------------------------
		// nhan vat 1
		p_player.Doplayer(map_data);
		p_player.HandleBuller(g_screen);
		if (p_player.Get_Input_type().defend == 1) {
			p_player.Show_Defend(g_screen);
			p_player.Set_ki_main(p_player.Get_ki_main() - 5);
		}
		p_player.Show(g_screen);
		if (!p_player.Get_Move_u()) {
				ret_P1_x = P2_Player.GetRect().x - 20;
			}
			if (p_player.Get_Input_type().bullet_Skill_I == 1) {
				//Am thanh Sung I
				p_player.Set_ki_main(p_player.Get_ki_main() - 50);
				Mix_PlayChannel(-1, g_sound_main_P1[2], 0);
				p_player.Show_Bullet_Size(g_screen);
			}
		if (p_player.Get_Move_u()) {
			//Am thanh Sung U
			if (p_player.Get_Input_type().bullet_Skill_U == 1) {
				p_player.Set_ki_main(p_player.Get_ki_main() - 50);
			}
			Mix_PlayChannel(-1, g_sound_main_P1[1], 0);
			p_player.Show_Bullet_Skill_U(g_screen);
			p_player.Buller_move_U(map_data, ret_P1_x);
		}
		
		//--------------------------------------------------
		// nhan vat 2
		P2_Player.Doplayer(map_data);
		P2_Player.HandleBuller(g_screen);
		if (P2_Player.Get_Input_type().defend == 1) {
			P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 5);
		}
		else {
			P2_Player.Show(g_screen);

			if (P2_Player.Get_Input_type().bullet_Skill_U == 1) {
				// Am thanh  U
				P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 10);
				Mix_PlayChannel(-1, g_sound_main_P2[1], 0);
				P2_Player.SetRect(p_player.GetRect().x, p_player.GetRect().y - 50);
			}
			else if (P2_Player.Get_Input_type().bullet_Skill_I == 1) {
				// Am thanh I
				P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 20);
				Mix_PlayChannel(-1, g_sound_main_P2[2], 0);
			}

		}

		//--------------------------------------------------
		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);
		
		//--------------------------------------------------------------SU LY VA CHAM-------------------------------------------
		SDL_Rect P1_Rect = p_player.GetRect();
		P1_Rect.w /= 8;
		SDL_Rect P2_Rect = P2_Player.GetRect();
		P2_Rect.w /= 8;
		// su ly va cham giua DAN Main 1 voi Main 2 
		std::vector<BulletObject*> bullet_list = p_player.get_bullet_list();
		for (int im = 0; im < bullet_list.size(); im++) {
			BulletObject* p_amo = bullet_list.at(im);
			if (p_amo != NULL) {
				bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), P2_Rect);
				if (P2_Player.Get_Input_type().defend == 1 || P2_Player.Get_Input_type().bullet_Skill_I == 1) ret_col = false;
				if (ret_col) {
					if (p_amo->GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
					else P2_Player.set_status(1);
					P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 5);
					Mix_PlayChannel(-1, g_nhacnen[0], 0);
					P2_Player.Setinput_hurt(1);
					SDL_RenderPresent(g_screen);
					p_player.Remove_Bullet(im);
				}
			}
		}

		// su ly va cham giua Dan Main 2 va Main 1
		std::vector<BulletObject*> bullet_list2 = P2_Player.get_bullet_list();
		for (int im = 0; im < bullet_list2.size(); im++) {
			BulletObject* p_amo = bullet_list2.at(im);
			if (p_amo != NULL) {
				bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), P1_Rect);
				if (p_player.Get_Input_type().defend == 1 && ret_col) {
					ret_col = false; P2_Player.Remove_Bullet(im);
				}
				if (ret_col) {
					if (p_amo->GetRect().x > p_player.GetRect().x) p_player.set_status(0);
					else p_player.set_status(1);
					p_player.Set_blood_main(p_player.Get_blood_main() - 5);
					Mix_PlayChannel(-1, g_nhacnen[0], 0);
					p_player.Setinput_hurt(1);
					SDL_RenderPresent(g_screen);
					P2_Player.Remove_Bullet(im);
				}
			}
		}
		//Su ly va cham giua Skill U p1 vs Main 2
		bool ret_col;
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(1).GetRect(), P2_Rect);
		if (P2_Player.Get_Input_type().defend == 1||p_player.Get_Move_u() == false) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);

			P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 20);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Set_Move_U(false);
			P2_Player.Setinput_hurt(1);

		}
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(0).GetRect(), P2_Rect);
		if (p_player.Get_Input_type().bullet_Skill_U == 0) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);
			P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 20);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Set_Move_U(false);
			P2_Player.Setinput_hurt(1);

		}
		// Su ly va cham giua Skill I p1 vs Main P2
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_BigSize().GetRect(), P2_Rect);
		if (P2_Player.Get_Input_type().defend == 1) ret_col = false;
		if (p_player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
		if (ret_col == true){
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);
			P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 10);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			P2_Player.Setinput_hurt(1);
		}

		// Su ly va cham giua SKill U p2 vs Main 1
		ret_col = false;
		if (P2_Player.Get_Input_type().bullet_Skill_U == 1) ret_col = true;
		if (p_player.Get_Input_type().defend == 1) ret_col = false;
		if (ret_col == true) {
			if (P2_Player.get_status() == 1) p_player.set_status(0);
			else p_player.set_status(1);
			p_player.Set_blood_main(p_player.Get_blood_main() - 2);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Setinput_hurt(1);
		}

		// Su ly va cham Giua SKill I p2 vs Main 1
		ret_col = SDLCommonFunc::CheckCollision(P2_Rect, P1_Rect);
		if (P2_Player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
		if (p_player.Get_Input_type().defend == 1) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x < P2_Player.GetRect().x) p_player.set_status(0);
			else p_player.set_status(1);
			p_player.Set_blood_main(p_player.Get_blood_main() - 3);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Setinput_hurt(1);

		}
		
		//--------------------------------------------------------------------------------------
		

			// Su ly game over----------------------------------------------

		if (p_player.Get_blood_main() <= 0) {
			GeometricFormat rectangle(SCREEN_WIDTH / 2 - 180, 100, 500, 150);
			ColorData color(40, 0, 0);
			Geometric::RenderRecttangle(rectangle, color, g_screen);

			SDL_Color textColor = { 255, 128, 0 };
			SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text_2, "P2 WIN", textColor);
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
			SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 150, 100, textSurface->w, textSurface->h };
			SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);
			SDL_RenderPresent(g_screen);
			SDL_Delay(3000);
			return 0;
		}
		else if (P2_Player.Get_blood_main() <= 0) {
			GeometricFormat rectangle(SCREEN_WIDTH / 2 - 180, 100, 500, 150);
			ColorData color(40, 0, 0);
			Geometric::RenderRecttangle(rectangle, color, g_screen);

			SDL_Color textColor = { 255, 128, 0 };
			SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text_2, "P1 WIN", textColor);
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
			SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 150, 100, textSurface->w, textSurface->h };
			SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);
			SDL_RenderPresent(g_screen);
			SDL_Delay(3000);
			return 0;
		}
		//------------------------------------------------------------------
// Ve mau // ki cho nhan vat P1
		// ve mau
		
		GeometricFormat rectangle_size1(50, 20, p_player.Get_blood_main()/3, 20);
		ColorData color_data1(37, 121, 28);
		Geometric::RenderRecttangle(rectangle_size1, color_data1, g_screen);

		GeometricFormat outline_size1(50, 20, 501, 22);
		ColorData color1(37, 121, 28);
		Geometric::RenderOutline(outline_size1, color1, g_screen);
		// ve ki
		if (p_player.Get_ki_main() <= 0) p_player.Set_ki_main(0);
		if (p_player.Get_ki_main() >= 1500)  p_player.Set_ki_main(1499);
		int ki1 = p_player.Get_ki_main() % 500;
		GeometricFormat rectangle_size_ki1(50, SCREEN_HEIGHT - 50, ki1, 20);
		ColorData color_data_ki1(0, 162, 232);
		Geometric::RenderRecttangle(rectangle_size_ki1, color_data_ki1, g_screen);

		GeometricFormat outline_size_ki1(50, SCREEN_HEIGHT - 50, 501, 22);
		ColorData color_ki1(0, 162, 232);
		Geometric::RenderOutline(outline_size_ki1, color_ki1, g_screen);


		//Ve mau // ki cho nhan vat P2
				// ve mau 
		
		GeometricFormat rectangle_size2(SCREEN_WIDTH / 2 + 100, 20, P2_Player.Get_blood_main()/3, 20);
		ColorData color_data2(37, 121, 28);
		Geometric::RenderRecttangle(rectangle_size2, color_data2, g_screen);

		GeometricFormat outline_size2(SCREEN_WIDTH / 2 + 100, 20, 501, 22);
		ColorData color2(37, 121, 28);
		Geometric::RenderOutline(outline_size2, color2, g_screen);
		// ve ki
		if (P2_Player.Get_ki_main() <= 0) P2_Player.Set_ki_main(0);
		if (P2_Player.Get_ki_main() >= 1500)  P2_Player.Set_ki_main(1499);
		int ki2 = P2_Player.Get_ki_main() % 500;
		GeometricFormat rectangle_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, ki2, 20);
		ColorData color_data_ki2(0, 162, 232);
		Geometric::RenderRecttangle(rectangle_size_ki2, color_data_ki2, g_screen);

		GeometricFormat outline_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, 501, 22);
		ColorData color_ki2(0, 162, 232);
		Geometric::RenderOutline(outline_size_ki2, color_ki2, g_screen);
		//------------------------------------------------------------------------------------
	
		//SU ly thoi gian
		std::string str_time = "Time : ";
		Uint32 time_val = (SDL_GetTicks() / 1000) - time_val_start;
		std::string str_val = std::to_string(time_val);
		str_time += str_val;

		SDL_Color textColor = { 217, 65, 38 };

		SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text, str_time.c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
		SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 70, 20, textSurface->w, textSurface->h };
		SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);
		
		//-------------------------------------------------------------------------------------
		
		SDL_RenderPresent(g_screen);
		SDL_RenderClear(g_screen);
		// huy cac con tro tranh tran bo nho --------------------------
		p_player.Free();
		P2_Player.Free();
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

		int real_imp_time = fps_time.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;
		if (real_imp_time < time_one_frame) {
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}
	
	}

	return 1;

}
int AutoPlay() {
	 int Mark_P1 = 0;
	 int Mark_P2 = 0;
	bool ret_b;
	BaseObject sieunhanr, sieunhanl, suppermenr, suppermenl;
	ret_b = sieunhanr.LoadImag("img//Sieunhan(r).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = sieunhanl.LoadImag("img//Sieunhan(l).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenr.LoadImag("img//Suppermen(r).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenl.LoadImag("img//Suppermen(l).png", g_screen);
	if (ret_b == false) return 1;

	int kt;
	//-----------------------------------------------------------------
						// chon map---------------------------------
	kt = chosen_map();
	//---------------------------------------------
						// Load Map----------------------------------------------
	if (LoadBackground(kt) == false) return -1;
	char c[30];
	sprintf_s(c, "map/map%d.dat", kt);
	game_map.LoadMap(c);
	//game_map.LoadMap("map/map1.dat");
	game_map.LoadTiles(g_screen);
	//-----------------------------------------------------------------------------------
						// Khoi tao nhan vat 1
	MainObject p_player;
	p_player.LoadImag("img//Suppermenright.png", g_screen);
	p_player.Set_clip();
	p_player.ktImage(g_screen);

	                   //Khoi tao nhan vat 2
	Main_P2_Object P2_Player;
	P2_Player.LoadImag("img//SieunhanLeft.png", g_screen);
	P2_Player.Set_clip();
	int ret_P1_x = 0;
	P2_Player.ktImage(g_screen);
	                    // khot tao threat
	for (int i = 0; i < Num_Threat; i++) {
		Begin_Threat(i);
		p_threatr[i].LoadImag("img//Threat(r).png", g_screen);
		p_threatl[i].LoadImag("img//Threat(l).png", g_screen);
		p_vuno[i].LoadImag("img//vuno.png", g_screen);
	}

	// vong lap while
	time_val_start = SDL_GetTicks() / 1000;
	bool is_quit = false;
	while (!is_quit) {
		fps_time.start();
		//-----------------------------------------------------------------------------------
		while (SDL_PollEvent(&g_event) != 0) {
			if (g_event.type == SDL_QUIT) {
				is_quit = true; break;
			}
			p_player.HandeInputAction(g_event, g_screen, g_sound_main_P1[0]);
			P2_Player.HandeInputAction(g_event, g_screen, g_sound_main_P2[0]);
		}

		SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
		SDL_RenderClear(g_screen);
		g_background.Render(g_screen, NULL);
		// ve nhan vat-----------------------------------------------------
		if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 0) {
			suppermenl.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			suppermenl.Render(g_screen);
		}
		else if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 1) {
			suppermenr.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			suppermenr.Render(g_screen);
		}
		if (P2_Player.Get_Input_type().empty == 1 && P2_Player.get_status() == 0) {
			sieunhanl.SetRect(P2_Player.GetRect().x, P2_Player.GetRect().y);
			if (P2_Player.Get_Input_type().defend == 0)
				sieunhanl.Render(g_screen);
		}
		else if (P2_Player.Get_Input_type().empty == 1 && P2_Player.get_status() == 1) {
			sieunhanr.SetRect(P2_Player.GetRect().x, P2_Player.GetRect().y);
			if (P2_Player.Get_Input_type().defend == 0)
				sieunhanr.Render(g_screen);
		}

		Map map_data = game_map.getMap();
		//--------------------------------------------------
		// 
		
		// nhan vat 1
		p_player.Doplayer(map_data);
		p_player.HandleBuller(g_screen);
		if (p_player.Get_Input_type().defend == 1) {
			p_player.Show_Defend(g_screen);
			p_player.Set_ki_main(p_player.Get_ki_main() - 5);
		}
		else {
			p_player.Show(g_screen);
			if (!p_player.Get_Move_u()) {
				ret_P1_x = P2_Player.GetRect().x - 20;
			}
			if (p_player.Get_Input_type().bullet_Skill_I == 1) {
				//Am thanh Sung I
				p_player.Set_ki_main(p_player.Get_ki_main() - 50);
				Mix_PlayChannel(-1, g_sound_main_P1[2], 0);
				p_player.Show_Bullet_Size(g_screen);
			}
		}
		
			//Am thanh Sung U
			if (p_player.Get_Input_type().bullet_Skill_U == 1) {
				p_player.Set_ki_main(p_player.Get_ki_main() - 50);
			Mix_PlayChannel(-1, g_sound_main_P1[1], 0);
			p_player.Show_Bullet_Skill_U(g_screen);
			p_player.Set_Move_U(false);
			}


		//--------------------------------------------------
		// nhan vat 2
		P2_Player.Doplayer(map_data);
		P2_Player.HandleBuller(g_screen);
		if (P2_Player.Get_Input_type().defend == 1) {
			P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 5);
		}
		else {
			P2_Player.Show(g_screen);
			 if (P2_Player.Get_Input_type().bullet_Skill_I == 1) {
				// Am thanh I
				P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 20);
				Mix_PlayChannel(-1, g_sound_main_P2[2], 0);
			}

		}

		//--------------------------------------------------
		//-------------------Su ly Threat-------------------------------
		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);
		for (int i = 0; i < Num_Threat; i++) {
			if (i < Num_Threat / 2) {
				p_threat[i].Set_goal_x(p_player.GetRect().x / TILE_SIZE);
				p_threat[i].Set_goal_y((p_player.GetRect().y) / TILE_SIZE);
			}
			else {
				p_threat[i].Set_goal_x(P2_Player.GetRect().x / TILE_SIZE);
				p_threat[i].Set_goal_y((P2_Player.GetRect().y) / TILE_SIZE+1);
			}
			p_threat[i].Search(p_threat[i].GetRect().x / TILE_SIZE, p_threat[i].GetRect().y / TILE_SIZE, game_map.getMap());
			p_threat[i].HandleBuller(g_screen);
			if (p_threat[i].get_status() == 0) {
				p_threatl[i].SetRect(p_threat[i].GetRect().x, p_threat[i].GetRect().y);
				p_threatl[i].Render(g_screen);
			}
			else {
				p_threatr[i].SetRect(p_threat[i].GetRect().x, p_threat[i].GetRect().y);
				p_threatr[i].Render(g_screen);
			}
			p_threat[i].Draw_blood(g_screen);
		}
		
		//--------------------------------------------------------------SU LY VA CHAM-------------------------------------------
			//Su lu va cham giua dan cua Main 2 va Threat
		std::vector<BulletObject*> bullet_list2 = P2_Player.get_bullet_list();
		for (int im = 0; im < bullet_list2.size(); im++) {
			BulletObject* p_amo = bullet_list2.at(im);
			if (p_amo != NULL) {
				for (int k = 0; k < Num_Threat; k++) {
					bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_threat[k].GetRect());
					if (ret_col) {
						p_threat[k].Set_blood(p_threat[k].Get_blood() - 10);
						SDL_RenderPresent(g_screen);
						P2_Player.Remove_Bullet(im);
						if (p_threat[k].Get_blood() <= 0) {
							for (int j = 0; j < p_threat[k].get_bullet_list().size(); j++) {
								p_threat[k].Remove_Bullet(j);
							}
							p_vuno[k].SetRect(p_threat[k].GetRect().x, p_threat[k].GetRect().y);
							p_vuno[k].Render(g_screen);
							Mark_P2++;
							SDL_RenderPresent(g_screen);
							SDL_Delay(400);
							Begin_Threat(k);
						}
					}
				}
			}
		}
			// Su ly dan cua Main 1 va threat 
		std::vector<BulletObject*> bullet_list1 = p_player.get_bullet_list();
		for (int im = 0; im < bullet_list1.size(); im++) {
			BulletObject* p_amo = bullet_list1.at(im);
			if (p_amo != NULL) {
				for (int k = 0; k < Num_Threat; k++) {
					bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_threat[k].GetRect());
					if (ret_col) {
						p_threat[k].Set_blood(p_threat[k].Get_blood() - 10);
						SDL_RenderPresent(g_screen);
						p_player.Remove_Bullet(im);
						if (p_threat[k].Get_blood() <= 0) {
							for (int j = 0; j < p_threat[k].get_bullet_list().size(); j++) {
								p_threat[k].Remove_Bullet(j);
							}
							p_vuno[k].SetRect(p_threat[k].GetRect().x, p_threat[k].GetRect().y);
							p_vuno[k].Render(g_screen);
							Mark_P1++;
							SDL_RenderPresent(g_screen);
							SDL_Delay(400);
							Begin_Threat(k);
						}
					}
				}
			}
		}
		
			// Su ly va cham giua dan cua Threat va Main 2 va Main 1
		SDL_Rect p1_rect = p_player.GetRect();
		p1_rect.w /= 8;
		SDL_Rect p2_rect = P2_Player.GetRect();
		p2_rect.w /= 8;
		
		for (int im = 0; im < Num_Threat; im++) {
			std::vector<BulletObject*> bullet_list = p_threat[im].get_bullet_list();
			BulletObject* p_amo;
			for (int k = 0; k < bullet_list.size(); k++) {
				p_amo = bullet_list.at(k);
			//---------------------------------------------------
				
				bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p2_rect);
				if (P2_Player.Get_Input_type().defend == 1 || P2_Player.Get_Input_type().bullet_Skill_I == 1) ret_col = false;
				if (ret_col) {
					if (p_amo->GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
					else P2_Player.set_status(1);
					P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 5);
					Mix_PlayChannel(-1, g_nhacnen[0], 0);
					P2_Player.Setinput_hurt(1);
					SDL_RenderPresent(g_screen);
					p_threat[im].Remove_Bullet(k);
					p_threat[im].Init_Bullet(g_screen);
				}
				
			//----------------------------------------------------	
				ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p1_rect);
				if (p_player.Get_Input_type().defend == 1 && ret_col) {
					ret_col = false;
					p_threat[im].Remove_Bullet(k);
					p_threat[im].Init_Bullet(g_screen);
				}
				if (ret_col) {
					if (p_amo->GetRect().x > p_player.GetRect().x) p_player.set_status(0);
					else p_player.set_status(1);
					p_player.Set_blood_main(p_player.Get_blood_main() - 5);
					Mix_PlayChannel(-1, g_nhacnen[0], 0);
					p_player.Setinput_hurt(1);
					SDL_RenderPresent(g_screen);
					p_threat[im].Remove_Bullet(k);
					p_threat[im].Init_Bullet(g_screen);
				}
				
			}
		}

		// Su ly va cham giua Skill I p1 vs Threat
		for (int tt = 0; tt < Num_Threat; tt++) {
			bool ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_BigSize().GetRect(), p_threat[tt].GetRect());
			if (p_player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
			if (ret_col == true) {
				p_threat[tt].Set_blood(p_threat[tt].Get_blood() - 30);
				if (p_threat[tt].Get_blood() <= 0) {
					for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
						p_threat[tt].Remove_Bullet(j);
					}
					p_vuno[tt].SetRect(p_threat[tt].GetRect().x, p_threat[tt].GetRect().y);
					p_vuno[tt].Render(g_screen);
					Mark_P1++;
					SDL_RenderPresent(g_screen);
					SDL_Delay(400);
					Begin_Threat(tt);
				}
			}
		}
		// Su ly va cham giua Skill U p1 vs Threat
		for (int tt = 0; tt < Num_Threat; tt++) {
			bool ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(0).GetRect(), p_threat[tt].GetRect());
			if (p_player.Get_Input_type().bullet_Skill_U == 0) ret_col = false;
			if (ret_col == true) {
				p_threat[tt].Set_blood(p_threat[tt].Get_blood() - 30);
				if (p_threat[tt].Get_blood() <= 0) {
					for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
						p_threat[tt].Remove_Bullet(j);
					}
					p_vuno[tt].SetRect(p_threat[tt].GetRect().x, p_threat[tt].GetRect().y);
					p_vuno[tt].Render(g_screen);
					Mark_P1++;
					SDL_RenderPresent(g_screen);
					SDL_Delay(400);
					Begin_Threat(tt);
				}
			}
		}
		// Su ly va cham Giua SKill I p2 vs Threat
		for (int tt = 0; tt < Num_Threat; tt++) {
			bool ret_col = SDLCommonFunc::CheckCollision(p2_rect,p_threat[tt].GetRect());
			if (P2_Player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
			if (ret_col == true) {
				p_threat[tt].Set_blood(p_threat[tt].Get_blood() - 40);
				if (p_threat[tt].Get_blood() <= 0) {
					for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
						p_threat[tt].Remove_Bullet(j);
					}
					p_vuno[tt].SetRect(p_threat[tt].GetRect().x, p_threat[tt].GetRect().y);
					p_vuno[tt].Render(g_screen);
					Mark_P2++;
					SDL_RenderPresent(g_screen);
					SDL_Delay(400);
					Begin_Threat(tt);
				}

			}
		}
		// Su ly va cham Giua SKill U p2 vs Threat
		for (int tt = 0; tt < Num_Threat; tt++) {
			bool ret_col = false;
			if (P2_Player.Get_Input_type().bullet_Skill_U == 1) {
				ret_col = true;
				Mix_PlayChannel(-1, g_sound_main_P2[1], 0);
				P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 20);
			}
			if(ret_col) ret_col = SDLCommonFunc::CheckCollision(p2_rect, p_threat[tt].GetRect());
			if (ret_col == true) {
				for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
					p_threat[tt].Remove_Bullet(j);
				}
				p_vuno[tt].SetRect(p_threat[tt].GetRect().x, p_threat[tt].GetRect().y);
				p_vuno[tt].Render(g_screen);
				Mark_P2++;
				SDL_RenderPresent(g_screen);
				SDL_Delay(400);
				Begin_Threat(tt);
			}
		}


		//--------------------------------------------------------------------------------------
	

			// Su ly game over----------------------------------------------

		if (p_player.Get_blood_main() <= 0) {
			GeometricFormat rectangle(SCREEN_WIDTH / 2 - 180, 100, 500, 150);
			ColorData color(40, 0, 0);
			Geometric::RenderRecttangle(rectangle, color, g_screen);

			SDL_Color textColor = { 255, 128, 0 };
			SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text_2, "P2 WIN", textColor);
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
			SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 150, 100, textSurface->w, textSurface->h };
			SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);
			SDL_RenderPresent(g_screen);
			SDL_Delay(3000);
			for (int im = 0; im < Num_Threat; im++) {
				std::vector<BulletObject*> bullet_list = p_threat[im].get_bullet_list();
					for (int k = 0; k < bullet_list.size(); k++) {
						p_threat[k].Remove_Bullet(im);
						p_threat[k].Free();
					}
			}
			return 0;
		}
		else if (P2_Player.Get_blood_main() <= 0) {
			GeometricFormat rectangle(SCREEN_WIDTH / 2 - 180, 100, 500, 150);
			ColorData color(40, 0, 0);
			Geometric::RenderRecttangle(rectangle, color, g_screen);

			SDL_Color textColor = { 255, 128, 0 };
			SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text_2, "P1 WIN", textColor);
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
			SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 150, 100, textSurface->w, textSurface->h };
			SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);
			SDL_RenderPresent(g_screen);
			SDL_Delay(3000);
			for (int im = 0; im < Num_Threat; im++) {
				std::vector<BulletObject*> bullet_list = p_threat[im].get_bullet_list();
				for (int k = 0; k < bullet_list.size(); k++) {
					p_threat[k].Remove_Bullet(im);
					p_threat[k].Free();
				}
			}
			return 0;
		}
		//------------------------------------------------------------------
		
		
// Ve mau // ki cho nhan vat P1
		// ve mau

		GeometricFormat rectangle_size1(50, 20, p_player.Get_blood_main(), 20);
		ColorData color_data1(37, 121, 28);
		Geometric::RenderRecttangle(rectangle_size1, color_data1, g_screen);

		GeometricFormat outline_size1(50, 20, 501, 22);
		ColorData color1(37, 121, 28);
		Geometric::RenderOutline(outline_size1, color1, g_screen);
		// ve ki
		if (p_player.Get_ki_main() <= 0) p_player.Set_ki_main(0);
		if (p_player.Get_ki_main() >= 1500)  p_player.Set_ki_main(1499);
		int ki1 = p_player.Get_ki_main() % 500;
		GeometricFormat rectangle_size_ki1(50, SCREEN_HEIGHT - 50, ki1, 20);
		ColorData color_data_ki1(0, 162, 232);
		Geometric::RenderRecttangle(rectangle_size_ki1, color_data_ki1, g_screen);

		GeometricFormat outline_size_ki1(50, SCREEN_HEIGHT - 50, 501, 22);
		ColorData color_ki1(0, 162, 232);
		Geometric::RenderOutline(outline_size_ki1, color_ki1, g_screen);


		//Ve mau // ki cho nhan vat P2
				// ve mau 

		GeometricFormat rectangle_size2(SCREEN_WIDTH / 2 + 100, 20, P2_Player.Get_blood_main(), 20);
		ColorData color_data2(37, 121, 28);
		Geometric::RenderRecttangle(rectangle_size2, color_data2, g_screen);

		GeometricFormat outline_size2(SCREEN_WIDTH / 2 + 100, 20, 501, 22);
		ColorData color2(37, 121, 28);
		Geometric::RenderOutline(outline_size2, color2, g_screen);
		// ve ki
		if (P2_Player.Get_ki_main() <= 0) P2_Player.Set_ki_main(0);
		if (P2_Player.Get_ki_main() >= 1500)  P2_Player.Set_ki_main(1499);
		int ki2 = P2_Player.Get_ki_main() % 500;
		GeometricFormat rectangle_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, ki2, 20);
		ColorData color_data_ki2(0, 162, 232);
		Geometric::RenderRecttangle(rectangle_size_ki2, color_data_ki2, g_screen);

		GeometricFormat outline_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, 501, 22);
		ColorData color_ki2(0, 162, 232);
		Geometric::RenderOutline(outline_size_ki2, color_ki2, g_screen);
		//------------------------------------------------------------------------------------
		//SU ly thoi gian
		std::string str_time = "Time : ";
		Uint32 time_val = (SDL_GetTicks() / 1000) - time_val_start;
		std::string str_val = std::to_string(time_val);
		str_time += str_val;

		SDL_Color textColor = { 217, 65, 38 };

		SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text, str_time.c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
		SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 70, 20, textSurface->w, textSurface->h };
		SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);

		//-------------------------------------------------------------------------------------

		//------------------Su ly diem----------------
		std::string str_mark_p1 = "Mark 1 : ";
		std::string mark1_val = std::to_string(Mark_P1);
		str_mark_p1 += mark1_val;
		SDL_Color textColor1 = { 255,255,255 };
		SDL_Surface* textSurface_mark1 = TTF_RenderText_Solid(g_font_text, str_mark_p1.c_str(), textColor1);
		SDL_Texture* textTexture_mark1 = SDL_CreateTextureFromSurface(g_screen, textSurface_mark1);
		SDL_Rect dstRect1 = { 170, 60, textSurface_mark1->w, textSurface_mark1->h };
		SDL_RenderCopy(g_screen, textTexture_mark1, NULL, &dstRect1);

		std::string str_mark_p2 = "Mark 2 : ";
		std::string mark2_val = std::to_string(Mark_P2);
		str_mark_p2 += mark2_val;
		SDL_Color textColor2 = {255,255, 255 };
		SDL_Surface* textSurface_mark2 = TTF_RenderText_Solid(g_font_text, str_mark_p2.c_str(), textColor2);
		SDL_Texture* textTexture_mark2 = SDL_CreateTextureFromSurface(g_screen, textSurface_mark2);
		SDL_Rect dstRect2 = { SCREEN_WIDTH / 2 + 170, 60, textSurface_mark2->w, textSurface_mark2->h };
		SDL_RenderCopy(g_screen, textTexture_mark2, NULL, &dstRect2);

		//----------------------------------------------------------------------------------------


		SDL_RenderPresent(g_screen);
		SDL_RenderClear(g_screen);


		// huy cac con tro tranh tran bo nho --------------------------
		p_player.Free();
		P2_Player.Free();
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

		int real_imp_time = fps_time.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;
		if (real_imp_time < time_one_frame) {
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}

	}
	return 1;
}
int main(int argc,char *argv[]){
	if (InitData() == false) return -1;
	MenuObject gover_menu;
	BaseObject g_over;
	g_over.LoadImag("img//gameover.png",g_screen);
	int is_continue = 0;
	std::string DS[2]={ "CONTINUE","EXIT" };
	int x_over[] = { SCREEN_WIDTH / 2 - 50,SCREEN_WIDTH / 2 - 50 };
	int y_over[] = { 50,SCREEN_HEIGHT-100 };
	menu_screen.LoadImag("img//Menu.png", g_screen);
	do {
		// Show menu game------------------------------------------
		std::string p_chu[] = { "Play Game","Exit","Help" };
		int x_menu[] = { 550,510,470 };
		int y_menu[] = { 200,300,400 };
		int kt = -1;
		do {
			if (kt == 2) {
				int k = -1;
				BaseObject huong_dan[2];
				huong_dan[0].LoadImag("img/Bk_huongdan.png",g_screen);
				huong_dan[1].LoadImag("img/huongdan.png",g_screen);
				MenuObject p_huongdan;
				std::string p_Chu[] = {"EXIT"};
				int x_m[] = { 1000 };
				int y_m[] = { 30 };
				huong_dan[0].Render(g_screen);
				huong_dan[1].Render(g_screen);
				SDL_RenderPresent(g_screen);
				p_huongdan.ShowMenu(g_screen, g_font_text_1, p_Chu, 1, x_m, y_m, g_nhacnen[1]);
				huong_dan[0].Free();
				huong_dan[1].Free();
			}
			menu_screen.Render(g_screen);
			kt = p_menu.ShowMenu(g_screen, g_font_text_1, p_chu, 3, x_menu, y_menu, g_nhacnen[1]);
		} while (kt == -1||kt==2);
		if (kt == 1) {
			return 0;
		}
		//menu_screen.Free();
		// Show Lua Chon Phan choi ------------------------------------------
		kt = -1;
		std::string p_luachon[] = { "P1 vs P2","Play Monster","AuTo Play"};
		int  x_ch[] = { 550,510,490};
		int y_ch[] = {200,300,400};
		do {
			menu_screen.Render(g_screen);
			kt = p_menu.ShowMenu(g_screen, g_font_text_1, p_luachon, 3, x_ch, y_ch, g_nhacnen[1]);
		} while (kt < 0);
		p_menu.Free();
		if (kt == 0||kt==2) {
			if (Play(kt) == 1) break;
		}
		else if (AutoPlay() == 1) break;
		g_over.Render(g_screen);
		is_continue = gover_menu.ShowMenu(g_screen, g_font_text_1, DS, 2, x_over, y_over,g_nhacnen[1]);
		SDL_RenderPresent(g_screen);
	} while (is_continue==0);
	close();
	return 0;
}
