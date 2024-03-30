#include"ImpTimer.h"
#include"Common_Function.h"
ImpTimer::ImpTimer(){
	start_tick=0;
	is_started = true;
}
ImpTimer::~ ImpTimer(){

}
void ImpTimer::start(){
	is_started=true;
	start_tick= SDL_GetTicks();
}
void  ImpTimer::stop(){
	is_started=false;
}

int ImpTimer::get_ticks(){
	if(is_started==true){
			return SDL_GetTicks()-start_tick;
	}
	return 0;
}
bool ImpTimer::Is_Started(){
	return is_started;
}
