#pragma once

#include <array>
#include <string>
#include <stdexcept>
#include <iterator>
#include <SFML/Graphics.hpp>
#include "EnclosedWorld.h"

struct InitParams
{
	sf::VideoMode videoMode;
	sf::ContextSettings settings;

	//Defaults!
	InitParams()
		:videoMode(1000, 800)
	{
		settings.antialiasingLevel = 4;
	}
};

struct SimParams
{
	unsigned frame_rate;
	double inv_frame_rate;
	double factor;
	unsigned velocityIterations;
	unsigned positionIterations;

	SimParams()
		:frame_rate(60)
		,inv_frame_rate(1.0f/frame_rate)
		,factor(10)
		,velocityIterations(6)
		,positionIterations(2)
	{}
};

struct Quit: public std::exception
{};

struct Restart: public std::exception
{};

//Empty transform. Returns p. Used to convert old versions of draw to new version for backwards compat
sf::Vector2f noTransform(const sf::Vector2f& p);

//Support function for drawBody. Converts b2Vec2 to sf::Vector2f.
sf::Vector2f convertVector(b2Vec2 vec, const b2Transform& xf, const unsigned factor);

class Simulation
{
private:
	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<EnclosedWorld> m_world;
	sf::Color m_backgroundColor;
	std::array<b2Body*, 4> m_edges;
	SimParams m_simParams;
	float m_progress;
	float m_maxprogress;
	bool m_loading;
	bool m_paused;

/////////////////////////////////////////////////////////////////////
// Internal helper methods
/////////////////////////////////////////////////////////////////////
private:
	void initialize_systems(const InitParams& params);
	void updateProgress();

/////////////////////////////////////////////////////////////////////
// Virtuals
/////////////////////////////////////////////////////////////////////
private:
	//Prepare. Initializes Graphics and Physics systems
	virtual InitParams prepare()
	{
		return InitParams();
	}

	//Title. Used by the default prepare implementation.
	virtual std::string title() =0;

	//Initialize. Called before the main loop.
	virtual void initialize() {}

	//Think. Called once per frame.
	virtual void think(unsigned long frame) {}

	//Render. Called between clear and display.
	virtual void render() {}

	//Handle an event. Called zero or more times before the think each frame. Default handles quit
	virtual void handle_event(sf::Event& event) {default_event(event);}

/////////////////////////////////////////////////////////////////////
// Public Interface
/////////////////////////////////////////////////////////////////////
public:
	Simulation();
	virtual ~Simulation() {}

	void execute();

	sf::Vector2u windowDimensions() const;
	sf::Vector2f windowHalfDimensions() const;
	b2Vec2 physicsDimensions() const;
	b2Vec2 physicsHalfDimensions() const;

/////////////////////////////////////////////////////////////////////
// Interface to simulations
/////////////////////////////////////////////////////////////////////
protected:
	void setTopEdge(bool setting);
	void setBottomEdge(bool setting);
	void setLeftEdge(bool setting);
	void setRightEdge(bool setting);

	void setAllEdges(bool setting);

	template<class Shape>
	void drawShape(
		Shape&& shape,
		const sf::Color& color=sf::Color::Black,
		const sf::Color& outline=sf::Color::Transparent,
		const float thickness=2)
	{
		shape.setFillColor(color);
		shape.setOutlineColor(outline);
		shape.setOutlineThickness(thickness);
		window().draw(shape);
	}

	//New PointTransform templates. Old versions retained for backwards compatibility.
	template<class Transform>
	void drawBody(b2Body& body, Transform&& pointTransform,
		const sf::Color& color = sf::Color::Black,
		const sf::Color& outline = sf::Color::Transparent,
		const float thickness=2)
	{
		const b2Transform& xf(body.GetTransform());
		for(b2Fixture* fixture = body.GetFixtureList(); fixture; fixture = fixture->GetNext())
		{
			switch(fixture->GetType())
			{
			case b2Shape::e_polygon:
				{
					auto shape = static_cast<b2PolygonShape*>(fixture->GetShape());
					auto vertexCount = shape->m_vertexCount;
					sf::ConvexShape object(vertexCount);
					for(int i = 0; i < vertexCount; ++i)
					{
						object.setPoint(i, pointTransform(convertVector(shape->m_vertices[i], xf, m_simParams.factor)));
					}
					drawShape(object, color, outline, thickness);
				}
				break;
				//TODO: add other types here
			}
		}
	}

	template<class Container, class Transform>
	void drawBodies(const Container& container, Transform&& pointTransform,
		const sf::Color& color = sf::Color::Black,
		const sf::Color& outline = sf::Color::Transparent,
		const float thickness=2)
	{
		auto end = std::end(container);
		for(auto it = std::begin(container); it != end; ++it)
			drawBody(**it, pointTransform, color, outline, thickness);
	}

	template<class Transform>
	void drawWorld(b2World& world, Transform&& pointTransform,
		const sf::Color& color = sf::Color::Black,
		const sf::Color& outline = sf::Color::Transparent,
		const float thickness = 2)
	{
		for(auto body = world.GetBodyList(); body; body=body->GetNext())
			drawBody(*body, pointTransform, color, outline, thickness);
	}

	//Old version retained for backwards compatibility
	void drawBody(b2Body& body,
		const sf::Color& color=sf::Color::Black,
		const sf::Color& outline=sf::Color::Transparent,
		const float thickness=2)
	{
		drawBody(body, &noTransform, color, outline, thickness);
	}

	template<class Container>
	void drawBodies(const Container& container,
		const sf::Color& color=sf::Color::Black,
		const sf::Color& outline=sf::Color::Transparent,
		const float thickness=2)
	{
		drawBodies(container, &noTransform, color, outline, thickness);
	}

	//Update progress bar. Call in initialize()
	void setProgress(float p);
	void addProgress(float p);
	void maxProgress(float p);

	//Default behavior. Quit, save, reset, etc.
	void default_event(sf::Event& event);

	//Pause or unpause
	void pause();
	
	//void drawBody(const b2Body& body, const sf::Color& fill, const sf::Color& line = sf::Color::Transparent);
	sf::RenderWindow& window() {return *m_window;}
	EnclosedWorld& world() {return *m_world;}
	sf::Color& background() {return m_backgroundColor;}
	const SimParams& simParams() {return m_simParams;}
};

#define REGISTER_SIMULATION(BASE, NAME) REGISTER_TYPE(Simulation, BASE, NAME)