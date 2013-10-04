#pragma once

#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>

class SFMLDraw
{
private:
	sf::RenderWindow* window;

public:
	SFMLDraw(sf::RenderWindow& window, const sf::Color& color, const sf::Color& outline);
	
	sf::ConvexShape make_polygon(const b2Vec2* vertecies, int32 vertexCount);
	sf::CircleShape make_circle(const b2Vec2& center, float32 radius);
	sf::ConvexShape make_segment(const b2Vec2& p1, const b2Vec2& p2);

	void draw_shape(sf::Shape&& object, const sf::Color& color, const sf::Color& );

	void DrawPolygon(const b2Vec
};
