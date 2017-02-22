#pragma once

#include "Collider.h"

//Basically just a Collider with a different name
namespace base
{
	class Trigger : public base::Collider
	{
		hull m_hull;
		aabb m_localBox;
	public:
		Trigger(float radius = 0.5f) : m_hull(radius) { m_localBox = m_hull.boundingBox(); }
		Trigger(const vec2 *ccw, size_t a_size) : m_hull(ccw, a_size) { m_localBox = m_hull.boundingBox(); }
		Trigger(const hull &a_hull) : m_hull(a_hull) { m_localBox = m_hull.boundingBox(); }
	};
}
