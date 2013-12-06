#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include <array>

#include "helpers.h"
#include "rand.h"

template<class T>
inline sf::Uint8 clamp_color(T color)
{
	const int min_color = 0;
	const int max_color = 255;
	const int color_val = color;

	return static_cast<sf::Uint8>(
		color_val > max_color
			? max_color
			: color_val < min_color
				? min_color
				: color_val);
}

inline sf::Color random_rgb(const sf::Color& base, const sf::Vector3f& deltas)
{
	return sf::Color(
		clamp_color(base.r + randcentered(0, deltas.x)),
		clamp_color(base.g + randcentered(0, deltas.y)),
		clamp_color(base.b + randcentered(0, deltas.z)),
		base.a);
}

inline sf::Color really_random_rdb()
{
	return sf::Color(
		randint(0, 255),
		randint(0, 255),
		randint(0, 255));
}


inline const float val_sat_transform(const float comp, const float max, const float sat_factor, const float value_factor)
{
	return clamp_color(
		comp +
		((max - comp) * sat_factor) +
		(comp * value_factor));
}

inline sf::Color random_val_sat(const sf::Color& base, const sf::Vector2f deltas)
{
	const std::array<float, 3> color_array = {base.r, base.g, base.b};
	const auto mM = std::minmax_element(color_array.begin(), color_array.end());
	const float min = *mM.first;
	const float max = *mM.second;

	const float old_value = *mM.second;
	const float new_value = randcentered(old_value, deltas.x);
	const float value_factor = new_value / old_value - 1;

	const float old_sat = max - min;
	const float new_sat = randcentered(old_sat, deltas.y);
	const float sat_factor = (old_sat == 0 ? 0 : (new_sat / old_sat) - 1);


	return sf::Color(
		val_sat_transform(color_array[0], max, sat_factor, value_factor),
		val_sat_transform(color_array[1], max, sat_factor, value_factor),
		val_sat_transform(color_array[2], max, sat_factor, value_factor),
		base.a);
}

inline sf::Color to_val_sat(const sf::Color& color, const sf::Vector2f amounts)
{
	const std::array<float, 3> color_array = {color.r, color.g, color.b};
	const auto mM = std::minmax_element(color_array.begin(), color_array.end());
	const float min = *mM.first;
	const float max = *mM.second;

	const float old_value = *mM.second;
	const float new_value = amounts.x < 0 ? old_value : amounts.x;
	const float value_factor = new_value / old_value - 1;

	const float old_sat = max - min;
	const float new_sat = amounts.y < 0 ? old_sat : amounts.y;
	const float sat_factor = (old_sat == 0 ? 0 : (new_sat / old_sat) - 1);


	return sf::Color(
		val_sat_transform(color_array[0], max, sat_factor, value_factor),
		val_sat_transform(color_array[1], max, sat_factor, value_factor),
		val_sat_transform(color_array[2], max, sat_factor, value_factor),
		color.a);
}

struct ColorTransform
{
	sf::Vector2f vs_deltas;
	sf::Vector3f rgb_deltas;
	sf::Vector2f vs_force;

	bool vs_flag;
	bool rgb_flag;
	bool force_flag;

	ColorTransform():
		vs_flag(false),
		rgb_flag(false),
		force_flag(false)
	{}

	void set_rgb(const sf::Vector3f& rgb)
	{
		rgb_deltas = rgb;
		rgb_flag = true;
	}

	void set_vs(const sf::Vector2f& vs)
	{
		vs_deltas = vs;
		vs_flag = true;
	}

	void set_force_vs(const sf::Vector2f& force)
	{
		vs_force = force;
		force_flag = true;
	}

	sf::Color apply_rgb(const sf::Color& color) const
	{
		return rgb_flag
			? random_rgb(color, rgb_deltas)
			: color;
	}

	sf::Color appy_vs(const sf::Color& color) const
	{
		return vs_flag
			? random_val_sat(color, vs_deltas)
			: color;
	}

	sf::Color apply_force(const sf::Color& color) const
	{
		return force_flag
			? to_val_sat(color, vs_force)
			: color;
	}

	sf::Color apply(const sf::Color& color) const
	{
		return apply_force(appy_vs(apply_rgb(color)));
	}

};

template<class Input>
struct linear_color_scale
{
	typedef int ComponentOutput;
	typedef range<Input> InputRange;
	typedef range<ComponentOutput> ComponentOutputRange;
	typedef linear_scale<Input, ComponentOutput> ComponentScale;

	InputRange input_range;
	std::array<ComponentOutputRange, 4> output_ranges;

	linear_color_scale(Input input_min, Input input_max, sf::Color color_min, sf::Color color_max):
		input_range(input_min, input_max)
	{
		output_ranges[0] = ComponentOutputRange(color_min.r, color_max.r);
		output_ranges[1] = ComponentOutputRange(color_min.g, color_max.g);
		output_ranges[2] = ComponentOutputRange(color_min.b, color_max.b);
		output_ranges[3] = ComponentOutputRange(color_min.a, color_max.a);
	}

	sf::Color operator() (Input input) const
	{
		return sf::Color(
			clamp_color(ComponentScale(input_range, output_ranges[0])(input)),
			clamp_color(ComponentScale(input_range, output_ranges[1])(input)),
			clamp_color(ComponentScale(input_range, output_ranges[2])(input)),
			clamp_color(ComponentScale(input_range, output_ranges[3])(input)));
	}

};