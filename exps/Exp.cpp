#include "Exp.h"
#include "ExpLess.h"
#include "ExpMedium.h"
#include "ExpMany.h"
#include "../Utils.h"
#include "../shapes/Circle.h"
#include "../monsters/Monster.h"
#include "../shapes/Rectangle.h"
#include "../data/DataCenter.h"
#include "../data/GIFCenter.h"
#include "../data/SoundCenter.h"
#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_primitives.h>

// fixed settings
/*namespace TowerSetting {
	constexpr char attack_sound_path[] = "./assets/sound/Arrow.wav";
};*/

//al_init_primitives_addon();

namespace ExpSetting {
	static constexpr char gif_root_path[50] = "./assets/gif/Exp";
	static constexpr char gif_postfix[][10] = {
		"Less", "Medium", "Many"
	};
}

Exp*
Exp::create_exp(ExpType type, const Point &p) {
	switch(type) {
		case ExpType::LESS: {
			return new ExpLess(p);
		} case ExpType::MEDIUM: {
			return new ExpMedium(p);
		} case ExpType::MANY: {
			return new ExpMany(p);
		}
	}
	GAME_ASSERT(false, "exp type error.");
}

/**
 * @param p center point (x, y).
 * @param attack_range any monster inside this number would trigger attack.
 * @param attack_freq period for tower to attack.
 * @param type tower type.
*/
Exp::Exp(const Point &p, int gain, ExpType type):type(type){
	for(size_t type=0;type < static_cast<size_t>(ExpType::EXPTYPE_MAX);++type){
        char buffer[50];
	sprintf(
		buffer, "%s/exp_%s.gif",
		ExpSetting::gif_root_path,
        ExpSetting::gif_postfix[static_cast<int>(type)]);
        gifPath[static_cast<ExpType>(type)]=std::string{buffer};
    }
	this->gain=gain;
	shape.reset(new Point(p.x, p.y));
	/*ImageCenter *IC = ImageCenter::get_instance();
	// shape here is used to represent the tower's defending region. If any monster walks into this area (i.e. the bounding box of the monster and defending region of the tower has overlap), the tower should attack.
	//shape.reset(new Circle(p.x, p.y, attack_range));
	//counter = 0;
	//this->attack_freq = attack_freq;
	this->type = type;
	bitmap = IC->get(ExpSetting::exp_full_img_path[static_cast<int>(type)]);*/
}

/**
 * @brief Update attack cooldown and detect if the tower could make an attack.
 * @see Tower::attack(Object *target)
*/
/*Exp::update() {
	if(counter) counter--;
	else {
		DataCenter *DC = DataCenter::get_instance();
		for(Monster *monster : DC->monsters) {
			if(attack(monster)) break;
		}
	}
}*/

/**
 * @brief Check whether the tower can attack the target. If so, shoot a bullet to the target.
*/

void Exp::draw(){
	GIFCenter*GIFC=GIFCenter::get_instance();
    ALGIF_ANIMATION*gif=GIFC->get(gifPath[type]);
	//printf("test22\n");
    algif_draw_gif(
		gif,
		shape->center_x() ,
		shape->center_y(),0);
}



/**
 * @brief Get the area of the tower, and return with a Rectangle object.
*/
/*Rectangle
Tower::get_region() const {
	int w = al_get_bitmap_width(bitmap);
	int h = al_get_bitmap_height(bitmap);
	return {
		shape->center_x() - w/2,
		shape->center_y() - h/2,
		shape->center_x() - w/2 + w,
		shape->center_y() - h/2 + h
	};
}*/
