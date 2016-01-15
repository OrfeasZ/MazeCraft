#include "MazeEntity.h"

#include <VFS/FileSystem.h>
#include <VFS/FSFile.h>

#include "MazeLevelEntity.h"
#include "MazeBlockEntity.h"

using namespace Game::Entities;

MazeEntity::MazeEntity(const std::string& p_MazeName) :
	m_InitialHammers(0)
{
	VFS::FSFile* s_File;

	if (!VFS::FileSystem::GetInstance()->OpenFile("/data/game/" + p_MazeName + ".maz", "rb", &s_File))
		return;

	int s_LevelCount;
	int s_Size;

	s_File->Scan("L=%d\n", &s_LevelCount);
	s_File->Scan("N=%d\n", &s_Size);
	s_File->Scan("K=%d\n", &m_InitialHammers);

	for (int i = 0; i < s_LevelCount; ++i)
	{
		std::string s_LevelName("");
		std::string s_ReadData = s_File->Read(1);

		while (s_ReadData != "\n" && s_ReadData != "\r")
		{
			s_LevelName += s_ReadData;
			s_ReadData = s_File->Read(1);
		}

		while (s_ReadData == " " || s_ReadData == "\t" || s_ReadData == "\n" || s_ReadData == "\r")
			s_ReadData = s_File->Read(1);

		s_File->Seek(-1, SEEK_CUR);

		auto s_Level = new MazeLevelEntity(i, s_Size, s_LevelName);

		for (int j = 0; j < s_Size * s_Size; ++j)
		{
			char s_BlockType[16];
			s_File->Scan("%s", &s_BlockType);

			// Read whitespace.
			std::string s_ReadChar = s_File->Read(1);
			while (s_ReadChar == " " || s_ReadChar == "\t" || s_ReadChar == "\n" || s_ReadChar == "\r")
				s_ReadChar = s_File->Read(1);

			s_File->Seek(-1, SEEK_CUR);

			s_Level->SetBlock(j, s_BlockType);
		}

		m_Levels.push_back(s_Level);
	}
}

MazeEntity::~MazeEntity()
{
}

void MazeEntity::Init()
{
	for (auto s_Level : m_Levels)
		s_Level->Init();
}

void MazeEntity::Update(double p_Delta)
{
	for (auto s_Level : m_Levels)
		s_Level->Update(p_Delta);
}

bool MazeEntity::CanMove(glm::vec3 p_CurrentPosition, glm::vec3 p_TargetPosition, glm::vec3& p_FinalPosition, int32_t& p_X, int32_t& p_Y, int32_t& p_Level)
{
	p_FinalPosition = p_CurrentPosition;

	// Get block for current position.
	int32_t s_X = (p_CurrentPosition.x + 1.f) / 2.f;
	int32_t s_Y = (p_CurrentPosition.z + 1.f) / 2.f;
	int32_t s_LevelIndex = (p_CurrentPosition.y + 1.f) / 2.f;

	p_X = s_X;
	p_Y = s_Y;
	p_Level = s_LevelIndex;

	// Make sure we're within bounds.
	if (s_LevelIndex >= m_Levels.size() || s_LevelIndex < 0)
		return false;

	MazeLevelEntity* s_Level = m_Levels[s_LevelIndex];

	// Does this block exist?
	auto s_Block = s_Level->GetBlock(s_X, s_Y);

	if (!s_Block)
		return false;

	// Get target block.
	int32_t s_TargetX = (p_TargetPosition.x + 1.f) / 2.f;
	int32_t s_TargetY = (p_TargetPosition.z + 1.f) / 2.f;

	// Prevent from exiting the maze.
	auto s_TargetBlock = s_Level->GetBlock(s_TargetX, s_TargetY);

	if (!s_TargetBlock)
		return false;

	// We're moving within the same block; allow that.
	if (s_X == s_TargetX && s_Y == s_TargetY)
	{
		p_FinalPosition = p_TargetPosition;
		return true;
	}
	
	// Only allow movement if the target block is empty, destroyed, or a warp zone.
	if (s_TargetBlock->Type() == MazeBlockEntity::Empty ||
		s_TargetBlock->Type() == MazeBlockEntity::Destroyed ||
		s_TargetBlock->Type() == MazeBlockEntity::WarpZone)
	{
		p_FinalPosition = p_TargetPosition;
		return true;
	}

	// TODO: Add some distance from the walls to prevent view clipping.
	// TODO: Don't completely disable movement to allow wall sliding.

	return false;
}

void MazeEntity::SetWallVisibility(bool p_Visible)
{
	for (auto s_Level : m_Levels)
		s_Level->SetWallVisibility(p_Visible);
}