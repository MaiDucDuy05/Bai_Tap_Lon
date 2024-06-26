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
#include<iostream>
#include<fstream>
BaseObject g_background;
BaseObject menu_BK;
BaseObject menu_screen;
MenuObject p_menu;
ImpTimer fps_time;
GameMap game_map;
std::ifstream in;
std::ofstream out;
Uint32 time_val_start;

Threat_Object p_threat[10];
BaseObject p_vuno[10];
bool InitData() {
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_window = SDL_CreateWindow("GAME SDL 2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (g_window == NULL) {
		success = false;
	}
	else {
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) {
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) success = false;
	else {
		
		g_sound_main_P1[0]=Mix_LoadWAV("music//Sung(J).wav");
		g_sound_main_P1[1]=Mix_LoadWAV("music//Sung(U).wav");
		g_sound_main_P1[2]=Mix_LoadWAV("music//Sung(I).wav");

		g_sound_main_P2[0]=Mix_LoadWAV("music//Kiem(J).wav");
		g_sound_main_P2[1]=Mix_LoadWAV("music//Kiem(U).wav");
		g_sound_main_P2[2]=Mix_LoadWAV("music//Kiem(I).wav");

		g_nhacnen[0]=Mix_LoadWAV("music//Tiengtho.wav");
		g_nhacnen[1] = Mix_LoadWAV("music//nhacdao.wav");
		g_nhacnen[2] = Mix_LoadWAV("music//Readygo.wav");
		g_nhacnen[3] = Mix_LoadWAV("music//Win.mp3");
		g_nhacnen[4] = Mix_LoadWAV("music//KO.wav");
		g_sound_rong[0] = Mix_LoadWAV("music//Tiengrong.mp3");
		g_sound_rong[1] = Mix_LoadWAV("music//Vocanh.mp3");
		for (int i = 0; i < 3; i++) {
			if(g_sound_main_P1[i]==NULL||g_sound_main_P2[i]==NULL||g_nhacnen[i]==NULL) success = false;
		}
		Mix_VolumeChunk(g_sound_main_P2[2], MIX_MAX_VOLUME / 4);
		Mix_VolumeChunk(g_nhacnen[1], MIX_MAX_VOLUME / 2);
		
	}
	if (TTF_Init() == -1) success = false;
	g_font_text = TTF_OpenFont("fornchu.ttf", 30);
	g_font_text_1 = TTF_OpenFont("fornchu.ttf", 60);
	g_font_text_2 = TTF_OpenFont("fornchu.ttf", 120);
	if (g_font_text == NULL || g_font_text_1 == NULL) success = false;
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
				return -1;
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
int chosen_threat(int &NUM_Threat,int HIGH_SCORE[]) {
	MenuObject p_menu_threat;
	bool ret = menu_BK.LoadImag("img//BK_Threat.png", g_screen);
	if (ret == false) return 0;
	std::string Menu_threat[6] = { "+","-","Player_P1","Player_P2","Two Player","High Score"};
	int dx_threat[6] = { 700,750,500,500,500,500 };
	int dy_threat[6] = { 120,120,200,300,400,500 };
	int kt_threat = 0;
	do {
		std::string num_threat = std::to_string(NUM_Threat);
		SDL_Color textColor1 = { 255,255,255 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text_1, num_threat.c_str(), textColor1);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
		SDL_Rect dstRect1 = { 600, 120, textSurface->w, textSurface->h };
		menu_BK.Render(g_screen);
		SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect1);
		kt_threat = p_menu_threat.ShowMenu(g_screen, g_font_text_1, Menu_threat, 6, dx_threat, dy_threat, g_nhacnen[1]);
		if (kt_threat == 0) NUM_Threat++;
		else if (kt_threat == 1) NUM_Threat--;
		if (NUM_Threat <= 1) NUM_Threat = 1;
		else if (NUM_Threat >= 5) NUM_Threat = 5;

		if (kt_threat == 5) {
			int k = 1;MenuObject p_high_score;
			in.open("HighScore.txt"); int n; int i = 0;
			while (in >> n) {
				HIGH_SCORE[i] = n;
				i++;
			}
			in.close();
			do {
				std::string h_score_1 = "Player_1 : " + std::to_string(HIGH_SCORE[0]);
				std::string h_score_2 = "Player_2 : " + std::to_string(HIGH_SCORE[1]);
				std::string h_score_3 = "Two Player: " + std::to_string(HIGH_SCORE[2]);
				SDL_Rect dstRect1, dstRect3, dstRect2;

				SDL_Color textColor1 = { 255,255,255 };
				SDL_Surface* textSur1 = TTF_RenderText_Solid(g_font_text_1, h_score_1.c_str(), textColor1);
				SDL_Surface* textSur2 = TTF_RenderText_Solid(g_font_text_1, h_score_2.c_str(), textColor1);
				SDL_Surface* textSur3 = TTF_RenderText_Solid(g_font_text_1, h_score_3.c_str(), textColor1);

				SDL_Texture* textTex1 = SDL_CreateTextureFromSurface(g_screen, textSur1);
				SDL_Texture* textTex2 = SDL_CreateTextureFromSurface(g_screen, textSur2);
				SDL_Texture* textTex3 = SDL_CreateTextureFromSurface(g_screen, textSur3);
				 
				 dstRect1.x = 450, dstRect1.y = 150, dstRect1.w = textSur1->w, dstRect1.h = textSur1->h;
				 dstRect2.x = 450, dstRect2.y = 300, dstRect2.w = textSur2->w, dstRect2.h = textSur2->h;
				 dstRect3.x = 450, dstRect3.y = 450, dstRect3.w = textSur3->w, dstRect3.h = textSur3->h;
				menu_BK.Render(g_screen);
				
				SDL_RenderCopy(g_screen, textTex1, NULL, &dstRect1);
				SDL_RenderCopy(g_screen, textTex2, NULL, &dstRect2);
				SDL_RenderCopy(g_screen, textTex3, NULL, &dstRect3);
				
				std::string p_Chu[] = { "EXIT" };
				int x_m[] = { 1000 };
				int y_m[] = { 30 };
				k=p_high_score.ShowMenu(g_screen, g_font_text_1, p_Chu, 1, x_m, y_m, g_nhacnen[1]);
				
					SDL_DestroyTexture(textTex1);SDL_FreeSurface(textSur1);
					SDL_DestroyTexture(textTex2); SDL_FreeSurface(textSur2);
					SDL_DestroyTexture(textTex3); SDL_FreeSurface(textSur3);
					if (k == -1) return -1;
			} while (k > 0);
			kt_threat = 0; p_high_score.Free();
		}
		if (kt_threat == -1) return -1;
	} while (kt_threat < 2);
	p_menu_threat.Free();
	return kt_threat;
}
bool LoadBackground(int i) {
	char c[30];
	sprintf_s(c, "img/background%d.png", i);
	bool ret = g_background.LoadImag(c, g_screen);
	if (ret == false) return false;
	return true;
}
BaseObject p_game_over[6];
void LoadGameOver() {
	for (int i = 1; i <= 6; i++) {
		char Over[30];
		sprintf_s(Over, "Gameover/BKdie%d.png", i);
		p_game_over[i-1].LoadImag(Over, g_screen);
	}
}
void Begin_Threat(int i) {
	int x_, y_;
	p_threat[i].Init_Bullet(g_screen);
	do {
		x_ = rand() % MAX_MAP_X;
		y_ = rand() % MAX_MAP_Y;
	} while (game_map.getMap().tile[y_][x_] != BLANK_TILE);
	p_threat[i].Set_is_threat(true);
	p_threat[i].SetRect(x_ * TILE_SIZE, y_ * TILE_SIZE);
	p_threat[i].Set_blood(100);
}
void close() {
	TTF_CloseFont(g_font_text);
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen == NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
int Play(int luachon) {
	bool ret_b;
	int TT_Over = 1;
	bool GAME_OVER = false;
	bool SanSang = true;
	BaseObject sieunhanr, sieunhanl, suppermenr, suppermenl;
	ret_b = sieunhanr.LoadImag("img//Sieunhan(r).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = sieunhanl.LoadImag("img//Sieunhan(l).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenr.LoadImag("img//Suppermen(r).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenl.LoadImag("img//Suppermen(l).png", g_screen);
	if (ret_b == false) return 1;


	//-----------------------------------------------------------------

	int kt;// chon map---------------------------------
	kt = chosen_map();
	if (kt == -1) return 1;
	//---------------------------------------------
						// Load Map----------------------------------------------
	if (LoadBackground(kt) == false) return -1;
	LoadGameOver();
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
	bool is_quit = false;
	while (!is_quit) {
		fps_time.start();
		//-----------------------------------------------------------------------------------
		if (GAME_OVER == false && SanSang == false) {
			while (SDL_PollEvent(&g_event) != 0) {
				if (g_event.type == SDL_QUIT) {
					is_quit = true; break;
				}
				p_player.HandeInputAction(g_event, g_screen, g_sound_main_P1[0]);
				if (luachon == 0) P2_Player.HandeInputAction(g_event, g_screen, g_sound_main_P2[0]);
			}

			if (luachon == 2)P2_Player.Auto_(p_player.GetRect(), p_player.Get_Input_type(), g_screen, g_sound_main_P2[0]);
		}

		SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
		if (TT_Over < 16) {
			SDL_RenderClear(g_screen);
			g_background.Render(g_screen, NULL);
		}
		Map map_data = game_map.getMap();
		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);


		// Su ly game over----------------------------------------------

		if (p_player.Get_blood_main() <= 0 || P2_Player.Get_blood_main() <= 0) {
			GAME_OVER = true;
			if (P2_Player.Get_blood_main() <= 0) {
				P2_Player.Setinput_hurt(1);
			}
			else {
				p_player.Setinput_hurt(1);
			}
			BaseObject game_over;
			char Over[30];
			if (TT_Over < 5) {
				sprintf_s(Over, "Gameover/BKdie%d.png", TT_Over);
				p_game_over[TT_Over - 1].Render(g_screen); SDL_Delay(30); TT_Over++;
			}
			else if(TT_Over<15) {
				sprintf_s(Over, "Gameover/BKdie%d.png", (TT_Over % 2) + 5);
				p_game_over[(TT_Over)%2 + 4].Render(g_screen); SDL_Delay(30); TT_Over++;
			}
			else if (TT_Over >= 16) {
				Mix_HaltChannel(Mix_PlayChannel(-1, g_nhacnen[0], 0));
				Mix_PlayChannel(-1, g_nhacnen[3], 0);
				Mix_PlayChannel(-1, g_nhacnen[4], 0);
				game_over.LoadImag("Gameover/KO.png", g_screen);
				game_over.SetRect(500, 200);
				game_over.Render(g_screen);
				SDL_RenderPresent(g_screen);
				SDL_Delay(2000);
				game_over.LoadImag("Gameover/win.png", g_screen);
				if (P2_Player.Get_blood_main() <= 0) {
					game_over.SetRect(50, 100);
				}
				else {
					game_over.SetRect(700, 100);
				}
				game_over.Render(g_screen);
				SDL_RenderPresent(g_screen);
				SDL_Delay(2000);
				g_background.Free();
				return 0;
				}
				else TT_Over++;
				game_over.Free();
		}

		// ve nhan vat-----------------------------------------------------

		if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 0) {
			suppermenl.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			if (p_player.Get_Input_type().defend == 0)
			suppermenl.Render(g_screen);
		}
		else if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 1) {
			suppermenr.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			if (p_player.Get_Input_type().defend == 0)
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

		//--------------------------------------------------
		// nhan vat 1
		p_player.Doplayer(map_data);
		p_player.HandleBuller(g_screen);
		if (p_player.Get_Input_type().defend == 1) {
			p_player.Show_Defend(g_screen);
			p_player.Set_ki_main(p_player.Get_ki_main() - 5);
		}
		else p_player.Show(g_screen);
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
			P2_Player.Show_Defend(g_screen);
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
		//--------------------------------------------------------------SU LY VA CHAM-------------------------------------------
		SDL_Rect P1_Rect = p_player.GetRect();
		P1_Rect.w /= 8;
		SDL_Rect P2_Rect = P2_Player.GetRect();
		P2_Rect.w /= 8;
		// su ly va cham giua DAN Main 1 voi Main 2 
		std::vector<BulletObject*> bullet_list = p_player.get_bullet_list();
		for (int im = 0; im < bullet_list.size(); im++) {
			BulletObject* p_amo = bullet_list.at(im);
			if (p_amo->Check_Map(map_data) && p_amo!=NULL)  p_player.Remove_Bullet(im);
			if (p_amo != NULL) {
				bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), P2_Rect);
				if (P2_Player.Get_Input_type().defend == 1 || P2_Player.Get_Input_type().bullet_Skill_I == 1) ret_col = false;
				if (ret_col) {
					if (p_amo->GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
					else P2_Player.set_status(1);
					P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 10);
					if (TT_Over < 10)  Mix_PlayChannel(-1, g_nhacnen[0], 0);
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
			if (p_amo->Check_Map(map_data) && p_amo!=NULL)  P2_Player.Remove_Bullet(im);
			if (p_amo != NULL) {
				bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), P1_Rect);
				if (p_player.Get_Input_type().defend == 1 && ret_col) {
					ret_col = false; P2_Player.Remove_Bullet(im);
				}
				if (ret_col) {
					if (p_amo->GetRect().x > p_player.GetRect().x) p_player.set_status(0);
					else p_player.set_status(1);
					p_player.Set_blood_main(p_player.Get_blood_main() - 10);
					if(TT_Over<10) Mix_PlayChannel(-1, g_nhacnen[0], 0);
					p_player.Setinput_hurt(1);
					SDL_RenderPresent(g_screen);
					P2_Player.Remove_Bullet(im);
				}
			}
		}
		//Su ly va cham giua Skill U p1 vs Main 2
		bool ret_col;
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(1).GetRect(), P2_Rect);
		if (P2_Player.Get_Input_type().defend == 1 || p_player.Get_Move_u() == false) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);

			P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 100);
			if (TT_Over < 10)  Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Set_Move_U(false);
			P2_Player.Setinput_hurt(1);

		}
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(0).GetRect(), P2_Rect);
		if (P2_Player.Get_Input_type().defend == 1 || p_player.Get_Move_u() == false) ret_col = false;
		if (p_player.Get_Input_type().bullet_Skill_U == 0) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);
			P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 20);
			if (TT_Over < 10)  Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Set_Move_U(false);
			P2_Player.Setinput_hurt(1);

		}
		// Su ly va cham giua Skill I p1 vs Main P2
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_BigSize().GetRect(), P2_Rect);
		if (P2_Player.Get_Input_type().defend == 1) ret_col = false;
		if (p_player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);
			P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 15);
			if (TT_Over < 10)  Mix_PlayChannel(-1, g_nhacnen[0], 0);
			P2_Player.Setinput_hurt(1);
		}

		// Su ly va cham giua SKill U p2 vs Main 1
		ret_col = false;
		if (P2_Player.Get_Input_type().bullet_Skill_U == 1) ret_col = true;
		if (p_player.Get_Input_type().defend == 1) ret_col = false;
		if (ret_col == true) {
			if (P2_Player.get_status() == 1) p_player.set_status(0);
			else p_player.set_status(1);
			p_player.Set_blood_main(p_player.Get_blood_main() - 3);
			if (TT_Over < 10)  Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Setinput_hurt(1);
		}

		// Su ly va cham Giua SKill I p2 vs Main 1
		if (P2_Rect.x <= 1) P2_Rect.x = 5;
		else if (P2_Rect.x >= SCREEN_WIDTH - 65) {
			P2_Rect.x = SCREEN_WIDTH - 66;
		}
		ret_col = SDLCommonFunc::CheckCollision(P2_Rect, P1_Rect);
		if (P2_Player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
		if (p_player.Get_Input_type().defend == 1) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x < P2_Player.GetRect().x) p_player.set_status(0);
			else p_player.set_status(1);
			p_player.Set_blood_main(p_player.Get_blood_main() - 15);
			if (TT_Over < 1)  Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Setinput_hurt(1);

		}

		//--------------------------------------------------------------------------------------



