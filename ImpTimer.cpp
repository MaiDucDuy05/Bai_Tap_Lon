#include"ImpTimer.h"
#include"Common_Function.h"
ImpTimer::ImpTimer(){
	start_tick=0;
	paused_tick=0;
	is_paused=false;
	is_started = false;
}
ImpTimer::~ ImpTimer(){

}
void ImpTimer::start(){
	is_started=true;
	is_paused=false;
	start_tick= SDL_GetTicks();
}
void  ImpTimer::stop(){
	is_paused=false;
	is_started=false;
}
void ImpTimer:: paused(){
	if(is_started==true && is_paused==false){
		is_paused=true;
		paused_tick=SDL_GetTicks()-paused_tick;
	}
}
void ImpTimer::unpaused(){
	if(is_paused==true){
		is_paused=false;
		start_tick=SDL_GetTicks()-paused_tick;
		paused_tick=0;
	}
}

int ImpTimer::get_ticks(){
	if(is_started==true){
		if(is_paused==true){
			return paused_tick;
		}
		else{
			return SDL_GetTicks()-start_tick;
		}
	}
	return 0;
}
bool ImpTimer::Is_Started(){
	return is_started;
}
bool ImpTimer::Is_Paused(){
	return is_paused;
}