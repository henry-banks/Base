#pragma once

namespace base
{
	//Connects to an Entity with a collider and acts as a wall/floor to the playerController.
	class Boundary
	{
		bool isHoriz;

	public:
		Boundary(bool isHoriz_a = false) : isHoriz(isHoriz_a) {};
	};
}