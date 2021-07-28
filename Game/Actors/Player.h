#pragma once
#include "Base/Actor.h"

class Player : public nc::Actor
{
public:
	Player(const nc::Transform transform, std::shared_ptr<nc::Shape> shape, float speed); 

	void Update(float dt);
	void OnCollision(Actor* actor) override;

private:
	float fireTimer{ 0 }; 
	float fireRate{ 0.2f }; 
	float speed = 300; 
};