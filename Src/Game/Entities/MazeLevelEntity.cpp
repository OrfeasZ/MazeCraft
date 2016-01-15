#include "MazeLevelEntity.h"

#include "MazeBlockEntity.h"

#include <random>

#include <Managers/ModelManager.h>
#include <Managers/ShaderManager.h>

#include <Rendering/Objects/Model.h>

using namespace Game::Entities;

MazeLevelEntity::MazeLevelEntity(uint32_t p_Level, uint32_t p_Size, const std::string& p_LevelName) :
	m_Level(p_Level),
	m_Size(p_Size),
	m_LevelName(p_LevelName)
{
	m_Blocks = (MazeBlockEntity**) malloc(p_Size * p_Size * sizeof(MazeLevelEntity*));
	memset(m_Blocks, 0x00, p_Size * p_Size * sizeof(MazeLevelEntity*));
}

MazeLevelEntity::~MazeLevelEntity()
{
	for (int i = 0; i < m_Size * m_Size; ++i)
		if (m_Blocks[i])
			delete m_Blocks[i];

	free(m_Blocks);

	for (auto s_Model : m_FloorModels)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(s_Model);
		delete s_Model;
	}

	for (auto s_Model : m_WallModels)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(s_Model);
		delete s_Model;
	}

	for (auto s_Model : m_FrameModels)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(s_Model);
		delete s_Model;
	}

	m_FloorModels.clear();
	m_WallModels.clear();
	m_FrameModels.clear();
}

void MazeLevelEntity::Init()
{
	// TODO: Create surrounding walls for each level.
	// TODO: Create door frames for each level.

	for (int i = 0; i < m_Size * m_Size; ++i)
		if (m_Blocks[i])
			m_Blocks[i]->Init();
}

void MazeLevelEntity::Update(double p_Delta)
{
	// We don't need to task this, since blocks don't have much logic.
	for (int i = 0; i < m_Size * m_Size; ++i)
		if (m_Blocks[i])
			m_Blocks[i]->Update(p_Delta);
}

void MazeLevelEntity::SetBlock(uint32_t p_Index, const std::string& p_Type)
{
	if (p_Index >= m_Size * m_Size)
		return;

	// Delete the previous block we had here.
	if (m_Blocks[p_Index])
		delete m_Blocks[p_Index];

	// Create a new one based on type.
	if (p_Type == "R")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Red, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "G")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Green, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "B")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Blue, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "T1")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Texture1, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "T2")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Texture2, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "T3")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Texture3, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "W")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::WarpZone, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "D")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Destroyed, p_Index % m_Size, p_Index / m_Size, m_Level);
	else if (p_Type == "E")
		m_Blocks[p_Index] = new MazeBlockEntity(MazeBlockEntity::Empty, p_Index % m_Size, p_Index / m_Size, m_Level);
	else
		m_Blocks[p_Index] = nullptr;

	int s_X = p_Index % m_Size;
	int s_Y = p_Index / m_Size;

	// Create floor.
	if (m_Level == 0 && p_Type == "E")
	{
		auto s_FloorModel = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("floor"), 
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

		s_FloorModel->Position(glm::vec3((s_X) * 2.0, (m_Level * 2.0) - 0.999, (s_Y)* 2.0));
		s_FloorModel->Color(glm::vec4(0.7, 0.7, 0.7, 1.0));

		m_FloorModels.push_back(s_FloorModel);
		Managers::ModelManager::GetInstance()->RegisterModel(s_FloorModel);
	}

	// Create walls.
	if (s_X == 0)
	{
		auto s_WallModel = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("floor"),
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

		s_WallModel->Position(glm::vec3(((s_X) * 2.0) - 1.001f, (m_Level * 2.0), (s_Y) * 2.0));

		s_WallModel->Color(glm::vec4(0.3, 0.3, 0.3, 1.0));
		s_WallModel->Rotate(1.5708, glm::vec3(0.0, 0.0, 1.0));
		s_WallModel->ShouldRender(false);

		m_WallModels.push_back(s_WallModel);
		Managers::ModelManager::GetInstance()->RegisterModel(s_WallModel);
	}
	else if (s_X == m_Size - 1)
	{
		auto s_WallModel = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("floor"),
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

		s_WallModel->Position(glm::vec3(((s_X) * 2.0) + 1.001f, (m_Level * 2.0), (s_Y) * 2.0));

		s_WallModel->Color(glm::vec4(0.3, 0.3, 0.3, 1.0));
		s_WallModel->Rotate(1.5708, glm::vec3(0.0, 0.0, 1.0));
		s_WallModel->ShouldRender(false);

		m_WallModels.push_back(s_WallModel);
		Managers::ModelManager::GetInstance()->RegisterModel(s_WallModel);
	}
	
	if (s_Y == 0)
	{
		auto s_WallModel = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("floor"),
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

		s_WallModel->Position(glm::vec3(((s_X) * 2.0), (m_Level * 2.0), ((s_Y) * 2.0) - 1.001f));

		s_WallModel->Color(glm::vec4(0.3, 0.3, 0.3, 1.0));
		s_WallModel->Rotate(1.5708, glm::vec3(1.0, 0.0, 0.0));
		s_WallModel->ShouldRender(false);

		m_WallModels.push_back(s_WallModel);
		Managers::ModelManager::GetInstance()->RegisterModel(s_WallModel);
	}
	else if (s_Y == m_Size - 1)
	{
		auto s_WallModel = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("floor"),
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

		s_WallModel->Position(glm::vec3(((s_X) * 2.0), (m_Level * 2.0), ((s_Y) * 2.0) + 1.001f));

		s_WallModel->Color(glm::vec4(0.3, 0.3, 0.3, 1.0));
		s_WallModel->Rotate(1.5708, glm::vec3(1.0, 0.0, 0.0));
		s_WallModel->ShouldRender(false);

		m_WallModels.push_back(s_WallModel);
		Managers::ModelManager::GetInstance()->RegisterModel(s_WallModel);
	}
}

