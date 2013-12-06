#pragma once

#include <array>
#include <string>
#include <stdexcept>
#include <iterator>
#include <SFML/Graphics.hpp>
#include <CPPRegistry/CPPRegistry.h>
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
	sf::Shader* shader;

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

	bool is_paused() const {return m_paused;}

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
		window().draw(shape, shader);
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
			case b2Shape::e_edge:
				{
					auto shape = static_cast<b2EdgeShape*>(fixture->GetShape());
					std::array<sf::Vertex, 2> points;
					points[0].position = pointTransform(convertVector(shape->m_vertex1, xf, m_simParams.factor));
					points[1].position = pointTransform(convertVector(shape->m_vertex2, xf, m_simParams.factor));
					points[0].color = color;
					points[1].color = color;
					window().draw(points.data(), points.size(), sf::PrimitiveType::Lines, shader);
				}
				break;
			case b2Shape::e_circle:
				{
					b2CircleShape* shape = static_cast<b2CircleShape*>(fixture->GetShape());
					auto radius = shape->m_radius * m_simParams.factor;
					sf::CircleShape object(radius, 60);
					object.setOrigin(radius, radius);
					object.setPosition(pointTransform(convertVector(shape->m_p, xf, m_simParams.factor)));
					drawShape(object, color, outline, thickness);
				}
				break;
			}

		}
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

	template<class Container, class Transform>
	void drawBodies(const Container& container, Transform&& pointTransform,
		const sf::Color& color = sf::Color::Black,
		const sf::Color& outline = sf::Color::Transparent,
		const float thickness=2)
	{
		for(auto& body : container)
			drawBody(*body, pointTransform, color, outline, thickness);
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

	void useShader(sf::Shader* shader);
};

TYPE_REGISTRY(Simulations, Simulation);

#define REGISTER_SIMULATION(TYPE, NAME) REGISTER_TYPE(Simulations, TYPE, NAME)
#define SIM(TYPE, NAME) REGISTERED_TYPE(Simulations, TYPE, NAME): public Simulation