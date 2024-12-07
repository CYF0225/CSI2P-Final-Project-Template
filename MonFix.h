#ifndef MONFIX_H_INCLUDED
#define MONFIX_H_INCLUDED
#include "Object.h"
#include<map>
#include<string>

enum class MonFixState {
	APPEAR,
	STAND,
	ATTACK,
	DIE,
	MONFIXSTATE_MAX
};

class MonFix : public Object{
    public:
    void init(int x,int y);
	void update();
	void draw();
	static MonFix* create_monfix(int x,int y);
	MonFix(int x,int y,MonFixState state);
	MonFixState get_state() const {return state;}
    void set_state(MonFixState new_state) {state=new_state;}
	bool monfix_erase;
	int get_width();
	int get_height();

    private:
    MonFixState state;
    //double speed=5;
    std::map<MonFixState,std::string> gifPath;

};

#endif