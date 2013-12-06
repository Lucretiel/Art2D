#include "Huge.h"

#include <array>

class ChaosRect: public sf::RectangleShape
{
private:
	std::array<sf::Vector2f, 4> offsets;

public:
	float offset_range;
	ChaosRect(const sf::Vector2f& size, float offset):
		RectangleShape(size),
		offset_range(offset)
	{
		randomize();
	}

	void randomize()
	{
		for(sf::Vector2f& vec : offsets)
			vec = random_centered(sf::Vector2f(), offset_range);
		update();
	}

	sf::Vector2f getPoint(unsigned int index) const override
	{
		return RectangleShape::getPoint(index) + offsets[index];
	}
};

class Dissonance: public Huge
{
	std::string title() override {return "Dissonance";}
	std::vector<ChaosRect> rects;

	void initialize() override
	{
		const auto size = windowDimensions();
		linear_scale<float, float> window_scale(0, 1000, 0, size.x);
		linear_scale<float, float> chaos_scale(0, size.x, window_scale(20), 0);
		linear_scale<float, float> size_scale(0, size.x, 3, 1);
		range<float> chaos_clamp(0, window_scale(30));
		range<float> size_clamp(1, 3);
		const unsigned num_rects(300);
		ChaosRect rect(sf::Vector2f(window_scale(100), window_scale(20)), 0);
		rect.setFillColor(sf::Color::Black);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(window_scale(-1));

		sf::Vector2f top_left(window_scale(-100), window_scale(-100));
		sf::Vector2f bottom_right(size.x + window_scale(100), size.y + window_scale(100));
		maxProgress(num_rects);
		for(unsigned i = 0; i < num_rects; ++i)
		{
			auto location = random_in(top_left, bottom_right);
			auto chaos = chaos_clamp.clamp(chaos_scale(location.x));
			float scale = size_clamp.clamp(randuniform(1, size_scale(location.x)));
			rect.setScale(scale, scale);
			rect.setPosition(location);
			rect.offset_range = chaos * scale;
			rect.setRotation(randuniform(0, 360));
			rect.randomize();
			rects.push_back(rect);
			addProgress(1);
		}
	}

	void handle_event(sf::Event& event)
	{
		default_event(event);
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			for(ChaosRect& rect : rects)
				rect.randomize();
	}


	void render() override
	{
		for(const ChaosRect& rect : rects)
			window().draw(rect);
	}

};

REGISTER_SIMULATION(Dissonance, "dissonance");
