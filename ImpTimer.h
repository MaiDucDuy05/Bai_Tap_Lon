#ifndef  IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	void stop();
	void paused();
	void unpaused();

	int get_ticks();
	bool Is_Started();
	bool Is_Paused();

	private:

	int start_tick;
	int paused_tick;
	bool is_started;
	bool is_paused;

};

#endif