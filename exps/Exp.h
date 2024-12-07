#ifndef EXP_H_INCLUDED
#define EXP_H_INCLUDED

#include "../Object.h"
#include "../shapes/Rectangle.h"
#include <allegro5/bitmap.h>
#include <string>
#include <map>
#include <array>


// fixed settings
enum class ExpType {
	LESS,MEDIUM,MANY,EXPTYPE_MAX
};


class Exp : public Object
{
public:
	/**
	 * @brief Get the ALLEGRO_BITMAP* instance of the full image of a specific TowerType.
	 */
	static ALLEGRO_BITMAP *get_bitmap(ExpType type);
	/**
	 * @brief Create a Tower* instance by the type.
	 * @param type the type of a tower.
	 * @param p center point of the tower.
	 */
	static Exp *create_exp(ExpType type, const Point &p);
public:
	Exp(const Point &p, int gain, ExpType type);
	virtual ~Exp() {}
	void update();
	//virtual bool attack(Object *target);
	void draw();
	//void drawexp();
	int getgain() const {return gain;}
	//Rectangle get_region() const;
	//virtual Bullet *create_bullet(Object *target) = 0;
	//virtual const double attack_range() const = 0;
	ExpType type=ExpType::LESS;
private:
	/**
	 * @var attack_freq
	 * @brief Tower attack frequency. This variable will be set by its child classes.
	 **
	 * @var counter
	 * @brief Tower attack cooldown.
	 */
	int gain;
	//ALLEGRO_BITMAP *bitmap;
	std::map<ExpType,std::string> gifPath;
};

#endif
