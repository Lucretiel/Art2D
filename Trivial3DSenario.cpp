#include "Trivial3DSenario.h"

#include "TypeRegistry.h"
REGISTER_SIMULATION(Trivial3DSenario, "trivial3d");

#include "helpers.h"

void Trivial3DSenario::initialize()
{
	sideWorld.SetGravity(b2Vec2(0, 20));

	sf::Vector2f w_center(windowHalfDimensions());

	backPlane = VanishPlane::backPlane(w_center, 0);
	floorPlane = VanishPlane::floorPlane(w_center, 0);
	sidePlane = VanishPlane::sidePlane(w_center, 0);
	
	auto p_center = physicsHalfDimensions();

	polygonDef def;
	def.bodyDef.position.Set(0, 50);
	def.bodyDef.angle = 0;
	def.bodyDef.type = b2_dynamicBody;
	def.bodyDef.angularVelocity = 5;
	def.bodyDef.linearVelocity.Set(20, -10);
	def.shape.SetAsBox(5, 5);
	def.fixtureDef.density = 1;
	sideBodies.push_back(make_shape(sideWorld, def));

	polygonDef def2;
	def2.bodyDef.position.Set(50, 100);
	def2.bodyDef.angle = 0;
	def2.bodyDef.type = b2_staticBody;
	def2.shape.SetAsBox(50, 5);
	def2.fixtureDef.density = 1;
	sideBodies.push_back(make_shape(sideWorld, def2));
}

void Trivial3DSenario::think(unsigned long frame)
{
	floorWorld.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);
	sideWorld.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);
	backWorld.Step(
		simParams().inv_frame_rate,
		simParams().velocityIterations,
		simParams().positionIterations);

	sidePlane.intersect += 5;
}

void Trivial3DSenario::render()
{
	drawBodies(floorBodies, floorPlane);
	drawBodies(sideBodies, sidePlane);
	drawBodies(backBodies, backPlane);
}