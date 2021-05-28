#include "Demo.hpp"

float g_shapeSize = 5.f;

void Demo::add_shape(vec2d pos)
{
	polygon& shape = m_shapes.emplace_back();

	shape.pos = pos;
	shape.size = g_shapeSize;
	shape.model_points[0] = { -g_shapeSize, -g_shapeSize };
	shape.model_points[1] = { -g_shapeSize, +g_shapeSize };
	shape.model_points[2] = { +g_shapeSize, +g_shapeSize };
	shape.model_points[3] = { +g_shapeSize, -g_shapeSize };
}

void Demo::add_rect(vec2d pos, vec2d params)
{
	polygon& shape = m_shapes.emplace_back();

	shape.pos = pos;
	shape.size = g_shapeSize;
	shape.model_points[0] = { -params.x, -params.y };
	shape.model_points[1] = { -params.x, +params.y };
	shape.model_points[2] = { +params.x, +params.y };
	shape.model_points[3] = { +params.x, -params.y };
	shape.is_static = true;
}

void Demo::update(float dt)
{
	for (polygon& shape: m_shapes)
	{
		for (int i = 0; i < shape.model_points.size(); i++)
			shape.world_points[i] =
		{	// 2D Rotation Transform + 2D Translation
			(shape.model_points[i].x * cosf(shape.angle)) - (shape.model_points[i].y * sinf(shape.angle)) + shape.pos.x,
			(shape.model_points[i].x * sinf(shape.angle)) + (shape.model_points[i].y * cosf(shape.angle)) + shape.pos.y,
		};
		if (selected != &shape && !shape.is_static)
		{
		    //shape.pos.y += shape.acc.y * dt;
		    //shape.acc.y += 9.8f * dt;
		}
	}

	if (selected)
	{
		selected->pos.x += speed.x * dt;
		selected->pos.y += speed.y * dt;
	}

	for (int m = 0; m < m_shapes.size(); m++)
	{
		for (int n = m + 1; n < m_shapes.size(); n++)
		{
			check_and_resolve_collision_static(m_shapes[m], m_shapes[n]);
		}
	}
}

void Demo::select_shape(float x, float y)
{
	for (auto& shape : m_shapes)
	{
		if (x > shape.world_points[0].x && x < shape.world_points[3].x && y > shape.world_points[0].y && y < shape.world_points[1].y) 
		{
			if (selected)
			{
				selected->is_selected = false;
			}
			shape.is_selected = true;
			selected = &shape;
			return;
		}
	}
}

void Demo::set_selected_speed(float x, float y)
{
	speed.x = x;
	speed.y = y;
}

bool Demo::check_and_resolve_collision_static(polygon& r1, polygon& r2) const
{
	//polygon* poly1 = &r1;
	//polygon* poly2 = &r2;

	//float overlap = INFINITY;

	//for (int shape = 0; shape++ < 2; std::swap(poly1, poly2))
	//{
	//	for (int a = 0; a < poly1->world_points.size(); a++)
	//	{
	//		int b = (a + 1) % poly1->world_points.size();
	//		vec2d axisProj = { -(poly1->world_points[b].y - poly1->world_points[a].y), poly1->world_points[b].x - poly1->world_points[a].x };

	//		float min_r1 = INFINITY, max_r1 = -INFINITY;
	//		for (int p = 0; p < poly1->world_points.size(); p++)
	//		{
	//			float q = (poly1->world_points[p].x * axisProj.x + poly1->world_points[p].y * axisProj.y);
	//			min_r1 = std::min(min_r1, q);
	//			max_r1 = std::max(max_r1, q);
	//		}

	//		float min_r2 = INFINITY, max_r2 = -INFINITY;
	//		for (int p = 0; p < poly2->world_points.size(); p++)
	//		{
	//			float q = (poly2->world_points[p].x * axisProj.x + poly2->world_points[p].y * axisProj.y);
	//			min_r2 = std::min(min_r2, q);
	//			max_r2 = std::max(max_r2, q);
	//		}

	//		overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

	//		if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
	//			return false;
	//	}
	//}

	//vec2d d = { r2.pos.x - r1.pos.x, r2.pos.y - r1.pos.y };
	//float s = sqrtf(d.x * d.x + d.y * d.y);
	//r1.pos.x -= overlap * d.x / s;
	//r1.pos.y -= overlap * d.y / s;
	//return false;

	polygon* poly1 = &r1;
	polygon* poly2 = &r2;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			poly1 = &r2;
			poly2 = &r1;
		}

		// Check diagonals of this polygon...
		for (int world_points = 0; world_points < poly1->world_points.size(); world_points++)
		{
			vec2d line_r1s = poly1->pos;
			vec2d line_r1e = poly1->world_points[world_points];

			vec2d displacement = { 0,0 };

			// ...against edges of this polygon
			for (int q = 0; q < poly2->world_points.size(); q++)
			{
				vec2d line_r2s = poly2->world_points[q];
				vec2d line_r2e = poly2->world_points[(q + 1) % poly2->world_points.size()];

				// Standard "off the shelf" line segment intersection
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
				{
					displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
					displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
				}
			}

			if (!r1.is_static && !r2.is_static)
			{
			    r1.pos.x += displacement.x / 2 * (shape == 0 ? -1 : +1);
			    r1.pos.y += displacement.y / 2 * (shape == 0 ? -1 : +1);
				/*if (displacement.y > 0.001)
				{
				    r1.acc.y = 0.f;
					r2.acc.y = 0.f;
				}*/
				
			    r2.pos.x -= displacement.x / 2 * (shape == 0 ? -1 : +1);
			    r2.pos.y -= displacement.y / 2 * (shape == 0 ? -1 : +1);
			}
			else if (r1.is_static)
			{
			    //r2.pos.x -= displacement.x * (shape == 0 ? -1 : +1);
			    r2.pos.y -= displacement.y * (shape == 0 ? -1 : +1);
				if (displacement.y > 0.001)
				{
				    r1.acc.y = 0.f;
					r2.acc.y = 0.f;
					r2.is_static = true;
				}
			}
			else
			{
			    //r1.pos.x += displacement.x * (shape == 0 ? -1 : +1);
			    r1.pos.y += displacement.y * (shape == 0 ? -1 : +1);
				if (displacement.y > 0.001)
				{
					r2.is_static = true;
				    r1.acc.y = 0.f;
					r2.acc.y = 0.f;
				}
			}

			
		}
	}
	return false;
}
