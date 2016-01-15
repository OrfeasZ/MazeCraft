#pragma once

#include <Misc/StdAfx.h>

namespace Rendering
{
	namespace Objects
	{
		class Light
		{
		public:
			Light();
			~Light();

		public:
			inline glm::vec3 Position() const { return m_Position; }
			inline void Position(glm::vec3 p_Position) { m_Position = p_Position; }

			inline glm::vec3 Color() const { return m_Color; }
			inline void Color(glm::vec3 p_Color) { m_Color = p_Color; }

			inline float Attenuation() const { return m_Attenuation; }
			inline void Attenuation(float p_Value) { m_Attenuation = p_Value; }

			inline float AmbientCoefficient() const { return m_AmbientCoefficient; }
			inline void AmbientCoefficient(float p_Value) { m_AmbientCoefficient = p_Value; }

			inline bool Active() const { return m_Active; }
			inline void Active(bool p_Active) { m_Active = p_Active; }

		protected:
			glm::vec3 m_Position;
			glm::vec3 m_Color;
			float m_Attenuation;
			float m_AmbientCoefficient;
			bool m_Active;
		};
	};
}