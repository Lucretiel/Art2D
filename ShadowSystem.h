#pragma once

#include "Simulation.h"
#include <memory>

struct ShadowLayer: public sf::Drawable
{
private:
	sf::Sprite sprite;
public:
	std::shared_ptr<sf::RenderTexture> target;
	sf::Color background;
	
	sf::Color shadow_color;
	sf::Vector2f shadow_offset;
	sf::BlendMode shadow_blend;

	ShadowLayer(sf::Vector2u size, sf::Vector2f offset):
		target(std::make_shared<sf::RenderTexture>()),
		background(sf::Color(0, 0, 0, 0)),
		shadow_color(sf::Color(10, 10, 10)),
		shadow_offset(offset),
		shadow_blend(sf::BlendMultiply)
	{
		target->create(size.x, size.y);
		sprite.setTexture(target->getTexture());
	}
	
	void draw(sf::RenderTarget& render_target, sf::RenderStates states) const override
	{
		render_target.draw(sprite, states);
	}

	void clear()
	{
		target->clear(background);
	}

};


class ShadowSystem: public Simulation
{
private:
	std::vector<ShadowLayer> layers;

	void add_layer() {}
	template<class... Vec>
	void add_layer(const sf::Vector2f& offset, const Vec&... rest)
	{
		layers.emplace_back(windowDimensions(), offset);
		add_layer(rest...);
	}

	void render() override
	{
		layer_render();
		for(auto& layer : layers)
		{
			layer.target->display();
			window().draw(layer);
			layer.clear();
		}
	}

	virtual void layer_render() =0;

protected:
	template<class Draw>
	void draw_shadowed_object(Draw& object, int layer, sf::RenderStates states=sf::RenderStates::Default)
	{
		layers[layer].target->draw(object, states);
		sf::Color original_color(object.getFillColor());

		typedef std::vector<ShadowLayer>::iterator layer_iterator;
		typedef std::reverse_iterator<layer_iterator> r_layer_iterator;
		for(--layer; layer >= 0; --layer)
		{
			states.blendMode = layers[layer].shadow_blend;
			states.transform.translate(layers[layer].shadow_offset);
			object.setFillColor(layers[layer].shadow_color);
			layers[layer].target->draw(object, states);
		}
		object.setFillColor(original_color);
	}

	template<class... Vec>
	void set_layers(const Vec&... offsets)
	{
		layers.clear();
		add_layer(offsets...);
	}

	ShadowLayer& get_layer(int layer)
	{
		return layers[layer];
	}

};
