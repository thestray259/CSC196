#pragma once
#include "Base/Actor.h"

class Projectile : public nc::Actor
{
public:
	Projectile(const nc::Transform transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed } {}

	void Update(float dt);

private:
	float lifetime{ 1 }; 
	float speed = 300;
};
