#include "Game.h"
#include "Base/Actor.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Projectile.h"

void Game::Initialize()
{
	engine = std::make_unique<nc::Engine>(); // new Engine 
	engine->Startup(); 
	scene = std::make_unique<nc::Scene>(); // new Scene 
	scene->engine = engine.get(); 
	
	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "explosion.wav"); 
	engine->Get<nc::AudioSystem>()->AddAudio("playershoot", "playershoot.wav"); 
	engine->Get<nc::AudioSystem>()->AddAudio("enemyshoot", "enemyshoot.wav"); 

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1)); 
	//engine->Get<nc::EventSystem>()->Subscribe("PlayerHurt", std::bind(&Game::OnPlayerHurt, this, std::placeholders::_1)); 
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1)); 
}

void Game::Shutdown()
{
	scene->RemoveAllActors(); 
	engine->Shutdown(); 
}

void Game::Update(float dt)
{
	stateTimer += dt; 

	switch (state)
	{
	case Game::eState::Title:
		if (Core::Input::IsPressed(VK_SPACE))
		{
			state = eState::StartGame; 
		}
		break;
	case Game::eState::StartGame:
		score = 0; 
		lives = 1; 
		state = eState::StartLevel; 
		break;
	case Game::eState::StartLevel:
		UpdateLevelStart(dt); 
		break;
	case Game::eState::LevelOne:
		if (scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::LevelTwo;
		}
		break; 
	case Game::eState::LevelTwo:
		for (size_t i = 0; i < 4; i++)
		{
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy.txt"), 100.0f));
		}

		if (scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::LevelThree;
		}
		break;
	case Game::eState::LevelThree:
		for (size_t i = 0; i < 6; i++)
		{
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy.txt"), 100.0f));
		}

		if (scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::GameWin;
		}
		break;
	case Game::eState::Game:
		if (scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::GameWin; 
		}
		break;
	case Game::eState::GameWin:
		break; 
	case Game::eState::GameLose:
		if (scene->GetActors<Enemy>().size() > 0)
		{
			scene->RemoveAllActors(); 
		}
		break;
	default:
		break;
	}
	
	engine->Update(dt); 
	scene->Update(dt); 
}

void Game::Draw(Core::Graphics& graphics)
{
	switch (state)
	{
	case Game::eState::Title:
		graphics.SetColor(nc::Color::green); 
		graphics.DrawString(370, 300 + static_cast<int>((std::sin(stateTimer * 3.0f)) * 5.0f), "VECTREX 9999"); 

		graphics.SetColor(nc::Color::cyan);
		graphics.DrawString(340, 360, "Press Space To Start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::LevelOne:
		break;
	case Game::eState::LevelTwo:
		break;
	case Game::eState::LevelThree:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameWin:
		graphics.SetColor(nc::Color::blue);
		graphics.DrawString(370, 380, "You Win!");
		break; 
	case Game::eState::GameLose:
		graphics.SetColor(nc::Color::red);
		graphics.DrawString(370, 380, "Game Over");
		break;
	default:
		break;
	}

	graphics.SetColor(nc::Color::white); 
	graphics.DrawString(30, 20, std::to_string(score).c_str()); 
	graphics.DrawString(750, 20, std::to_string(lives).c_str()); 

	scene->Draw(graphics); 
	engine->Draw(graphics); 
}

void Game::UpdateTitle(float dt)
{
	if (Core::Input::IsPressed(VK_SPACE))
	{
		stateFunction = &Game::UpdateLevelStart;
		//state = eState::StartGame;
	}
}

void Game::UpdateLevelStart(float dt)
{
	scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0, 3 }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("player.txt"), 300.0f));
	
	for (size_t i = 0; i < 2; i++)
	{
		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy.txt"), 100.0f));
	}

	state = eState::LevelOne;
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data); 
}

//void Game::OnPlayerHurt(const nc::Event& event)
//{
//	lives--; 
//
//	if (lives == 0)
//	{
//		state = eState::GameOver; 
//	}
//}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives = 0;
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameLose;
}
