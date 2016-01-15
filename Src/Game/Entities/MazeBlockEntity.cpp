#include "MazeBlockEntity.h"

#include <Managers/ModelManager.h>
#include <Managers/TextureManager.h>
#include <Managers/ShaderManager.h>

#include <Rendering/Objects/CubeModel.h>

using namespace Game::Entities;

MazeBlockEntity::MazeBlockEntity(BlockType p_Type, int32_t p_X, int32_t p_Y, int32_t p_Level) :
	m_Type(p_Type),
	m_X(p_X),
	m_Y(p_Y),
	m_Level(p_Level),
	m_Model(nullptr),
	m_ExtraModel01(nullptr),
	m_ExtraModel02(nullptr)
{
}

MazeBlockEntity::~MazeBlockEntity()
{
	if (m_Model)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(m_Model);
		delete m_Model;
	}

	if (m_ExtraModel01)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(m_ExtraModel01);
		delete m_ExtraModel01;
	}

	if (m_ExtraModel02)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(m_ExtraModel02);
		delete m_ExtraModel02;
	}

	m_Model = nullptr;
	m_ExtraModel01 = nullptr;
	m_ExtraModel02 = nullptr;
}

void MazeBlockEntity::Init()
{
	// Create the required model.
	switch (m_Type)
	{
	case MazeBlockEntity::Red:
	case MazeBlockEntity::Green:
	case MazeBlockEntity::Blue:
		m_Model = new Rendering::Objects::CubeModel(Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));
		break;

	case MazeBlockEntity::Texture1:
	case MazeBlockEntity::Texture2:
	case MazeBlockEntity::Texture3:
		m_Model = new Rendering::Objects::CubeModel(Managers::ShaderManager::GetInstance()->GetShaderProgram("TexturedModel"));
		break;

	case MazeBlockEntity::Empty:
		m_Model = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("hatch"), 
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));
		break;

	case MazeBlockEntity::Destroyed:
		m_Model = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("debris"), 
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));
		break;

	case MazeBlockEntity::WarpZone:
		m_Model = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("warp-base"),
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

		m_ExtraModel01 = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("warp-ring"),
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

		m_ExtraModel02 = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("warp-ring"),
			Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));
		break;
	}

	// Set colors/textures.
	switch (m_Type)
	{
	case MazeBlockEntity::Red:
		m_Model->Color(glm::vec4(1.0, 0.0, 0.0, 1.0));
		break;

	case MazeBlockEntity::Green:
		m_Model->Color(glm::vec4(0.0, 1.0, 0.0, 1.0));
		break;

	case MazeBlockEntity::Blue:
		m_Model->Color(glm::vec4(0.0, 0.0, 1.0, 1.0));
		break;

	case MazeBlockEntity::Texture1:
		m_Model->Color(glm::vec4(0.9, 0.9, 0.9, 1.0));
		m_Model->Texture(Managers::TextureManager::GetInstance()->GetTexture("T1"));
		break;

	case MazeBlockEntity::Texture2:
		m_Model->Color(glm::vec4(0.9, 0.9, 0.9, 1.0));
		m_Model->Texture(Managers::TextureManager::GetInstance()->GetTexture("T2"));
		break;

	case MazeBlockEntity::Texture3:
		m_Model->Color(glm::vec4(0.9, 0.9, 0.9, 1.0));
		m_Model->Texture(Managers::TextureManager::GetInstance()->GetTexture("T3"));
		break;

	case MazeBlockEntity::Empty:
		m_Model->Color(glm::vec4(0.6, 0.6, 0.6, 1.0));
		break;

	case MazeBlockEntity::Destroyed:
		m_Model->Color(glm::vec4(0.6, 0.6, 0.6, 1.0));
		break;

	case MazeBlockEntity::WarpZone:
		m_Model->Color(glm::vec4(0.5, 0.5, 0.5, 1.0));
		m_ExtraModel01->Color(glm::vec4(0.1, 0.1, 0.3, 1.0));
		m_ExtraModel02->Color(glm::vec4(0.1, 0.1, 0.3, 1.0));
		break;
	}

	if (m_Model)
	{
		// Position the model.
		m_Model->Position(glm::vec3(m_X * 2.0, m_Level * 2.0, m_Y * 2.0));

		if (m_Type == MazeBlockEntity::Destroyed)
			m_Model->Translate(glm::vec3(0.f, -0.999f, 0.f));
		else if (m_Type == MazeBlockEntity::Empty)
			m_Model->Translate(glm::vec3(0.f, 0.999f, 0.f));
		else if (m_Type == MazeBlockEntity::WarpZone)
			m_Model->Translate(glm::vec3(0.f, -0.999f, 0.f));

		Managers::ModelManager::GetInstance()->RegisterModel(m_Model);
	}

	if (m_ExtraModel01)
	{
		m_ExtraModel01->Position(glm::vec3(m_X * 2.0, (m_Level * 2.0) - 1.0, m_Y * 2.0));
		Managers::ModelManager::GetInstance()->RegisterModel(m_ExtraModel01);
	}

	if (m_ExtraModel02)
	{
		m_ExtraModel02->Position(glm::vec3(m_X * 2.0, (m_Level * 2.0) - 1.0, m_Y * 2.0));
		Managers::ModelManager::GetInstance()->RegisterModel(m_ExtraModel02);
	}
}

void MazeBlockEntity::Update(double p_Delta)
{
	if (m_ExtraModel01)
		m_ExtraModel01->Rotate(8.0 * p_Delta, glm::vec3(0.f, 1.f, 0.f));

	if (m_ExtraModel02)
		m_ExtraModel02->Rotate(7.0 * p_Delta, glm::vec3(0.f, 1.f, 0.f));
}

void MazeBlockEntity::Transparent(bool p_Transparent)
{
	auto s_OriginalColor = m_Model->Color();
	m_Model->Color(glm::vec4(s_OriginalColor.x, s_OriginalColor.y, s_OriginalColor.z, p_Transparent ? 0.2 : 1.0));
}

void MazeBlockEntity::Destroy()
{
	if (m_Model)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(m_Model);
		delete m_Model;
	}

	if (m_ExtraModel01)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(m_ExtraModel01);
		delete m_ExtraModel01;
	}

	if (m_ExtraModel02)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(m_ExtraModel02);
		delete m_ExtraModel02;
	}

	m_ExtraModel01 = m_ExtraModel02 = nullptr;

	m_Type = MazeBlockEntity::Destroyed;

	m_Model = new Rendering::Objects::Model(Managers::ModelManager::GetInstance()->GetModelData("debris"),
		Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));

	m_Model->Color(glm::vec4(0.6, 0.6, 0.6, 1.0));
	m_Model->Position(glm::vec3(m_X * 2.0, m_Level * 2.0, m_Y * 2.0));
	m_Model->Translate(glm::vec3(0.f, -0.999f, 0.f));

	Managers::ModelManager::GetInstance()->RegisterModel(m_Model);
}