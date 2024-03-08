#ifndef BULLET_OBJECT_H_
#define  BULLET_OBJECT_H_

#include"Common_Function.h"
#include"BaseObject.h"

class BulletObject : public BaseObject{
public:
	BulletObject();
	~BulletObject();
	enum BulletDir{
		DIR_RIGHT = 20,
		DIR_LEFT = 21,
	};

	void set_x_val(const int & xVal) {x_val= xVal;}
	void set_y_val(const int & yVal) {y_val= yVal;}

	int get_x_val() const{return x_val;}
	int get_y_val() const{return y_val;}

	void set_is_move(const bool & isMove){is_move = isMove;}
	bool get_is_move() const { return is_move;}

	void HandleMove(const int &x_border , const int &y_border);
	void set_bullet_dir(const unsigned int & bulletdir){
		bullet_dir=bulletdir;
	}
	int get_bullet_dir() const { return bullet_dir;}

private:

	int x_val;
	int y_val;
	bool is_move;
	unsigned int bullet_dir;

};
#endif