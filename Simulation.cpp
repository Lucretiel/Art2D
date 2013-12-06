#include "Simulation.h"

#include <vector>
#include <iostream>

void Simulation::initialize_systems(const InitParams& params)
{
	m_window.reset(new sf::RenderWindow(
		params.videoMode,
		title(),
		sf::Style::Titlebar | sf::Style::Close,
		params.settings));

	b2Vec2 physicsDimensions(
		float(params.videoMode.width) / simParams().factor,
		float(params.videoMode.height) / simParams().factor);

	m_world.reset(new EnclosedWorld(b2Vec2(0, 0), physicsDimensions));

	m_world->SetAllowSleeping(true);
}

Simulation::Simulation()
	:m_backgroundColor(sf::Color::White)
	,m_progress(0)
	,m_maxprogress(1)
	,m_paused(false)
	,shader(nullptr)
{}

void Simulation::setTopEdge(bool setting) {world().setTopEdge(setting);}
void Simulation::setBottomEdge(bool setting) {world().setBottomEdge(setting);}
void Simulation::setLeftEdge(bool setting) {world().setLeftEdge(setting);}
void Simulation::setRightEdge(bool setting) {world().setRightEdge(setting);}
void Simulation::setAllEdges(bool setting) {world().setAllEdges(setting);}

sf::Vector2f noTransform(const sf::Vector2f& p) {return p;}

sf::Vector2f convertVector(b2Vec2 vec, const b2Transform& xf, const unsigned factor)
{
	vec = b2Mul(xf, vec);
	return sf::Vector2f(vec.x * factor, vec.y * factor);
}

void Simulation::setProgress(float p)
{
	m_progress = p;
	updateProgress();
}

void Simulation::addProgress(float p)
{
	m_progress += p;
	updateProgress();
}

void Simulation::maxProgress(float p)
{
	m_maxprogress = p;
	updateProgress();
}

void Simulation::updateProgress()
{
	if(m_loading && m_window && m_maxprogress)
	{
		auto window_dimensions(windowDimensions());
		sf::RectangleShape rect;
		rect.setPosition(0, window_dimensions.y / 2);
		rect.setSize(sf::Vector2f(window_dimensions.x, 10));
		rect.setScale(m_progress/m_maxprogress, 1);
		rect.setFillColor(sf::Color::Black);

		m_window->clear(m_backgroundColor);
		m_window->draw(rect, shader);
		m_window->display();
	}
}

void Simulation::default_event(sf::Event& event)
{
	if(event.type == sf::Event::Closed)
		throw Quit();

	if(event.type == sf::Event::KeyPressed)
	{
		switch(event.key.code)
		{
		case sf::Keyboard::Q:
			throw Quit();
			break;
		case sf::Keyboard::R:
			throw Restart();
			break;
		case sf::Keyboard::S:
			{
				auto image = m_window->capture();
				image.saveToFile(title() + ".png");
			}
			break;
		case sf::Keyboard::P:
			pause();
			break;
		}

	}
}

void Simulation::pause()
{
	m_paused = !m_paused;
	if(m_paused)
		window().setTitle(title() + " (paused)");
	else
		window().setTitle(title());
}

sf::Vector2u Simulation::windowDimensions() const
{
	return m_window->getSize();
}

sf::Vector2f Simulation::windowHalfDimensions() const
{
	auto dim = windowDimensions();
	return sf::Vector2f(dim.x / 2, dim.y / 2);
}

b2Vec2 Simulation::physicsDimensions() const
{
	return m_world->dimensions();
}

b2Vec2 Simulation::physicsHalfDimensions() const
{
	auto dim = physicsDimensions();
	return b2Vec2(dim.x / 2, dim.y / 2);
}

void Simulation::useShader(sf::Shader* shader)
{
	this->shader = shader;
}


void Simulation::execute()
{
	m_loading = true;
	initialize_systems(prepare());
	initialize();
	m_loading = false;
	m_window->setFramerateLimit(m_simParams.frame_rate);
	
	sf::Event event;
	unsigned long frame = 0;
	try
	{
		while(true)
		{
			while(m_window->pollEvent(event))
			{
				handle_event(event);
			}

			if(!m_paused)
			{
				world().Step(
					m_simParams.inv_frame_rate,
					m_simParams.velocityIterations,
					m_simParams.positionIterations);
				think(frame++);
			}
			m_window->clear(m_backgroundColor);
			render();
			m_window->display();
		}
	}
	catch(const Quit&)
	{}
}