bool MazeLevelEntity::GetRandomEmptyPosition(glm::vec3& p_Position, int32_t& p_X, int32_t& p_Y)
{
	p_Position = glm::vec3(1.0, -1.0, 1.0);
	p_X = 0;
	p_Y = 0;

	// Collect all empty blocks.
	std::vector<MazeBlockEntity*> s_EmptyBlocks;

	for (uint32_t i = 0; i < m_Size * m_Size; ++i)
		if (m_Blocks[i]->Type() == MazeBlockEntity::Empty)
			s_EmptyBlocks.push_back(m_Blocks[i]);

	if (s_EmptyBlocks.size() == 0)
		return false;
	
	// Select a random empty block.
	std::random_device s_Device;
	std::mt19937 s_Generator(s_Device());

	auto s_RandomBlock = s_EmptyBlocks.begin();

	std::uniform_int_distribution<> s_Distribution(0, std::distance(s_RandomBlock, s_EmptyBlocks.end()) - 1);
	std::advance(s_RandomBlock, s_Distribution(s_Generator));

	// Calculate the player location based on that block.
	p_Position = glm::vec3((*s_RandomBlock)->X() * 2.f, (m_Level * 2.f) - 1.f, (*s_RandomBlock)->Y() * 2.f);
	p_X = (*s_RandomBlock)->X();
	p_Y = (*s_RandomBlock)->Y();

	return true;
}

void MazeLevelEntity::SetWallVisibility(bool p_Visible)
{
	for (auto s_Wall : m_WallModels)
		s_Wall->ShouldRender(p_Visible);
}

void MazeLevelEntity::PopulateFrames()
{
	// Populate on X axis.
	for (int y = 0; y < m_Size; ++y)
	{
		for (int x = 1; x < m_Size; ++x)
		{
			auto s_Block = GetBlock(x, y);
			auto s_PrevBlock = GetBlock(x - 1, y);

			if (s_Block->Type() == MazeBlockEntity::Empty && s_PrevBlock->Type() == MazeBlockEntity::Empty)
			{
				auto s_FrameModel = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("frame"),
					Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

				s_FrameModel->Position(glm::vec3((x * 2.0) - 1.0, (m_Level * 2.0), (y * 2.0)));

				s_FrameModel->Color(glm::vec4(0.5, 0.5, 0.5, 1.0));
				s_FrameModel->Rotate(1.5708, glm::vec3(0.0, 1.0, 0.0));

				m_FrameModels.push_back(s_FrameModel);
				Managers::ModelManager::GetInstance()->RegisterModel(s_FrameModel);
			}
		}
	}

	// Populate on Y axis.
	for (int x = 0; x < m_Size; ++x)
	{
		for (int y = 1; y < m_Size; ++y)
		{
			auto s_Block = GetBlock(x, y);
			auto s_PrevBlock = GetBlock(x, y - 1);
			
			if (s_Block->Type() == MazeBlockEntity::Empty && s_PrevBlock->Type() == MazeBlockEntity::Empty)
			{
				auto s_FrameModel = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("frame"),
					Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

				s_FrameModel->Position(glm::vec3((x * 2.0), (m_Level * 2.0), (y * 2.0) - 1.0));

				s_FrameModel->Color(glm::vec4(0.5, 0.5, 0.5, 1.0));

				m_FrameModels.push_back(s_FrameModel);
				Managers::ModelManager::GetInstance()->RegisterModel(s_FrameModel);
			}
		}
	}
}