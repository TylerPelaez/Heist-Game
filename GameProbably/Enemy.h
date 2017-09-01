#pragma once
#include "Entity.h"
class Enemy : public Entity
{
public:
	std::vector<int> walkpath;

	int currentwalknode = -1;
	float walktox;
	float walktoy;

	bool walking;

	Enemy(PolygonMap* _map);

	void move() override;

};

