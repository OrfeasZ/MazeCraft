#include "Light.h"

using namespace Rendering::Objects;

Light::Light() :
	m_Position(0.f, 0.f, 0.f),
	m_Color(1.f, 1.f, 1.f),
	m_Attenuation(0.f),
	m_AmbientCoefficient(0.f),
	m_Active(false)
{
}

Light::~Light()
{
}