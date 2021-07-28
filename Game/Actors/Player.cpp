#include "Player.h"
#include "Enemy.h"
#include "Engine.h"
#include "Projectile.h"
#include "Framework/ResourceSystem.h"
#include <memory>

Player::Player(const nc::Transform transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed } 
{
	std::unique_ptr locator = std::make_unique<Actor>(); 
	locator->transform.localPosition = nc::Vector2{ -8, 0 }; 
	AddChild(std::move(locator)); 

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, 5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, -5 };
	AddChild(std::move(locator));
}


void Player::Update(float dt)
{
	Actor::Update(dt); 

	// movement
	float thrust = 0;
	if (Core::Input::IsPressed('A')) transform.rotation -= 5 * dt;
	if (Core::Input::IsPressed('D')) transform.rotation += 5 * dt;
	if (Core::Input::IsPressed('W')) thrust = speed;

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	// fire
	fireTimer -= dt; 
	if (fireTimer <=0 && Core::Input::IsPressed(VK_SPACE))
	{
		fireTimer = fireRate; 

		{
			std::vector<nc::Vector2> points = { {-5, -5}, {5, -5}, {0, 10}, {-5, -5} };

			nc::Transform t = children[1]->transform; 
			t.scale = 0.5f; 

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectile.txt"), 600.0f);
			projectile->tag = "Player"; 
			scene->AddActor(std::move(projectile));
		}

		{
			std::vector<nc::Vector2> points = { {-5, -5}, {5, -5}, {0, 10}, {-5, -5} };

			nc::Transform t = children[2]->transform;
			t.scale = 0.5f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectile.txt"), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("playershoot");
	}

	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 3, 2, nc::Color::white, 50); 
}

void Player::OnCollision(Actor* actor)
{
	if (dynamic_cast<Enemy*>(actor))
	{
		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::red, 50);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "PlayerDead";
		event.data = std::string("yes I'm dead"); 
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}
