#include "Shape.h"
#include <fstream>

namespace nc
{
	void Shape::Draw(Core::Graphics& graphics, const Vector2& position, float angle, float scale)
	{
		graphics.SetColor(color); 

		for (size_t i = 0; i < points.size() - 1; i++)
		{
			nc::Vector2 p1 = position + (Vector2::Rotate(points[i], angle) * scale);// + Vector2{ rand() % 10 - 5, rand() % 10 - 5 };
			nc::Vector2 p2 = position + (Vector2::Rotate(points[i + 1], angle) * scale);// + Vector2{ rand() % 10 - 5, rand() % 10 - 5 };

			graphics.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
	}

	void Shape::Draw(Core::Graphics& graphics, const Transform& transform)
	{
		graphics.SetColor(color);

		for (size_t i = 0; i < points.size() - 1; i++)
		{
			nc::Vector2 p1 = transform.matrix * points[i]; 
			nc::Vector2 p2 = transform.matrix * points[i + 1]; 

			graphics.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
	}

	bool Shape::Load(const std::string& filename)
	{
		bool success = false; 

		std::cout << filename;
		std::ifstream stream(filename); 
		if (stream.is_open())
		{
			success = true; 

			// read color
			stream >> color; 

			// read number of points
			std::string line; 
			std::getline(stream, line); 
			size_t size = stoi(line); 

			// read points
			for (size_t i = 0; i < size; i++)
			{
				nc::Vector2 point; 
				stream >> point; 

				points.push_back(point); 
			}

			ComputeRadius(); 
		}

		return success; 
	}

	void Shape::ComputeRadius()
	{
		for (auto& point : points)
		{
			float r = point.Length(); 
			//radius = std::max(radius, r); 
			if (r > radius) radius = r; 
		}
	}
}

