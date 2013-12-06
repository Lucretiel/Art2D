#include "Simulation.h"
#include "helpers.h"
#include "rand.h"
#include "color_helpers.h"

struct colored_body
{
	b2Body* body;
	sf::Color color;
	float distance;

	colored_body(b2Body* body, const sf::Color& color, float distance):
		body(body),
		color(color),
		distance(distance)
	{}
};

const static double pi = 4 * std::atan(static_cast<double>(1));

SIM(ChangingGreys, "changing-greys")
{
	std::string title() override {return "Changing Greys";}

	std::vector<colored_body> bodies;

	void initialize()
	{
		auto pd = physicsDimensions();
		unsigned area = (pd.x * pd.y)/1.5;

		b2Vec2 radiation_center(15, 15);
		linear_color_scale<float> color_scale(0, 90,
			sf::Color(0x24, 0x21, 0x1E), sf::Color(0x12, 0x16, 0x16));

		polygonDef block_def;
		block_def.bodyDef.active = false;
		block_def.bodyDef.type = b2_staticBody;
		block_def.shape.SetAsBox(5, 1);
		maxProgress(area);
		bodies.reserve(area);
		for(unsigned i = 0; i < area; ++i)
		{
			b2Vec2 position;
			do
			{
				position = random_in(b2Vec2(-5, -5), pd+b2Vec2(5, 5));
			} while((position - radiation_center).LengthSquared() < 2);

			b2Vec2 relative = position - radiation_center;
			float angle = std::atan2(relative.y, relative.x) + (pi/2);
			float length = relative.Length();

			block_def.bodyDef.angle = randcentered(angle, to_radians(20));
			block_def.bodyDef.position = position;
			bodies.emplace_back(
				make_shape(world(), block_def),
				random_val_sat(
					color_scale(length),
					sf::Vector2f(5, 5)),
					length);
			addProgress(1);
		}
		background() = sf::Color(255, 0xA7, 0);
	}

	void render() override
	{
		for(auto& body : bodies)
		{
			drawBody(*body.body, body.color, sf::Color::Transparent, 0.0);
		}

	}


};
