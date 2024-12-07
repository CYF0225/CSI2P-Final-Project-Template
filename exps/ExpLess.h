#ifndef EXPLESS_H_INCLUDED
#define EXPLESS_H_INCLUDED

#include "Exp.h"
#include "../shapes/Point.h"

// fixed settings: TowerArcane attributes
class ExpLess : public Exp
{
public:
	ExpLess(const Point &p) : Exp(p, 30, ExpType::LESS) {}
};

#endif
