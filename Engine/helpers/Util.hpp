#pragma once
#ifndef UTIL_HPP
#define UTIL_HPP

#include <algorithm>
#include <SFML/System.hpp>


inline int randClamp(int minValue, int maxValue)
{
	return (rand() % (maxValue - minValue)) + minValue;
}


inline float clamp(float value, float minValue, float maxValue)
{
	return std::min(std::max(value, minValue), maxValue);
}


inline float crossProduct(sf::Vector2f vector1, sf::Vector2f vector2)
{
	return (vector1.x * vector2.y) - (vector1.y * vector2.x);
}


inline bool collisionAABB(sf::Vector2f position1, sf::Vector2f size1, sf::Vector2f position2, sf::Vector2f size2)
{
	return (position1.x <= position2.x + size2.x &&
			position1.x + size1.x >= position2.x &&
			position1.y <= position2.y + size2.y &&
			position1.y + size1.y >= position2.y
		? true : false);
}


#endif // UTIL_HPP