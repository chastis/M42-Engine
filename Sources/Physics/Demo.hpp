#pragma once

#include <array>
#include <vector>

extern float g_shapeSize;

struct vec2d
{
	float x = 0.f;
	float y = 0.f;
};

struct polygon
{
	std::array<vec2d, 4> world_points;
	std::array<vec2d, 4> model_points;
	float angle = 0.f;
	vec2d pos;
	float size = 0.f;
	bool is_selected = false;
	bool is_static = false;
	vec2d acc;
};

class Demo
{
public:
	void add_shape(vec2d pos);
	void add_rect(vec2d pos, vec2d params);

	template <class Fn>
	void visit_shapes(Fn&& visitor) const;

	void update(float dt);

	void select_shape(float x, float y);

	void set_selected_speed(float x, float y);
	vec2d get_selected_speed() const { return speed; }

	polygon* get_selected() { return selected; }
private:

	bool check_and_resolve_collision_static(polygon& r1, polygon& r2) const;

	std::vector<polygon> m_shapes;

	polygon* selected = nullptr;
	vec2d speed;
};

template <class Fn>
void Demo::visit_shapes(Fn&& visitor) const
{
	for (const auto& shape : m_shapes)
	{
		visitor(shape);
	}
}
