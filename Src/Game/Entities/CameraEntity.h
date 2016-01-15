#pragma once

#include "IEntity.h"

namespace Game
{
	namespace Entities
	{
		class CameraEntity :
			public IEntity
		{
		public:
			CameraEntity();
			virtual ~CameraEntity() override;

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		public:
			inline bool Active() const { return m_Active; }
			inline void Active(bool p_Active) { m_Active = p_Active; m_Dirty = true; }

			inline glm::vec3 Position() const { return m_Position; }
			inline void Position(glm::vec3 p_Position) { m_Position = p_Position; m_Dirty = true; }

			inline glm::vec2 Angles() const { return m_Angles; }
			inline void Angles(glm::vec2 p_Angles) { m_Angles = p_Angles; m_Dirty = true; }

			inline float FOV() const { return m_FOV; }
			inline void FOV(float p_FOV) { m_FOV = p_FOV; m_Dirty = true; }

		protected:
			bool m_Active;
			bool m_Dirty;
			glm::vec3 m_Position;
			glm::vec2 m_Angles;
			float m_FOV;
		};
	}
}