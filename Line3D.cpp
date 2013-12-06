#pragma once

#include "Simulation.h"
#include "helpers.h"
#include "rand.h"
#include "3DPlanes.h"

const static int lines_per_thing = 10;
const static float box_size = 500;

enum which_type {Top=0, Left, Bottom, Right};

SIM(Line3D, "3d_surface")
{
	typedef std::array<sf::Vertex, 2> line;
	std::vector<line> bodies;
	std::vector<sf::RectangleShape> rects;

	std::string title() override {return "3D Line Surface";}

	void initialize() override
	{
		auto window_center = windowHalfDimensions();
		auto window_size = windowDimensions();

		float left = 0;
		float bottom = window_size.y;
		float right = left + box_size;
		float top = bottom - box_size;

		sf::Vector2f vanish(window_center.x * 1.5, bottom - (box_size/2));

		std::array<float, 2> depths;
		depths.fill(0);
		std::array<which_type, 5> whiches = {Left, Bottom, Left, Top, Right};
		maxProgress(whiches.size() * lines_per_thing * 2);
		for(unsigned which_i = 0; which_i < whiches.size(); ++which_i)
		{
			auto which = whiches[which_i];
			VanishPlane plane = (which % 2 == 0) ?
				VanishPlane::floorPlane(vanish, which == 0 ? top : bottom) :
				VanishPlane::sidePlane(vanish, which == 1 ? left : right);

			if(which_i != 3)
			{
				depths[0] = depths[1];
				depths[1] += 800;
			}

			for(unsigned i = 0; i < lines_per_thing; ++i)
			{
				bodies.emplace_back();
				if(which % 2 == 0)
				{
					float x;
					switch(i)
					{
					case 0:
						x = left; break;
					case 1:
						x = right; break;
					default:
						x = randuniform(left, right); break;
					}

					for(unsigned p = 0; p < 2; ++p)
					{
						float y = depths[p];
						bodies.back()[p] = sf::Vertex(plane(sf::Vector2f(x, y)), sf::Color::Black);
						addProgress(1);
					}
				}
				else
				{
					float y;
					switch(i)
					{
					case 0:
						y = top; break;
					case 1:
						y = bottom; break;
					default:
						y = randuniform(top, bottom); break;
					}
					for(unsigned p = 0; p < 2; ++p)
					{
						float x = depths[p];
						bodies.back()[p] = sf::Vertex(plane(sf::Vector2f(x, y)), sf::Color::Black);
						addProgress(1);
					}
				}
			}
		}

		
		auto final_plane = VanishPlane::backPlane(vanish, depths[1]);
		sf::RectangleShape rect;
		sf::Vector2f topLeft;
		sf::Vector2f bottomRight;

		auto add_rect = [&](float x1, float y1, float x2, float y2, float depth)
		{
			final_plane.intersect = depth;
			topLeft = final_plane(sf::Vector2f(x1, y1));
			bottomRight = final_plane(sf::Vector2f(x2, y2));
			rect.setSize(bottomRight - topLeft);
			rect.setPosition(topLeft);
			rect.setFillColor(sf::Color::Black);
			rect.setOutlineThickness(0);
			rects.push_back(rect);
		};

		//RIGHT->
		add_rect(right, top, right+200000, bottom, depths[1]);
		//UP^
		add_rect(left, top-200000, right, top, 1600);
		//DOWN V
		add_rect(left, bottom, right, bottom + 200000, 800);
		//LEFT<-
		add_rect(left - 200000, top, left, bottom, 1600);
	}

	void render() override
	{
		for(auto& rect : rects)
		{
			window().draw(rect);
		}

		for(auto& line : bodies)
		{
			window().draw(line.data(), line.size(), sf::PrimitiveType::Lines);
		}
	}


};
