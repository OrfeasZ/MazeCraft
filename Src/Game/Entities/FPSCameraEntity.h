#pragma once

#include "CameraEntity.h"

namespace Game
{
	namespace Entities
	{
		class FPSCameraEntity :
			public CameraEntity
		{
		public:
			FPSCameraEntity();

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		protected:
			double m_LastMouseX;
			double m_LastMouseY;
		};
	}
}