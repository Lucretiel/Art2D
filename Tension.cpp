#include "Simulation.h"
#include "helpers.h"
#include "rand.h"

class Tension : public Simulation
{
	std::string title() override {return "Tension";}

	float elongate(float side)
	{
		return 8/side;
	}

	template<int inputmin, int inputmax, int outputmin, int outputmax>
	struct linear_scale
	{
		float operator()(float input)
		{
			const float input_range = inputmax - inputmin;
			const float output_range = outputmax - outputmin;

			return (((input - inputmin)/input_range)*output_range)+outputmin;
		}

	};


	void initialize() override
	{
		setAllEdges(false);
		world().SetGravity(b2Vec2_zero);

		b2Vec2 center(0, 80);
		const auto dimensions = physicsDimensions();
		maxProgress(50);

		linear_scale<0, 75, 3, 1> size_scale;
		linear_scale<0, 10000, 0, 180> angle_scale;
		for(int i = 0; i < 50; ++i)
		{
			polygonDef def;
			def.bodyDef.position = random_margin(dimensions, 3);
			def.bodyDef.type = b2_dynamicBody;
			def.fixtureDef.friction = 0;
			def.fixtureDef.density = 1;
			def.fixtureDef.isSensor = true;
			float distance = (def.bodyDef.position - center).Length();
			float side1 = size_scale(distance);
			float side2 = elongate(side1);
			def.shape.SetAsBox(side1, side2);
			def.bodyDef.angle = to_radians(randcentered(0, angle_scale(pow(distance, 2))));
			make_shape(world(), def);
			addProgress(1);
		}

	}

	void render() override
	{
		drawWorld(world(), &noTransform, sf::Color::Black, sf::Color::White, -1);
	}


};

#include "TypeRegistry.h"
REGISTER_SIMULATION(Tension, "tension");
