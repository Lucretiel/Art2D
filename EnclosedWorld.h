#pragma once
#include <array>
#include <Box2D\Box2D.h>

class EnclosedWorld : public b2World
{
public:
	//hmmm who made this public
	std::array<b2Body*, 4> m_edges;
private:
	const b2Vec2 m_dimensions;

	void setEdge(const unsigned index, bool active)
	{
		m_edges.at(index)->SetActive(active);
	}

public:
	EnclosedWorld(const b2Vec2& gravity, const b2Vec2 dimensions)
		:b2World(gravity)
		,m_dimensions(dimensions)
	{
		b2BodyDef edgeDef;
		edgeDef.type = b2_staticBody;
		edgeDef.position = b2Vec2(0, 0);
		edgeDef.active = true;
		for(int i = 0; i < 4; ++i)
			m_edges[i] = CreateBody(&edgeDef);

		const b2Vec2 topleft(0,0);
		const b2Vec2 topright(dimensions.x, 0);
		const b2Vec2 bottomright(dimensions.x, dimensions.y);
		const b2Vec2 bottomleft(0, dimensions.y);

		b2EdgeShape edge;

		//Top
		edge.Set(topright, topleft);
		m_edges[0]->CreateFixture(&edge, 0);

		//Left
		edge.Set(topleft, bottomleft);
		m_edges[1]->CreateFixture(&edge, 0);

		//Bottom
		edge.Set(bottomleft, bottomright);
		m_edges[2]->CreateFixture(&edge, 0);

		//Right
		edge.Set(bottomright, topright);
		m_edges[3]->CreateFixture(&edge, 0);
	}
	~EnclosedWorld() {}

	b2Vec2 dimensions() const {return m_dimensions;}
	b2Vec2 halfDimensions() const {b2Vec2(m_dimensions.x / 2, m_dimensions.y / 2);}

	void setTopEdge(bool setting) {setEdge(0, setting);}
	void setBottomEdge(bool setting) {setEdge(2, setting);}
	void setLeftEdge(bool setting) {setEdge(1, setting);}
	void setRightEdge(bool setting) {setEdge(3, setting);}
	void setAllEdges(bool setting)
	{
		for(unsigned i = 0; i < m_edges.size(); ++i)
			setEdge(i, setting);
	}
};

