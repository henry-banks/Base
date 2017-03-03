#pragma once
//Lifetime but with a different name because the Lifetime is MYSTERIOUSLY NOT WORKING
#include <string>

namespace base
{

	class Timer
	{
		float m_lifetime;
	public:
		float lifespan;
		std::string name;

		Timer(float a_span = 1.f, std::string a_name = "timer") : lifespan(a_span), m_lifetime(0), name(a_name) {}

		void age(float dt) { m_lifetime += dt; }

		bool isAlive() const { return m_lifetime < lifespan; }
		float pctDead() const { return m_lifetime / lifespan; }
		void reset() { m_lifetime = 0; }
	};

}