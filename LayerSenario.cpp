#include "LayerSenario.h"

#include "helpers.h"
#include "rand.h"
#include "TypeRegistry.h"

REGISTER_SIMULATION(LayerSenario, "layer");

void LayerSenario::initialize()
{
	setRightEdge(false);
	maxProgress(layers.size() * 50);

	int iLayer = 0;
	for(auto layer = layers.begin(); layer != layers.end(); ++layer, ++iLayer)
	{
		for(int i = 0; i < 50; ++i)
		{
			polygonDef def;
			def.bodyDef.angularDamping = 0.5;
			def.bodyDef.linearDamping = 0.5;
			def.fixtureDef.density = 1;
			def.fixtureDef.filter.groupIndex = -1;
			float factor;
			def.shape.SetAsBox(3-iLayer, 5);

			switch(iLayer)
			{
			case 0:
				def.bodyDef.angle = to_radians(90);
				def.bodyDef.position = random_margin(physicsDimensions(), 5);
				factor = 1.2;
				break;
			case 1:
				def.bodyDef.angle = to_radians(90 + randuniform(-10, 10));
				def.bodyDef.position = random_margin(physicsDimensions(), 15);
				break;
			case 2:
				def.bodyDef.angle = to_radians(randuniform(0, 360));
				def.bodyDef.position = random_margin(physicsDimensions(), 20);
				break;
			}

			layer->push_back(make_shape(world(), def));
			addProgress(1);
		}
	}
}

void LayerSenario::render()
{
	for(int i = 0; i < layers.size(); ++i)
	{
		sf::Color fill;
		sf::Color outline(sf::Color::Black);
		switch(i)
		{
		case 0:
			fill = sf::Color(200, 200, 200);
			break;
		case 1:
			fill = sf::Color::Black;
			break;
		case 2:
			fill = sf::Color::White;
			break;
		}

		drawBodies(layers[i], fill, outline, -2);
	}
}