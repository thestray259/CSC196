#include "Enemy.h"
#include "Player.h"
#include "Engine.h"
#include "Projectile.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>())
	{
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);

		transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
		transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

		float turnAngle = nc::Vector2::SignedAngle(forward, direction.Normalized());
		float angle = nc::Vector2::Angle(forward, direction.Normalized());

		transform.rotation = transform.rotation + turnAngle * (3 * dt);

		//transform.rotation = nc::Lerp(transform.rotation, transform.rotation + turnAngle, 2 * dt); 

		fireTimer -= dt;
		if (fireTimer <= 0 && angle <= nc::DegToRad(10)) 
		{
			fireTimer = fireRate;
			std::vector<nc::Vector2> points = { {-5, -5}, {5, -5}, {0, 10}, {-5, -5} };
			std::shared_ptr<nc::Shape> proj = std::make_shared<nc::Shape>();
			proj->Load("enemyproj.txt");

			nc::Transform t = transform;
			t.scale = 0.5f;
			scene->AddActor(std::make_unique<Projectile>(t, proj, 200.0f));

			scene->engine->Get<nc::AudioSystem>()->PlayAudio("enemyshoot");
		}
	}
}

void Enemy::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player")
	{
		destroy = true; 
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::red, 50);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event; 
		event.name = "AddPoints"; 
		event.data = 300; 
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}
