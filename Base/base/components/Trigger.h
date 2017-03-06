#pragma once

#include "Collider.h"
#include <string>

//Basically just a Collider with a different name and tags
namespace base
{
	class Trigger : public base::Collider
	{
		aabb m_localBox;
		std::string tag;
	public:
		Trigger(float radius = 0.5f, std::string a_tag = "default") : Collider(radius), tag(a_tag) { }
		Trigger(const vec2 *ccw, size_t a_size, std::string a_tag = "default") : Collider(ccw, a_size), tag(a_tag) { m_localBox = m_hull.boundingBox(); }
		Trigger(const hull &a_hull, std::string a_tag = "default") : Collider(a_hull), tag(a_tag) { m_localBox = m_hull.boundingBox(); }

		hull m_hull;
		vec2 offset;
		bool isActive = false;

		void update(vec2 a_offset)
		{
			for (int i = 0; i < m_hull.size; i++)
			{
				m_hull.points[i] += offset;
			}
			m_hull.calculateNormals();
			m_localBox = m_hull.boundingBox();
		}
	};
}
