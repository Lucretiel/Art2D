#include "VanishingPoint.h"

#include "TypeRegistry.h"
REGISTER_SIMULATION(VanishingPoint, "vanishing");

#include "rand.h"
#include "helpers.h"

const static sf::Vector2f vanishPoint(500, 600);
const static VanishPlane stackLeftBack(VanishPlane::sidePlane(vanishPoint, -400, sf::Vector2f(1000, 0)));
const static VanishPlane stackRightBack(VanishPlane::sidePlane(vanishPoint, 1000, sf::Vector2f(1000, 0)));
const static VanishPlane stackLeftFront(VanishPlane::sidePlane(vanishPoint, -400));
const static VanishPlane stackRightFront(VanishPlane::sidePlane(vanishPoint, 1000));
const static VanishPlane stackCenter(VanishPlane::sidePlane(vanishPoint, 200));
const static VanishPlane rearStack(VanishPlane::sidePlane(vanishPoint, 200, sf::Vector2f(1000, 0)));
const static VanishPlane floorPlane(VanishPlane::floorPlane(vanishPoint, 800, sf::Vector2f(100, 0)));

VanishingPoint::VanishingPoint()
	:goodStack(b2Vec2(0, 20), b2Vec2(100, 80))
	,badStack(b2Vec2(0, 20), b2Vec2(100, 80))
	,fallen(b2Vec2(0, 0), b2Vec2(80, 100))
	,verticals(b2Vec2(0, 0))
{}

InitParams VanishingPoint::prepare()
{
	InitParams params;
	params.videoMode = sf::VideoMode(800, 800);
	return params;
}
void VanishingPoint::initialize()
{
	{
		badStack.setLeftEdge(false);
		b2BodyDef edgeDef;
		edgeDef.type = b2_staticBody;
		edgeDef.position = b2Vec2(0, 0);
		edgeDef.active = true;
		b2EdgeShape edge;
		edge.Set(b2Vec2(-1000, 80), b2Vec2(1000, 80));
		auto edgeBody = badStack.CreateBody(&edgeDef);
		edgeBody->CreateFixture(&edge, 0);
	}
	maxProgress(300 * 2);
	for(int i = 0; i < 300; ++i)
	{
		polygonDef def;
		def.bodyDef.position = random_margin(goodStack.dimensions(), 5);
		def.bodyDef.angle = 0;
		def.bodyDef.fixedRotation = true;
		def.bodyDef.type = b2_dynamicBody;
		def.shape.SetAsBox(1, 5);
		def.fixtureDef.density = 1;
		make_shape(goodStack, def);

		addProgress(1);

		if(randint(0, 3))
		{
			def.bodyDef.position = random_margin(badStack.dimensions(), 5);
			def.bodyDef.angle = to_radians(randuniform(0, 360));
			def.bodyDef.fixedRotation = false;
			make_shape(badStack, def);
		}
		else
		{
			def.bodyDef.position = random_margin(fallen.dimensions(), 5);
			def.bodyDef.angle = to_radians(randuniform(0, 360));
			def.bodyDef.fixedRotation = false;
			def.bodyDef.type = b2_staticBody;
			make_shape(fallen, def);
		}
		addProgress(1);
	}

	polygonDef vertical;
	vertical.shape.SetAsBox(5, 40);
	vertical.fixtureDef.density = 1;
	vertical.bodyDef.type = b2_staticBody;
	vertical.bodyDef.angle = 0;
	vertical.bodyDef.position.y = 40;

	vertical.bodyDef.position.x = -40;
	make_shape(verticals, vertical);

	vertical.bodyDef.position.x = 20;
	make_shape(verticals, vertical);

	vertical.bodyDef.position.x = 100;
	make_shape(verticals, vertical);
}

void VanishingPoint::think(unsigned long frame)
{
	goodStack.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);
	badStack.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);
}

const static auto verticalsPlane1(VanishPlane::backPlane(vanishPoint, 2000));
const static auto verticalsPlane2(VanishPlane::backPlane(vanishPoint, 1000));
const static auto verticalsPlane3(VanishPlane::backPlane(vanishPoint, 0));

void VanishingPoint::render()
{
	drawWorld(verticals, verticalsPlane1, sf::Color::Black, sf::Color::White, -.5);

	drawWorld(goodStack, stackLeftBack, sf::Color::Black, sf::Color::White, -.5);
	drawWorld(goodStack, rearStack, sf::Color::Black, sf::Color::White, -.5);
	drawWorld(goodStack, stackRightBack, sf::Color::Black, sf::Color::White, -.5);

	drawWorld(verticals, verticalsPlane2, sf::Color::Black, sf::Color::White, -.5);

	drawWorld(goodStack, stackLeftFront, sf::Color::Black, sf::Color::White, -.5);
	drawWorld(fallen, floorPlane, sf::Color::Black, sf::Color::White, -.5);
	drawWorld(badStack, stackCenter, sf::Color::Black, sf::Color::White, -.5);
	drawWorld(goodStack, stackRightFront, sf::Color::Black, sf::Color::White, -.5);

	//drawWorld(verticals, verticalsPlane3, sf::Color::Black, sf::Color::White, .5);

}