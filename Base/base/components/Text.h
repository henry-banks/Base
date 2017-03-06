#pragma once
#include <cstring>
#include "Transform.h"

namespace base
{

class Text
{
	char m_characters[80];
	
public:
	unsigned sprite_id;
	unsigned tint = WHITE;
	vec2 offset = vec2{ 0,0 };
	vec2 off_scale = vec2{ 1, 1 };
	void setString(const char *src) { strcpy_s(m_characters, 80, src); }
	Text() { setString(""); }

	void draw(const Transform *T, const mat3 &cam)
	{
		auto glob = cam * T->getGlobalTransform();

		auto pos = glob.getTrans2D() + offset;
		auto dim = glob.getScale2D();
		
		sfw::drawString(sprite_id, m_characters, pos.x,pos.y,dim.x*off_scale.x,dim.y*off_scale.y, 0, '\0', tint );
	}
};

}