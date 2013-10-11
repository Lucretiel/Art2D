#include "OpenSpace.h"



#include "helpers.h"
#include "rand.h"

REGISTER_SIMULATION(OpenSpace, "open");

InitParams OpenSpace::prepare()
{
	InitParams params;
	params.videoMode = sf::VideoMode(800, 1000);
	return params;
}

void OpenSpace::initialize()
{

	maxProgress(200);
	setAllEdges(false);
	world().SetGravity(b2Vec2(0, 10));

	upper = makePlatform(10, -25);
	addProgress(25);
	middle = makePlatform(50, -25);
	addProgress(25);
	
	auto pd = physicsDimensions();

	for(int i = 0; i < 150; ++i)
	{
		polygonDef def;
		def.bodyDef.position.Set(randuniform(pd.x, pd.x + 30), randuniform(-30, pd.y-30));
		def.bodyDef.angle = to_radians(randuniform(0, 360));
		def.bodyDef.type = b2_dynamicBody;
		def.shape.SetAsBox(1, 5);
		def.fixtureDef.restitution = 0.5;
		def.fixtureDef.density = 4;
		bodies.push_back(make_shape(world(), def));
		addProgress(1);
	}
}

b2Body* OpenSpace::makePlatform(float ycenter, float angle)
{
	auto pd = physicsDimensions();
	polygonDef def;
	def.bodyDef.position.Set(pd.x, ycenter);
	def.bodyDef.type = b2_staticBody;
	def.bodyDef.angle = to_radians(angle);
	def.bodyDef.fixedRotation = true;
	def.shape.SetAsBox(pd.x-40, 2);
	def.fixtureDef.density = 1;
	def.fixtureDef.friction = 0.4;
	return make_shape(world(), def);
}

void OpenSpace::render()
{
	drawBody(*upper, sf::Color::Black);
	drawBody(*middle, sf::Color::Black);

	drawBodies(bodies, sf::Color::Black, sf::Color::White, -2);
}