#pragma once

#include "IEntity.h"

#include <vector>

namespace Game
{
	namespace Entities
	{
		class MazeLevelEntity;

		class MazeEntity : 
			public IEntity
		{
		public:
			MazeEntity(const std::string& p_MazeName);
			virtual ~MazeEntity() override;

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		public:
			bool CanMove(glm::vec3 p_CurrentPosition, glm::vec3 p_TargetPosition, glm::vec3& p_FinalPosition, int32_t& p_X, int32_t& p_Y, int32_t& p_Level);
			void SetWallVisibility(bool p_Visible);

		public:
			inline size_t GetLevelCount() const { return m_Levels.size(); }
			inline MazeLevelEntity* GetLevel(size_t p_Index) const { return (p_Index >= GetLevelCount()) ? nullptr : m_Levels[p_Index]; }
			inline uint32_t InitialHammers() const { return m_InitialHammers; }

		protected:
			uint32_t m_InitialHammers;
			std::vector<MazeLevelEntity*> m_Levels;
		};
	}
}