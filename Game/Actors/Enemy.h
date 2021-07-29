#pragma once
#include "Base/Actor.h"

class Enemy : public nc::Actor
{
public:
	Enemy(const nc::Transform transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed } {}

	void Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* actor) override; 

private:
	float fireTimer{ 0 };
	float fireRate{ 1.5f };
	float speed = 300;
};