// Ve mau // ki cho nhan vat P1
		// ve mau
		if (p_player.Get_blood_main() <= 0) p_player.Set_blood_main(0);
		if (P2_Player.Get_blood_main() <= 0) P2_Player.Set_blood_main(0);
		GeometricFormat rectangle_size1(50, 20, p_player.Get_blood_main() / 3, 20);
		ColorData color_data1(37, 121, 28);
		Geometric::RenderRecttangle(rectangle_size1, color_data1, g_screen);

		GeometricFormat outline_size1(50, 20, 501, 22);
		ColorData color1(37, 121, 28);
		Geometric::RenderOutline(outline_size1, color1, g_screen);
		// ve ki
		if (p_player.Get_ki_main() <= 0) p_player.Set_ki_main(0);
		if (p_player.Get_ki_main() >= 1500)  p_player.Set_ki_main(1499);
		int ki1 = p_player.Get_ki_main();
		GeometricFormat rectangle_size_ki1(50, SCREEN_HEIGHT - 50, ki1 / 3, 20);
		ColorData color_data_ki1(0, 162, 232);
		Geometric::RenderRecttangle(rectangle_size_ki1, color_data_ki1, g_screen);

		GeometricFormat outline_size_ki1(50, SCREEN_HEIGHT - 50, 501, 22);
		ColorData color_ki1(0, 162, 232);
		Geometric::RenderOutline(outline_size_ki1, color_ki1, g_screen);


		//Ve mau // ki cho nhan vat P2
				// ve mau 

		GeometricFormat rectangle_size2(SCREEN_WIDTH / 2 + 100, 20, P2_Player.Get_blood_main() / 3, 20);
		ColorData color_data2(37, 121, 28);
		Geometric::RenderRecttangle(rectangle_size2, color_data2, g_screen);

		GeometricFormat outline_size2(SCREEN_WIDTH / 2 + 100, 20, 501, 22);
		ColorData color2(37, 121, 28);
		Geometric::RenderOutline(outline_size2, color2, g_screen);
		// ve ki
		if (P2_Player.Get_ki_main() <= 0) P2_Player.Set_ki_main(0);
		if (P2_Player.Get_ki_main() >= 1500)  P2_Player.Set_ki_main(1499);
		int ki2 = P2_Player.Get_ki_main();
		GeometricFormat rectangle_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, ki2 / 3, 20);
		ColorData color_data_ki2(0, 162, 232);
		Geometric::RenderRecttangle(rectangle_size_ki2, color_data_ki2, g_screen);

		GeometricFormat outline_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, 501, 22);
		ColorData color_ki2(0, 162, 232);
		Geometric::RenderOutline(outline_size_ki2, color_ki2, g_screen);
		//------------------------------------------------------------------------------------

		//SU ly thoi gian
		if (SanSang == false) {
			std::string str_time = "Time : ";
			Uint32 time_val = (SDL_GetTicks() / 1000) - time_val_start;
			if (time_val % 3 == 1) {
				p_player.Set_ki_main(p_player.Get_ki_main() + 10);
				P2_Player.Set_ki_main(P2_Player.Get_ki_main() + 10);
			}
			std::string str_val = std::to_string(time_val);
			str_time += str_val;

			SDL_Color textColor = { 217, 65, 38 };

			SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text, str_time.c_str(), textColor);
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
			SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 70, 20, textSurface->w, textSurface->h };
			SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);
			SDL_FreeSurface(textSurface);
			SDL_DestroyTexture(textTexture);
		}
		//-------------------------------------------------------------------------------------
		//su ly bat dau tro choi 
		if (SanSang && p_player.get_on_ground()) {
			BaseObject game_start;
			Mix_PlayChannel(-1, g_nhacnen[2], 0);
			game_start.LoadImag("Gameover/Ready.png", g_screen);
			game_start.SetRect(500, 100);
			game_start.Render(g_screen);
			SDL_RenderPresent(g_screen);
			SDL_Delay(2000);
			game_start.LoadImag("Gameover/Go.png", g_screen);
			////-------------------------------
			g_background.Render(g_screen);
			game_map.DrawMap(g_screen);
			suppermenl.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			suppermenl.Render(g_screen);
			sieunhanr.SetRect(P2_Player.GetRect().x, P2_Player.GetRect().y);
			sieunhanr.Render(g_screen);
			////-------------------------------
			game_start.SetRect(550, 100);
			game_start.Render(g_screen);
			SDL_RenderPresent(g_screen);
			SDL_Delay(500);
			game_start.Free();
			time_val_start = SDL_GetTicks() / 1000;
			SanSang = false;
		}

		SDL_RenderPresent(g_screen);
		// huy cac con tro tranh tran bo nho --------------------------
		p_player.Free();
		P2_Player.Free();


		int real_imp_time = fps_time.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;
		if (real_imp_time < time_one_frame) {
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}

	}
	return 1;

}
int Play_Threat() {
	int dem = 1;
	int Mark_P1 = 0;
	int Mark_P2 = 0;
	bool start_play = false;
	bool SanSang = true;
	bool ret_b;
	int Num_Threat = 2;
	int kt;
	int HIGH_SCORE[3] = { 0,0,0 };
	//-----------------------------------------------------------------
	
	int CHON_TILE_PLAY = chosen_threat(Num_Threat, HIGH_SCORE);

	if (CHON_TILE_PLAY == 4) Num_Threat*=2;
	if (CHON_TILE_PLAY == -1) return 1;
	BaseObject sieunhanr, sieunhanl, suppermenr, suppermenl;
	if (CHON_TILE_PLAY!=2 ) {
		ret_b = sieunhanr.LoadImag("img//Sieunhan(r).png", g_screen);
		if (ret_b == false) return 1;
		ret_b = sieunhanl.LoadImag("img//Sieunhan(l).png", g_screen);
		if (ret_b == false) return 1;
	}
	if (CHON_TILE_PLAY!=3) {
		ret_b = suppermenr.LoadImag("img//Suppermen(r).png", g_screen);
		if (ret_b == false) return 1;
		ret_b = suppermenl.LoadImag("img//Suppermen(l).png", g_screen);
		if (ret_b == false) return 1;
	}
						// chon map---------------------------------
	kt = chosen_map();
	if (kt == -1) return 1;
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
	if (CHON_TILE_PLAY != 3) {
		p_player.LoadImag("img//Suppermenright.png", g_screen);
		p_player.Set_clip();
		p_player.ktImage(g_screen);
	}
	//Khoi tao nhan vat 2
	Main_P2_Object P2_Player;
		int ret_P1_x = 0;
	if (CHON_TILE_PLAY != 2) {
		P2_Player.LoadImag("img//SieunhanLeft.png", g_screen);
		P2_Player.Set_clip();
		P2_Player.ktImage(g_screen);
	}
	// khot tao vuno
	for (int i = 0; i < Num_Threat; i++) {
		p_threat[i].Load_Threat(g_screen);
		p_threat[i].set_hoisinh(false);
		p_vuno[i].LoadImag("img//vuno.png", g_screen);
	}
	// ---------------------------------------------------------------VONG LAP WHILE--------------------------------------------------------
	time_val_start = SDL_GetTicks() / 1000;
	bool is_quit = false;
	while (!is_quit) {
		fps_time.start();
		//-----------------------------------------------------------------------------------
		if ( SanSang == false) {
			while (SDL_PollEvent(&g_event) != 0) {
				if (g_event.type == SDL_QUIT) {
					is_quit = true; break;
				}
				if(CHON_TILE_PLAY !=3) p_player.HandeInputAction(g_event, g_screen, g_sound_main_P1[0]);
				if(CHON_TILE_PLAY !=2) P2_Player.HandeInputAction(g_event, g_screen, g_sound_main_P2[0]);
			}
		}

		SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
		SDL_RenderClear(g_screen);
		g_background.Render(g_screen, NULL);
		Map map_data = game_map.getMap();
		game_map.DrawMap(g_screen);
		// ve nhan vat-----------------------------------------------------
		if (CHON_TILE_PLAY != 3) {
			if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 0) {
				suppermenl.SetRect(p_player.GetRect().x, p_player.GetRect().y);
				suppermenl.Render(g_screen);
			}
			else if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 1) {
				suppermenr.SetRect(p_player.GetRect().x, p_player.GetRect().y);
				suppermenr.Render(g_screen);
			}
		}
		if (CHON_TILE_PLAY != 2) {
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
		}
		//--------------------------------------------------

		// nhan vat 1

		p_player.Doplayer(map_data);
		p_player.HandleBuller(g_screen);
		if (p_player.Get_Input_type().defend == 1) {
			p_player.Show_Defend(g_screen);
			p_player.Set_ki_main(p_player.Get_ki_main() - 5);
		}
		else {
			if(CHON_TILE_PLAY!=3) p_player.Show(g_screen);
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
		}p_player.Set_Move_U(false);


		//--------------------------------------------------
		// nhan vat 2
		P2_Player.Doplayer(map_data);
		P2_Player.HandleBuller(g_screen);
		if (P2_Player.Get_Input_type().defend == 1) {
			P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 5);
			P2_Player.Show_Defend(g_screen);
		}
		else {
			if (CHON_TILE_PLAY != 2) P2_Player.Show(g_screen);
			if (P2_Player.Get_Input_type().bullet_Skill_I == 1) {
				// Am thanh I
				P2_Player.Set_ki_main(P2_Player.Get_ki_main() - 20);
				Mix_PlayChannel(-1, g_sound_main_P2[2], 0);
			}

		}


		//--------------------------------------------------
		//-------------------Su ly Threat-------------------------------
		game_map.SetMap(map_data);
		for (int i = 0; i < Num_Threat; i++) {
			p_threat[i].HOI_SINH();
			if (!p_threat[i].Get_is_threat()) {
				if (start_play == true) {
					p_vuno[i].SetRect(p_threat[i].GetRect().x, p_threat[i].GetRect().y);
					p_vuno[i].Render(g_screen);
				}
				if (p_threat[i].get_hoisinh()) {
					Begin_Threat(i); start_play = true;
					Mix_PlayChannel(-1, g_sound_rong[0], 0);
				}
			}
			if (p_threat[i].Get_is_threat()) {
				if (CHON_TILE_PLAY == 2) {
					p_threat[i].Set_goal_x(p_player.GetRect().x / TILE_SIZE);
					p_threat[i].Set_goal_y((p_player.GetRect().y) / TILE_SIZE);
				}
				else if (CHON_TILE_PLAY == 3) {
					p_threat[i].Set_goal_x(P2_Player.GetRect().x / TILE_SIZE);
					p_threat[i].Set_goal_y((P2_Player.GetRect().y) / TILE_SIZE );
				}
				else {
					if (i < Num_Threat / 2) {
						p_threat[i].Set_goal_x(p_player.GetRect().x / TILE_SIZE);
						p_threat[i].Set_goal_y((p_player.GetRect().y) / TILE_SIZE);
					}
					else {
						p_threat[i].Set_goal_x(P2_Player.GetRect().x / TILE_SIZE);
						p_threat[i].Set_goal_y((P2_Player.GetRect().y) / TILE_SIZE );
					}
				}
				p_threat[i].Search(p_threat[i].GetRect().x / TILE_SIZE, p_threat[i].GetRect().y / TILE_SIZE, game_map.getMap());
				p_threat[i].HandleBuller(g_screen);

				p_threat[i].Render_Threat(g_screen, dem / 5);
				dem++;
				if (dem >= 16) { if(i==0)Mix_PlayChannel(-1, g_sound_rong[1], 0);  dem = 0; }
				p_threat[i].Draw_blood(g_screen);
			}
		}


		//--------------------------------------------------------------SU LY VA CHAM-------------------------------------------


			//Su lu va cham giua dan cua Main 2 va Threat
		std::vector<BulletObject*> bullet_list2 = P2_Player.get_bullet_list();
		for (int im = 0; im < bullet_list2.size(); im++) {
			BulletObject* p_amo = bullet_list2.at(im);
			if (p_amo != NULL) {
				for (int k = 0; k < Num_Threat; k++) {
					bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_threat[k].GetRect());
					if (!p_threat[k].Get_is_threat()) {
						ret_col = false;
					}
					if (ret_col) {
						p_threat[k].Set_blood(p_threat[k].Get_blood() - 10);
						SDL_RenderPresent(g_screen);
						P2_Player.Remove_Bullet(im);
						if (p_threat[k].Get_blood() <= 0) {
							for (int j = 0; j < p_threat[k].get_bullet_list().size(); j++) {
								p_threat[k].Remove_Bullet(j);
							}

							Mark_P2++;
							SDL_RenderPresent(g_screen);

							p_threat[k].Free();
							p_threat[k].Set_is_threat(false);
							p_threat[k].set_hoisinh(false);
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
					if (!p_threat[k].Get_is_threat()) ret_col = false;
					if (ret_col) {
						p_threat[k].Set_blood(p_threat[k].Get_blood() - 10);
						SDL_RenderPresent(g_screen);
						p_player.Remove_Bullet(im);
						if (p_threat[k].Get_blood() <= 0) {
							for (int j = 0; j < p_threat[k].get_bullet_list().size(); j++) {
								p_threat[k].Remove_Bullet(j);
							}
							Mark_P1++;
							SDL_RenderPresent(g_screen);

							p_threat[k].Free();
							p_threat[k].Set_is_threat(false);
							p_threat[k].set_hoisinh(false);
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
				if(CHON_TILE_PLAY==2) ret_col = false;
				if (P2_Player.Get_Input_type().defend == 1 || P2_Player.Get_Input_type().bullet_Skill_I == 1) ret_col = false;
				if (ret_col) {
					if (p_amo->GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
					else P2_Player.set_status(1);
					P2_Player.Set_blood_main(P2_Player.Get_blood_main() - 5);
					Mix_PlayChannel(-1, g_nhacnen[0], 0);
					P2_Player.Setinput_hurt(1);

					p_threat[im].Remove_Bullet(k);
					p_threat[im].Init_Bullet(g_screen);
				}

				//----------------------------------------------------	
				ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p1_rect);
				if (CHON_TILE_PLAY == 3) ret_col = false;
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

					p_threat[im].Remove_Bullet(k);
					p_threat[im].Init_Bullet(g_screen);
				}

			}
		}

		// Su ly va cham giua Skill I p1 vs Threat
		for (int tt = 0; tt < Num_Threat; tt++) {
			bool ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_BigSize().GetRect(), p_threat[tt].GetRect());
			if (p_player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
			if (!p_threat[tt].Get_is_threat()) ret_col = false;
			if (ret_col == true) {
				p_threat[tt].Set_blood(p_threat[tt].Get_blood() - 30);
				if (p_threat[tt].Get_blood() <= 0) {
					for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
						p_threat[tt].Remove_Bullet(j);
					}
					Mark_P1++;
					SDL_RenderPresent(g_screen);
					p_threat[tt].Free();
					p_threat[tt].Set_is_threat(false);
					p_threat[tt].set_hoisinh(false);
				}
			}
		}
		// Su ly va cham giua Skill U p1 vs Threat
		for (int tt = 0; tt < Num_Threat; tt++) {
			bool ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(0).GetRect(), p_threat[tt].GetRect());
			if (p_player.Get_Input_type().bullet_Skill_U == 0) ret_col = false;
			if (!p_threat[tt].Get_is_threat()) ret_col = false;
			if (ret_col == true) {
				p_threat[tt].Set_blood(p_threat[tt].Get_blood() - 30);
				if (p_threat[tt].Get_blood() <= 0) {
					for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
						p_threat[tt].Remove_Bullet(j);
					}
					Mark_P1++;
					SDL_RenderPresent(g_screen);
					p_threat[tt].Free();
					p_threat[tt].Set_is_threat(false);
					p_threat[tt].set_hoisinh(false);
				}
			}
		}
		// Su ly va cham Giua SKill I p2 vs Threat
		for (int tt = 0; tt < Num_Threat; tt++) {
			bool ret_col = SDLCommonFunc::CheckCollision(p2_rect, p_threat[tt].GetRect());
			if (P2_Player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
			if (!p_threat[tt].Get_is_threat()) ret_col = false;
			if (ret_col == true) {
				p_threat[tt].Set_blood(p_threat[tt].Get_blood() - 40);
				if (p_threat[tt].Get_blood() <= 0) {
					for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
						p_threat[tt].Remove_Bullet(j);
					}
					Mark_P2++;
					SDL_RenderPresent(g_screen);
					p_threat[tt].Free();
					p_threat[tt].Set_is_threat(false);
					p_threat[tt].set_hoisinh(false);
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
			if (ret_col) ret_col = SDLCommonFunc::CheckCollision(p2_rect, p_threat[tt].GetRect());
			if (!p_threat[tt].Get_is_threat()) ret_col = false;
			if (ret_col == true) {
				for (int j = 0; j < p_threat[tt].get_bullet_list().size(); j++) {
					p_threat[tt].Remove_Bullet(j);
				}
				p_vuno[tt].SetRect(p_threat[tt].GetRect().x, p_threat[tt].GetRect().y);
				p_vuno[tt].Render(g_screen);
				Mark_P2++;
				SDL_RenderPresent(g_screen);
				p_threat[tt].Free();
				p_threat[tt].Set_is_threat(false);
				p_threat[tt].set_hoisinh(false);
			}
		}


		//--------------------------------------------------------------------------------------		

// Ve mau // ki cho nhan vat P1
		// ve mau
		if (CHON_TILE_PLAY != 3) {
			GeometricFormat rectangle_size1(50, 20, p_player.Get_blood_main() / 3, 20);
			ColorData color_data1(37, 121, 28);
			Geometric::RenderRecttangle(rectangle_size1, color_data1, g_screen);

			GeometricFormat outline_size1(50, 20, 501, 22);
			ColorData color1(37, 121, 28);
			Geometric::RenderOutline(outline_size1, color1, g_screen);
			// ve ki
			if (p_player.Get_ki_main() <= 0) p_player.Set_ki_main(0);
			if (p_player.Get_ki_main() >= 1500)  p_player.Set_ki_main(1499);
			GeometricFormat rectangle_size_ki1(50, SCREEN_HEIGHT - 50, p_player.Get_ki_main()/3, 20);
			ColorData color_data_ki1(0, 162, 232);
			Geometric::RenderRecttangle(rectangle_size_ki1, color_data_ki1, g_screen);

			GeometricFormat outline_size_ki1(50, SCREEN_HEIGHT - 50, 501, 22);
			ColorData color_ki1(0, 162, 232);
			Geometric::RenderOutline(outline_size_ki1, color_ki1, g_screen);
		}

		//Ve mau // ki cho nhan vat P2
				// ve mau 
		if (CHON_TILE_PLAY != 2) {
			GeometricFormat rectangle_size2(SCREEN_WIDTH / 2 + 100, 20, P2_Player.Get_blood_main() / 3, 20);
			ColorData color_data2(37, 121, 28);
			Geometric::RenderRecttangle(rectangle_size2, color_data2, g_screen);

			GeometricFormat outline_size2(SCREEN_WIDTH / 2 + 100, 20, 501, 22);
			ColorData color2(37, 121, 28);
			Geometric::RenderOutline(outline_size2, color2, g_screen);
			// ve ki
			if (P2_Player.Get_ki_main() <= 0) P2_Player.Set_ki_main(0);
			if (P2_Player.Get_ki_main() >= 1500)  P2_Player.Set_ki_main(1499);
			
			GeometricFormat rectangle_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, P2_Player.Get_ki_main() /3 , 20);
			ColorData color_data_ki2(0, 162, 232);
			Geometric::RenderRecttangle(rectangle_size_ki2, color_data_ki2, g_screen);

			GeometricFormat outline_size_ki2(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT - 50, 501, 22);
			ColorData color_ki2(0, 162, 232);
			Geometric::RenderOutline(outline_size_ki2, color_ki2, g_screen);
		}
		//------------------------------------------------------------------------------------
		//SU ly thoi gian

		std::string str_time = "Time : ";
		Uint32 time_val = (SDL_GetTicks() / 1000) - time_val_start;
		if (time_val % 3 == 1) {
			p_player.Set_ki_main(p_player.Get_ki_main() + 10);
			P2_Player.Set_ki_main(P2_Player.Get_ki_main() + 10);

		}
		std::string str_val = std::to_string(200 - time_val);
		str_time += str_val;

		SDL_Color textColor = { 217, 65, 38 };

		SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text, str_time.c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
		SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 70, 20, textSurface->w, textSurface->h };
		SDL_RenderCopy(g_screen, textTexture, NULL, &dstRect);

		//-------------------------------------------------------------------------------------
		// --------SU ly bat dau tro choi-------------------------------
		if (SanSang && p_player.get_on_ground()) {
			BaseObject game_start;
			Mix_PlayChannel(-1, g_nhacnen[2], 0);
			game_start.LoadImag("Gameover/Ready.png", g_screen);
			game_start.SetRect(500, 100);
			game_start.Render(g_screen);
			SDL_RenderPresent(g_screen);
			SDL_Delay(2000);
			game_start.LoadImag("Gameover/Go.png", g_screen);
			////-------------------------------
			g_background.Render(g_screen);
			game_map.DrawMap(g_screen);
			suppermenl.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			suppermenl.Render(g_screen);
			sieunhanr.SetRect(P2_Player.GetRect().x, P2_Player.GetRect().y);
			sieunhanr.Render(g_screen);
			////-------------------------------
			game_start.SetRect(550, 100);
			game_start.Render(g_screen);
			SDL_RenderPresent(g_screen);
			SDL_Delay(500);
			game_start.Free();
			time_val_start = SDL_GetTicks() / 1000;
			SanSang = false;
		}

		// Su ly game over----------------------------------------------

		if ((p_player.Get_blood_main() <= 0 || P2_Player.Get_blood_main() <= 0)||( time_val >= 200)) {
			GeometricFormat rectangle(SCREEN_WIDTH / 2 - 400, 100, 800, 150);
			ColorData color(40, 0, 0);
			Geometric::RenderRecttangle(rectangle, color, g_screen);

			SDL_Color textColor = { 255, 128, 0 };
			std::string Score = "SCORE: " + std::to_string(Mark_P1 + Mark_P2);
			SDL_Surface* textSurface = TTF_RenderText_Solid(g_font_text_2,Score.c_str(), textColor);
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
			SDL_Rect dstRect = { SCREEN_WIDTH / 2 - 300, 100, textSurface->w, textSurface->h };
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
			out.open("HighScore.txt");
			if (CHON_TILE_PLAY == 2) {
				if (Mark_P1 > HIGH_SCORE[0]) out << Mark_P1 << std::endl;
				else out << HIGH_SCORE[0]<<std::endl;
				out << HIGH_SCORE[1] << std::endl;
				out << HIGH_SCORE[2];
			}
			else if (CHON_TILE_PLAY == 3) {
				out << HIGH_SCORE[0] << std::endl;
				if (Mark_P2 > HIGH_SCORE[1]) out << Mark_P2 << std::endl;
				else out << HIGH_SCORE[1] << std::endl;
				out << HIGH_SCORE[2];
			}
			else {
				out << HIGH_SCORE[0] << std::endl;
				out << HIGH_SCORE[1] << std::endl;
				if (Mark_P1 + Mark_P2 > HIGH_SCORE[2]) out << Mark_P1 + Mark_P1;
				else out << HIGH_SCORE[2];
			}
			out.close();
			return 0;
		}
		//------------------------------------------------------------------


		//------------------Su ly diem----------------
		if (CHON_TILE_PLAY != 3) {
			std::string str_mark_p1 = "Mark 1 : ";
			std::string mark1_val = std::to_string(Mark_P1);
			str_mark_p1 += mark1_val;
			SDL_Color textColor1 = { 255,255,255 };
			SDL_Surface* textSurface_mark1 = TTF_RenderText_Solid(g_font_text, str_mark_p1.c_str(), textColor1);
			SDL_Texture* textTexture_mark1 = SDL_CreateTextureFromSurface(g_screen, textSurface_mark1);
			SDL_Rect dstRect1 = { 170, 60, textSurface_mark1->w, textSurface_mark1->h };
			SDL_RenderCopy(g_screen, textTexture_mark1, NULL, &dstRect1);
			SDL_FreeSurface(textSurface_mark1);
			SDL_DestroyTexture(textTexture_mark1);
		}
		if (CHON_TILE_PLAY != 2) {
			std::string str_mark_p2 = "Mark 2 : ";
			std::string mark2_val = std::to_string(Mark_P2);
			str_mark_p2 += mark2_val;
			SDL_Color textColor2 = { 255,255, 255 };
			SDL_Surface* textSurface_mark2 = TTF_RenderText_Solid(g_font_text, str_mark_p2.c_str(), textColor2);
			SDL_Texture* textTexture_mark2 = SDL_CreateTextureFromSurface(g_screen, textSurface_mark2);
			SDL_Rect dstRect2 = { SCREEN_WIDTH / 2 + 170, 60, textSurface_mark2->w, textSurface_mark2->h };
			SDL_RenderCopy(g_screen, textTexture_mark2, NULL, &dstRect2);
			SDL_FreeSurface(textSurface_mark2);
			SDL_DestroyTexture(textTexture_mark2);
		}
		//----------------------------------------------------------------------------------------

		SDL_RenderPresent(g_screen);
		SDL_RenderClear(g_screen);
		// huy cac con tro tranh tran bo nho --------------------------
		p_player.Free();
		P2_Player.Free();
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);

		// su ly toc do game 
		int real_imp_time = fps_time.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;
		if (real_imp_time < time_one_frame) {
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}


	}
	return 1;
}
int Play_Huongdan() {
	bool ret_b;
	BaseObject sieunhanr, sieunhanl, suppermenr, suppermenl;
	BaseObject p_exit;
	p_exit.LoadImag("img/huong_dan/Exit.png", g_screen);
	p_exit.SetRect(1100, 20);
	ret_b = sieunhanr.LoadImag("img//Sieunhan(r).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = sieunhanl.LoadImag("img//Sieunhan(l).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenr.LoadImag("img//Suppermen(r).png", g_screen);
	if (ret_b == false) return 1;
	ret_b = suppermenl.LoadImag("img//Suppermen(l).png", g_screen);
	if (ret_b == false) return 1;
	BaseObject p_nv_huongdan;
	p_nv_huongdan.LoadImag("img/huong_dan/nv_huongdan.png", g_screen);

	//-----------------------------------------------------------------

	ImpTimer Time_Delay;

	//---------------------------------------------
						// Load Map----------------------------------------------
	if (LoadBackground(1) == false) return -1;
	game_map.LoadMap("map/map5.txt");
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
	BaseObject p_huongdan[4];
	BaseObject p_goi_y[4];
	p_huongdan[0].LoadImag("img/huong_dan/hd1.png", g_screen);
	p_goi_y[0].LoadImag("img/huong_dan/goiy1.png", g_screen);
	p_huongdan[1].LoadImag("img/huong_dan/hd2.png", g_screen);
	p_goi_y[1].LoadImag("img/huong_dan/goiy2.png", g_screen);

	BaseObject p_muiten;
	p_muiten.LoadImag("img/huong_dan/Muiten.png", g_screen);
	int dx_muiten[10] = { 750,670,558,650,595,590,750,760};
	int dy_muiten[10] = { 400,340,400,400,340,430,340,430};
	// vong lap while---------------------------------------------------------------
	bool is_quit = false;
	int d[11] = { 250,225,200,175,150,125,100,75,50,25,0};
	int dy = 0; bool start_game = false;
	int Status_huongdan = 0;
	while (!is_quit) {
		fps_time.start();
		//-----------------------------------------------------------------------------------
		while (SDL_PollEvent(&g_event) != 0) {
			if (g_event.type == SDL_QUIT) {
				is_quit = true; break;
			}
			if (g_event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX, mouseY;
				Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
				for (int i = 0; i < 4; i++) {
					if (SDLCommonFunc::Checkvitri(mouseX, mouseY, p_exit.GetRect())) return 0;
				}
			}p_player.HandeInputAction(g_event, g_screen, g_sound_main_P1[0]);
			//P2_Player.HandeInputAction(g_event, g_screen, g_sound_main_P2[0]);
		}

		SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);

		SDL_RenderClear(g_screen);
		g_background.Render(g_screen, NULL);

		Map map_data = game_map.getMap();
		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);
		p_exit.Render(g_screen);
		if (p_player.get_on_ground() && start_game == false) {
			Time_Delay.start();
			start_game = true;
		}
		if (start_game) {
			if (Status_huongdan == 10) {
				Status_huongdan = 4;
				dy = 0;
			}
			p_nv_huongdan.SetRect(650, 250 - d[dy]);
			p_nv_huongdan.Render(g_screen);
			dy++;
			if (dy >= 10) dy = 10;
			if (Status_huongdan < 4) {
				p_huongdan[0].SetRect(500, 400);
				if (dy == 10) p_huongdan[0].Render(g_screen);

				p_goi_y[0].SetRect(700, 20);
				if (dy == 10) p_goi_y[0].Render(g_screen);
			}
			else {
				p_huongdan[1].SetRect(500, 400);
				if (dy == 10) p_huongdan[1].Render(g_screen);

				p_goi_y[1].SetRect(700, 20);
				if (dy == 10) p_goi_y[1].Render(g_screen);
			}

			p_muiten.SetRect(dx_muiten[Status_huongdan], dy_muiten[Status_huongdan]);
			if (dy == 10)p_muiten.Render(g_screen);
		}
		if (p_player.Get_Input_type().right == 1 && Status_huongdan == 0) {
			if (Time_Delay.get_ticks() >= 2000) { Status_huongdan++; Time_Delay.start(); }
		}
		if (p_player.get_on_ground() == false && Status_huongdan == 1) {
			if (Time_Delay.get_ticks() >= 2000) { Status_huongdan++; Time_Delay.start(); }
		}
		if (p_player.Get_Input_type().left == 1 && Status_huongdan == 2) {
			if (Time_Delay.get_ticks() >= 2000) { Status_huongdan++; Time_Delay.start(); }
		}
		if (p_player.Get_Input_type().bullet_Skill_U == 1 && Status_huongdan == 4) {
			if (Time_Delay.get_ticks() >= 2000) { Status_huongdan++; Time_Delay.start(); }
		}
		if (p_player.Get_Input_type().bullet_Skill_J == 1 && Status_huongdan == 5)
		{
			if (Time_Delay.get_ticks() >= 2000) { Status_huongdan++; Time_Delay.start(); }
		}
		if (p_player.Get_Input_type().bullet_Skill_I == 1 && Status_huongdan == 6) {
			if (Time_Delay.get_ticks() >= 2000) { Status_huongdan++; Time_Delay.start(); }
		}

		P2_Player.exchange_input(Status_huongdan, g_screen);

		// ve nhan vat-----------------------------------------------------

		if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 0) {
			suppermenl.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			if (p_player.Get_Input_type().defend == 0)
				suppermenl.Render(g_screen);
		}
		else if (p_player.Get_Input_type().empty == 1 && p_player.get_status() == 1) {
			suppermenr.SetRect(p_player.GetRect().x, p_player.GetRect().y);
			if (p_player.Get_Input_type().defend == 0)
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
		p_player.Set_ki_main(1499);
		//--------------------------------------------------
		// nhan vat 1
		p_player.Doplayer(map_data);
		p_player.HandleBuller(g_screen);
		if (p_player.Get_Input_type().defend == 1) {
			p_player.Show_Defend(g_screen);
		}
		else p_player.Show(g_screen);
		if (!p_player.Get_Move_u()) {
			ret_P1_x = P2_Player.GetRect().x - 20;
		}
		if (p_player.Get_Input_type().bullet_Skill_I == 1) {
			//Am thanh Sung I
			Mix_PlayChannel(-1, g_sound_main_P1[2], 0);
			p_player.Show_Bullet_Size(g_screen);
		}
		if (p_player.Get_Move_u()) {
			//Am thanh Sung U
			Mix_PlayChannel(-1, g_sound_main_P1[1], 0);
			p_player.Show_Bullet_Skill_U(g_screen);
			p_player.Buller_move_U(map_data, ret_P1_x);
		}

		//--------------------------------------------------
		// nhan vat 2
		P2_Player.Doplayer(map_data);
		P2_Player.HandleBuller(g_screen);
		if (P2_Player.Get_Input_type().defend == 1) {
			P2_Player.Show_Defend(g_screen);
		}
		else {
			P2_Player.Show(g_screen);

			if (P2_Player.Get_Input_type().bullet_Skill_U == 1) {
				// Am thanh  U
				Mix_PlayChannel(-1, g_sound_main_P2[1], 0);
				P2_Player.SetRect(p_player.GetRect().x, p_player.GetRect().y - 50);
			}
			else if (P2_Player.Get_Input_type().bullet_Skill_I == 1) {
				// Am thanh I
				Mix_PlayChannel(-1, g_sound_main_P2[2], 0);
			}

		}
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
					ret_col = false; P2_Player.Remove_Bullet(im); if (Status_huongdan == 3) Status_huongdan = 10;
				}
				if (ret_col) {
					if (p_amo->GetRect().x > p_player.GetRect().x) p_player.set_status(0);
					else p_player.set_status(1);
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
		if (P2_Player.Get_Input_type().defend == 1 || p_player.Get_Move_u() == false) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Set_Move_U(false);
			P2_Player.Setinput_hurt(1);

		}
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_Skill_U(0).GetRect(), P2_Rect);
		if (p_player.Get_Input_type().bullet_Skill_U == 0) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Set_Move_U(false);
			P2_Player.Setinput_hurt(1);

		}
		// Su ly va cham giua Skill I p1 vs Main P2
		ret_col = SDLCommonFunc::CheckCollision(p_player.Get_Bullet_BigSize().GetRect(), P2_Rect);
		if (P2_Player.Get_Input_type().defend == 1) ret_col = false;
		if (p_player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x > P2_Player.GetRect().x) P2_Player.set_status(0);
			else P2_Player.set_status(1);
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
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Setinput_hurt(1);
		}

		// Su ly va cham Giua SKill I p2 vs Main 1
		if (P2_Rect.x <= 1) P2_Rect.x = 5;
		else if (P2_Rect.x >= SCREEN_WIDTH - 65) {
			P2_Rect.x = SCREEN_WIDTH - 66;
		}
		ret_col = SDLCommonFunc::CheckCollision(P2_Rect, P1_Rect);
		if (P2_Player.Get_Input_type().bullet_Skill_I == 0) ret_col = false;
		if (p_player.Get_Input_type().defend == 1) ret_col = false;
		if (ret_col == true) {
			if (p_player.GetRect().x < P2_Player.GetRect().x) p_player.set_status(0);
			else p_player.set_status(1);
			Mix_PlayChannel(-1, g_nhacnen[0], 0);
			p_player.Setinput_hurt(1);

		}

		//--------------------------------------------------------------------------------------
		SDL_RenderPresent(g_screen);
		// huy cac con tro tranh tran bo nho --------------------------
		p_player.Free();
		P2_Player.Free();


		int real_imp_time = fps_time.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;
		if (real_imp_time < time_one_frame) {
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}
	}
	return 1;
}
int main(int argc, char* argv[]) {
	if (InitData() == false) return -1;
	MenuObject gover_menu;
	BaseObject g_over;
	g_over.LoadImag("img//gameover.png", g_screen);
	int is_continue = 0;
	std::string DS[2] = { "CONTINUE","EXIT" };
	int x_over[] = { SCREEN_WIDTH / 2 - 50,SCREEN_WIDTH / 2 - 50 };
	int y_over[] = { 50,SCREEN_HEIGHT - 100 };
	menu_screen.LoadImag("img//Menu.png", g_screen);

	do {
		// Show menu game------------------------------------------
		std::string p_chu[] = { "Play Game","Exit","Help" };
		int x_menu[] = { 550,510,470 };
		int y_menu[] = { 200,300,400 };
		int kt = -2;
		do {
			if (kt == 2) {
				int k = -1;
				BaseObject huong_dan[2];
				huong_dan[0].LoadImag("img/Bk_huongdan.png", g_screen);
				huong_dan[1].LoadImag("img/huongdan.png", g_screen);
				MenuObject p_huongdan;
				std::string p_Chu[2] = { "EXIT","Guide Play"};
				int x_m[2] = { 1000,450 };
				int y_m[2] = { 30 ,450 };
				huong_dan[0].Render(g_screen);
				huong_dan[1].Render(g_screen);
				SDL_RenderPresent(g_screen);
				k = p_huongdan.ShowMenu(g_screen, g_font_text_1, p_Chu, 2, x_m, y_m, g_nhacnen[1]);
				if (k == 1) {
					k = Play_Huongdan();
				}
				huong_dan[0].Free();
				huong_dan[1].Free();
				if (k == 1 ) {
					close();
					return 0;
				}
			}
			menu_screen.Render(g_screen);
			kt = p_menu.ShowMenu(g_screen, g_font_text_1, p_chu, 3, x_menu, y_menu, g_nhacnen[1]);
		} while (kt == -2 || kt == 2);
		if (kt == -1||kt==1) {
			return 0;
		}
		//menu_screen.Free();
		// Show Lua Chon Phan choi ------------------------------------------
		kt = -2;
		std::string p_luachon[] = { "VS Human","VS Monster","VS Com" };
		int  x_ch[] = { 550,510,490 };
		int y_ch[] = { 200,300,400 };
		do {
			menu_screen.Render(g_screen);
			kt = p_menu.ShowMenu(g_screen, g_font_text_1, p_luachon, 3, x_ch, y_ch, g_nhacnen[1]);
		} while (kt < -1);
		p_menu.Free();
		if (kt == -1) break;
		if (kt == 0 || kt == 2) {
			if (Play(kt) == 1) break;
		}
		else if (Play_Threat() == 1) break;
		g_over.Render(g_screen);
		is_continue = gover_menu.ShowMenu(g_screen, g_font_text_1, DS, 2, x_over, y_over, g_nhacnen[1]);
		SDL_RenderPresent(g_screen);
	} while (is_continue == 0);

	close();
	return 0;
}
