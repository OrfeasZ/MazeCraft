#pragma once

#include "IEntity.h"

namespace Rendering
{
	namespace Objects
	{
		class Model;
		class Light;
	}
}

namespace Game
{
	namespace Entities
	{
		class FPSCameraEntity;

		class PlayerEntity :
			public IEntity
		{
		public:
			PlayerEntity();
			virtual ~PlayerEntity() override;

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		public:
			void HasControl(bool p_Control);
			bool SetCoordinates(int32_t p_X, int32_t p_Y, int32_t p_Level, bool p_Initial = false);
			void Position(glm::vec3 p_Position);

		public:
			inline bool HasControl() const { return m_HasControl; }
			inline int32_t CurrentX() const { return m_CurrentX; }
			inline int32_t CurrentY() const { return m_CurrentY; }
			inline int32_t CurrentLevel() const { return m_CurrentLevel; }
			inline void HammerCount(uint32_t p_Count) { m_HammerCount = p_Count; }
			inline uint32_t HammerCount() const { return m_HammerCount; }

		protected:
			Rendering::Objects::Model* m_PlayerModel;
			Rendering::Objects::Light* m_PlayerLight;
			FPSCameraEntity* m_Camera;
			bool m_HasControl;
			int32_t m_CurrentX;
			int32_t m_CurrentY;
			int32_t m_CurrentLevel;
			uint32_t m_HammerCount;
		};
	}
}