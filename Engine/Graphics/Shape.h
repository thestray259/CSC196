#pragma once
#include "Engine.h"
#include "Framework/ResourceSystem.h"

namespace nc
{
	class Shape : public Resource
	{
	public:
		Shape() {}
		Shape(const std::vector<Vector2>& points, const Color& color) : points{ points }, color { color } 
		{
			ComputeRadius();
		}

		void Draw(Core::Graphics& graphics, const Vector2& position, float angle, float scale = 1); 
		void Draw(Core::Graphics& graphics, const Transform& transform); 

		bool Load(const std::string& filename); 

	private: 
		void ComputeRadius(); 

	public:
		std::vector<Vector2> points; 
		Color color; 
		float radius{ 0 };
	};
}