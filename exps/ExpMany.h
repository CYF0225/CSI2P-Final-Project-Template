#ifndef EXPMANY_H_INCLUDED
#define EXPMANY_H_INCLUDED

#include "Exp.h"
#include "../shapes/Point.h"

// fixed settings: TowerArcane attributes
class ExpMany : public Exp
{
public:
	ExpMany(const Point &p) : Exp(p, 90, ExpType::MANY) {}
};

#endif
