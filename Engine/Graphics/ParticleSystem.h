#pragma once
#include "Base/System.h"
#include"Math/Vector2.h"
#include"Math/Color.h"
#include <vector>

namespace nc
{
	class ParticleSystem : public GraphicsSystem
	{
	public:
		struct Particle
		{
			Vector2 position; 
			Vector2 prevPosition; 
			Vector2 velocity; 
			Color color; 
			float lifetime; 
			bool isActive{ false };

			static bool IsNotActive(Particle particle) { return particle.isActive == false; }
		};
		void Startup() override; 
		void Shutdown() override; 

		void Update(float dt) override;
		void Draw(Core::Graphics& graphics) override; 

		void Create(const Vector2& position, size_t count, float lifetime, const Color& color, float speed); 

	private:
		std::vector<Particle> particles; 
	};
}