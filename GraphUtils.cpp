#include "GraphUtils.h"
#include "VectorUtils.h"
#include <algorithm>
#include <array>

float GraphUtils::CCW(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool GraphUtils::intersect(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d)
{
	return (CCW(a, b, c) * CCW(a, b, d) < 0 && CCW(c, d, b) * CCW(c, d, a) < 0);
}

// Checks if two lines with a width (read: rectangles) intersect using separating axis theorem.
bool GraphUtils::intersectWithWidth(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f d, float width)
{
	if (width <= 0.f)
	{
		return intersect(a, b, c, d);
	}

	// TEST BEGIN

	static const float offset = 40.f;
	auto bToA = b - a;
	auto cToD = d - c;
	a = a + VectorUtils::normalize(bToA) * offset;
	b = b - VectorUtils::normalize(bToA) * offset;
	c = c + VectorUtils::normalize(cToD) * offset;
	d = d - VectorUtils::normalize(cToD) * offset;
	

	// TEST END


	float halfWidth = width / 2.f;
	std::array<std::array<sf::Vector2f, 4>, 2> rects;
	sf::Vector2f line1Dir = b - a;
	sf::Vector2f line1Perpendicular = sf::Vector2f(-line1Dir.y, line1Dir.x);
	sf::Vector2f line1PerpendicularNorm = VectorUtils::normalize(line1Perpendicular);
	rects[0] = { a - line1PerpendicularNorm * halfWidth, a + line1PerpendicularNorm * halfWidth,
							b + line1PerpendicularNorm * halfWidth, b - line1PerpendicularNorm * halfWidth };

	sf::Vector2f line2Dir = d - c;
	sf::Vector2f line2Perpendicular = sf::Vector2f(-line2Dir.y, line2Dir.x);
	sf::Vector2f line2PerpendicularNorm = VectorUtils::normalize(line2Perpendicular);
	rects[1] = { c - line2PerpendicularNorm * halfWidth, c + line2PerpendicularNorm * halfWidth,
							d + line2PerpendicularNorm * halfWidth, d - line2PerpendicularNorm * halfWidth };

	for (const auto& rect : rects)
	{
		for (int i = 0; i < 4; ++i)
		{
			int i2 = (i + 1) % 4;
			sf::Vector2f face = rect[i2] - rect[i];
			sf::Vector2f facePerpendicularNorm = VectorUtils::normalize(sf::Vector2f(-face.y, face.x));
		
			std::vector<float> rect1Projections;
			for (auto& p : rects[0])
			{
				rect1Projections.push_back(VectorUtils::dot(facePerpendicularNorm, p));
			}
			std::vector<float> rect2Projections;
			for (auto& p : rects[1])
			{
				rect2Projections.push_back(VectorUtils::dot(facePerpendicularNorm, p));
			}

			float min1 = *std::min_element(rect1Projections.begin(), rect1Projections.end());
			float max1 = *std::max_element(rect1Projections.begin(), rect1Projections.end());

			float min2 = *std::min_element(rect2Projections.begin(), rect2Projections.end());
			float max2 = *std::max_element(rect2Projections.begin(), rect2Projections.end());

			if (!(max1 < min2 || max2 < min1))
			{
				return true; // intersection
			}
		}
	}

	return false;
}
