#include "helpers.h"
#include "rand.h"

b2Body* make_box(b2World& world, const b2Vec2& center, const b2Vec2& extents, float angle, short group)
{
	polygonDef def;
	def.bodyDef.position = center;
	def.bodyDef.angle = angle;
	def.bodyDef.type = b2_dynamicBody;
	def.bodyDef.angularDamping = 0.5;
	def.shape.SetAsBox(extents.x, extents.y);
	def.fixtureDef.density = 1;
	def.fixtureDef.friction = 1;
	def.fixtureDef.restitution = 0.3;
	def.fixtureDef.filter.groupIndex = group;

	return make_shape(world, def);
}

const double pi =  3.1415926535897932384626433832795028;

double to_degrees(double radians)
{
	return (radians * 180) / pi;
}

double to_radians(double degrees)
{
	return (degrees * pi) / 180;
}