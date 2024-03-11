
#include"MENU.h"

int MenuObject::ShowMenu(SDL_Renderer* des, TTF_Font* font,const std::string DANH_SACH[],const int So_luong,const int x[],const int y[]) {
	SDL_Color Color[2] = { {255,137,29},{255,0,0} };
	int x_m; int y_m;
	int p_vitrix[10];
	int p_vitriy[10];
	for (int i = 0; i < So_luong; i++) {
		p_vitrix[i] = x[i];
		p_vitriy[i] = y[i];
	}
	for (int i = 0; i < So_luong; i++) {
		textColor[i] = Color[0];

		textSurface[i] = TTF_RenderText_Solid(font, DANH_SACH[i].c_str(), textColor[i]);
		textTexture[i] = SDL_CreateTextureFromSurface(des, textSurface[i]);
		dstRect[i].x = p_vitrix[i]; dstRect[i].y = p_vitriy[i];
		dstRect[i].w = textSurface[i]->w; dstRect[i].h = textSurface[i]->h;
	}
	SDL_Event m_even;
	while (true) {
		for (int i = 0; i < So_luong; i++) {
			textSurface[i] = TTF_RenderText_Solid(font, DANH_SACH[i].c_str(), textColor[i]);
			textTexture[i] = SDL_CreateTextureFromSurface(des, textSurface[i]);
			SDL_RenderCopy(des, textTexture[i], NULL, &dstRect[i]);
		}
		while (SDL_PollEvent(&m_even)) {
			switch (m_even.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEMOTION:
				x_m = m_even.motion.x;
				y_m = m_even.motion.y;
				for (int i = 0; i < So_luong; i++) {
					if (SDLCommonFunc::Checkvitri(x_m, y_m, dstRect[i])) {
						textColor[i] = Color[1];
					}
					else {
						textColor[i] = Color[0];
					}
				}

				break;
			case SDL_MOUSEBUTTONDOWN:
				x_m = m_even.motion.x;
				y_m = m_even.motion.y;
				for (int i = 0; i < So_luong; i++) {
					if (SDLCommonFunc::Checkvitri(x_m, y_m, dstRect[i])) {
						for (int j = 0; j < So_luong; j++) {
							SDL_DestroyTexture(textTexture[j]);
							textTexture[j] = NULL;
							SDL_FreeSurface(textSurface[j]);
							textSurface[j] = NULL;
						}
						return i;
					}
				}
				break;
			default:
				break;
			}
			SDL_RenderPresent(des);
		}
	}
	return 1;
}