#ifndef EXPMEDIUM_H_INCLUDED
#define EXPMEDIUM_H_INCLUDED

#include "Exp.h"
#include "../shapes/Point.h"

// fixed settings: TowerArcane attributes
class ExpMedium : public Exp
{
public:
	ExpMedium(const Point &p) : Exp(p, 50, ExpType::MEDIUM) {}
};

#endif
