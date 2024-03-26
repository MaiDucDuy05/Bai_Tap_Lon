#ifndef	COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include<Windows.h>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<SDL_rect.h>



static  SDL_Window * g_window =NULL;
static SDL_Renderer * g_screen =NULL;
static  SDL_Event g_event;
static SDL_Event g_event_1;


static Mix_Chunk * g_sound_main_P1[10];
static Mix_Chunk * g_sound_main_P2[10];
static Mix_Chunk * g_nhacnen[2];

static TTF_Font* g_font_text = NULL;
static TTF_Font* g_font_text_1 = NULL;
static TTF_Font* g_font_text_2 = NULL;

//screen
const int FRAME_PER_SECOND = 25; // fbs
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B= 180;

#define TILE_SIZE 64
#define MAX_MAP_X 20
#define MAX_MAP_Y 10
#define BLANK_TILE 0

#define Num_Threat 4
typedef struct Input{
	int left;
	int right;
	int up;
	int down;
	int jump;
	int hurt;
	int bullet_Skill_I;// SKILL I
	int defend;
	int speed_up;
	int bullet_Skill_U ;// SKILL U
	int bullet_Skill_J; //SKILL j
	int empty;
};

typedef struct Map
{
	
	
	int max_x;
	int max_y;

	int tile[MAX_MAP_Y][MAX_MAP_X];

	char *file_name;
};
namespace SDLCommonFunc{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2) ;
	bool Checkvitri(const  int& x, const int& y, const  SDL_Rect&);
}

#endif