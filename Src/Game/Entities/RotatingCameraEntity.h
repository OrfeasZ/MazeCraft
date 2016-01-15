#pragma once

#include "CameraEntity.h"

namespace Game
{
	namespace Entities
	{
		class RotatingCameraEntity :
			public CameraEntity
		{
		public:
			RotatingCameraEntity();

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		public:
			inline glm::vec3 OrbitPosition() const { return m_OrbitPosition; }
			inline void OrbitPosition(glm::vec3 p_Position) { m_OrbitPosition = p_Position; }

			inline double OrbitDistance() const { return m_OrbitDistance; }
			inline void OrbitDistance(double p_Distance) { m_OrbitDistance = p_Distance; }

			inline double OrbitAngle() const { return m_OrbitAngle; }
			inline void OrbitAngle(double p_Angle) { m_OrbitAngle = p_Angle; }

			inline double RotationSpeed() const { return m_RotationSpeed; }
			inline void RotationSpeed(double p_Speed) { m_RotationSpeed = p_Speed; }

		protected:
			glm::vec3 m_OrbitPosition;
			double m_OrbitDistance;
			double m_OrbitAngle;
			double m_RotationSpeed;
			double m_RotationAngle;
		};
	}
}