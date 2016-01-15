#pragma once

#include "IEntity.h"

#include <vector>

namespace Rendering
{
	namespace Objects
	{
		class Model;
	}
}

namespace Game
{
	namespace Entities
	{
		class MazeBlockEntity;

		class MazeLevelEntity :
			public IEntity
		{
		public:
			MazeLevelEntity(uint32_t p_Level, uint32_t p_Size, const std::string& p_LevelName);
			virtual ~MazeLevelEntity() override;

		public:
			virtual void Init() override;
			virtual void Update(double p_Delta) override;

		public:
			void SetBlock(uint32_t p_Index, const std::string& p_Type);
			bool GetRandomEmptyPosition(glm::vec3& p_Position, int32_t& p_X, int32_t& p_Y);
			void SetWallVisibility(bool p_Visible);
			void PopulateFrames();

		public:
			inline uint32_t GetLevel() const { return m_Level; }
			inline uint32_t GetSize() const { return m_Size; }
			inline std::string GetName() const { return m_LevelName; }
			inline MazeBlockEntity* GetBlock(uint32_t p_X, uint32_t p_Y) const { return (p_X >= m_Size || p_Y >= m_Size) ? nullptr : m_Blocks[p_X + (p_Y * m_Size)]; }
			inline MazeBlockEntity* GetBlock(uint32_t p_Index) const { return (p_Index >= m_Size * m_Size) ? nullptr : m_Blocks[p_Index]; }

		protected:
			uint32_t m_Level;
			uint32_t m_Size;
			std::string m_LevelName;
			MazeBlockEntity** m_Blocks;
			std::vector<Rendering::Objects::Model*> m_FloorModels;
			std::vector<Rendering::Objects::Model*> m_WallModels;
			std::vector<Rendering::Objects::Model*> m_FrameModels;
		};
	}
}