#pragma once

#include "compdef.h"
#include "Transform.h"

namespace base
{
	class Button
	{
	protected:
		//Max is upper-right, min is bottom-left
		vec2 max, min;

	public:
		Button(vec2 max_a, vec2 min_a) : max(max_a), min(min_a)
		{};

		bool isClicked()
		{
			if (!sfw::getMouseButton(0)) return false;

			vec2 mousePos = vec2{ sfw::getMouseX(), sfw::getMouseY() };
			if (mousePos > min && mousePos < max && sfw::getMouseButton(0)) return true;

			return false;
		}
	};
}