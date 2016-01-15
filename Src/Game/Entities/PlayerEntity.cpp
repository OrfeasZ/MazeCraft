#include "PlayerEntity.h"

#include <Game/Main.h>

#include <Game/Entities/MazeEntity.h>
#include <Game/Entities/ScoringEntity.h>
#include <Game/Entities/MazeBlockEntity.h>
#include <Game/Entities/MazeLevelEntity.h>

#include <Game/Objects/CylinderModel.h>

#include <Rendering/Objects/Light.h>

#include <Managers/ModelManager.h>
#include <Managers/InputManager.h>
#include <Managers/ShaderManager.h>
#include <Managers/LightManager.h>

#include "FPSCameraEntity.h"

using namespace Game::Entities;

PlayerEntity::PlayerEntity() :
	m_Camera(nullptr),
	m_PlayerModel(nullptr),
	m_PlayerLight(nullptr),
	m_HasControl(false),
	m_CurrentX(0),
	m_CurrentY(0),
	m_CurrentLevel(0),
	m_HammerCount(0)
{
}

PlayerEntity::~PlayerEntity()
{
	if (m_PlayerModel)
	{
		Managers::ModelManager::GetInstance()->RemoveModel(m_PlayerModel);
		delete m_PlayerModel;
	}

	m_PlayerModel = nullptr;

	if (m_PlayerLight)
	{
		Managers::LightManager::GetInstance()->RemoveLight(m_PlayerLight);
		delete m_PlayerLight;
	}

	if (m_Camera)
		delete m_Camera;

	m_PlayerModel = nullptr;
	m_PlayerLight = nullptr;
	m_Camera = nullptr;
}

void PlayerEntity::Init()
{
	// Create our player model.
	m_PlayerModel = new Game::Objects::CylinderModel(Managers::ShaderManager::GetInstance()->GetShaderProgram("ShadedModel"));
	m_PlayerModel->Color(glm::vec4(1.f, 1.f, 1.f, 1.f));
	m_PlayerModel->Position(glm::vec3(0.f, 0.f, 0.f));
	m_PlayerModel->ShouldRender(false);

	Managers::ModelManager::GetInstance()->RegisterModel(m_PlayerModel);

	// Create our player light.
	m_PlayerLight = new Rendering::Objects::Light();
	m_PlayerLight->Color(glm::vec3(1.f, 1.f, 1.f));
	m_PlayerLight->Position(glm::vec3(0.f, 0.f, 0.f));
	m_PlayerLight->Attenuation(1.1f);
	m_PlayerLight->AmbientCoefficient(0.01f);
	m_PlayerLight->Active(true);

	Managers::LightManager::GetInstance()->RegisterLight(m_PlayerLight);

	// Create the FPS camera.
	m_Camera = new FPSCameraEntity();
	m_Camera->Position(glm::vec3(0.f, 0.f, 0.f));
	m_Camera->Active(false);
}

void PlayerEntity::Update(double p_Delta)
{
	if (!m_Camera || !m_PlayerModel || !m_PlayerLight)
		return;

	// No updates if we don't have control of the player.
	if (!m_HasControl)
		return;

	glm::vec3 s_CurrentPosition = m_PlayerModel->Position();

	if (Managers::InputManager::GetInstance()->WentKeyboardKeyDown(GLFW_KEY_SPACE))
	{
		// Figure out where the player is looking at.
		float s_Angle = m_Camera->Angles().x;

		if (s_Angle <= 0.785398)
			s_Angle += 6.28319;

		int s_Direction = (s_Angle - 0.785398) / 1.5708;
		
		int s_TargetX = m_CurrentX;
		int s_TargetY = m_CurrentY;

		if (s_Direction == 0)
			++s_TargetX;
		else if (s_Direction == 1)
			--s_TargetY;
		else if (s_Direction == 2)
			--s_TargetX;
		else if (s_Direction == 3)
			++s_TargetY;

		// Check if we just won the game.
		if (m_CurrentLevel == Main::GetInstance()->GetMaze()->GetLevelCount() - 1)
		{
			Main::GetInstance()->EndGame();
			return;
		}

		// Otherwise default to normal logic.
		auto s_Level = Main::GetInstance()->GetMaze()->GetLevel(m_CurrentLevel);

		if (s_TargetX >= 0 && s_TargetX < s_Level->GetSize() && s_TargetY >= 0 && s_TargetY < s_Level->GetSize())
		{
			auto s_TopLevel = Main::GetInstance()->GetMaze()->GetLevel(m_CurrentLevel + 1);

			auto s_TopBlock = s_TopLevel->GetBlock(m_CurrentX, m_CurrentY);
			auto s_TargetTopBlock = s_TopLevel->GetBlock(s_TargetX, s_TargetY);
			auto s_TargetBlock = s_Level->GetBlock(s_TargetX, s_TargetY);

			// See if we can move there.
			if (s_TopBlock->Type() == MazeBlockEntity::Empty &&
				s_TargetBlock->Type() != MazeBlockEntity::Empty &&
				s_TargetBlock->Type() != MazeBlockEntity::Destroyed &&
				s_TargetBlock->Type() != MazeBlockEntity::WarpZone &&
				(s_TargetTopBlock->Type() == MazeBlockEntity::Empty ||
				s_TargetTopBlock->Type() == MazeBlockEntity::Destroyed ||
				s_TargetTopBlock->Type() == MazeBlockEntity::WarpZone))
			{
				if (!SetCoordinates(s_TargetX, s_TargetY, m_CurrentLevel + 1))
				{
					// Set the final corrected position.
					m_PlayerModel->Position(glm::vec3(s_TargetX * 2.f, (m_CurrentLevel * 2.f) - 1.f, s_TargetY * 2.f));

					// Update camera and light positions.
					m_Camera->Position(glm::vec3(m_PlayerModel->Position().x, m_PlayerModel->Position().y + 1.0, m_PlayerModel->Position().z));
					m_PlayerLight->Position(m_Camera->Position());
				}

				// TODO: Animation possibly.
			}
		}
	}

	// Hammer toss!
	if (Managers::InputManager::GetInstance()->WentKeyboardKeyDown(GLFW_KEY_H) && m_HammerCount > 0)
	{
		// Figure out where the player is looking at.
		float s_Angle = m_Camera->Angles().x;

		if (s_Angle <= 0.785398)
			s_Angle += 6.28319;

		int s_Direction = (s_Angle - 0.785398) / 1.5708;
		
		int s_TargetX = m_CurrentX;
		int s_TargetY = m_CurrentY;

		if (s_Direction == 0)
			++s_TargetX;
		else if (s_Direction == 1)
			--s_TargetY;
		else if (s_Direction == 2)
			--s_TargetX;
		else if (s_Direction == 3)
			++s_TargetY;

		auto s_Level = Main::GetInstance()->GetMaze()->GetLevel(m_CurrentLevel);

		if (s_TargetX >= 0 && s_TargetX < s_Level->GetSize() && s_TargetY >= 0 && s_TargetY < s_Level->GetSize())
		{
			auto s_TargetBlock = s_Level->GetBlock(s_TargetX, s_TargetY);

			// Can this block be destroyed?
			if (s_TargetBlock->Type() != MazeBlockEntity::Empty &&
				s_TargetBlock->Type() != MazeBlockEntity::Destroyed &&
				s_TargetBlock->Type() != MazeBlockEntity::WarpZone)
			{
				--m_HammerCount;
				Main::GetInstance()->GetScoring()->AddHammer();
				s_TargetBlock->Destroy();
			}
		}
	}

	bool s_DirtyMovement = false;

	if (Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_W) ||
		Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_W) ||
		Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_S) ||
		Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_S) ||
		Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_A) ||
		Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_A) ||
		Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_D) ||
		Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_D))
	{
		// Compute direction based on camera angles.
		auto s_Angles = m_Camera->Angles();

		glm::vec3 s_Direction(cos(s_Angles.y) * sin(s_Angles.x), sin(s_Angles.y), cos(s_Angles.y) * cos(s_Angles.x));
		glm::vec3 s_Right = glm::vec3(sin(s_Angles.x - glm::pi<float>() / 2.f), 0, cos(s_Angles.x - glm::pi<float>() / 2.f));

		if (Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_W) || Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_W))
		{
			m_PlayerModel->Translate(glm::vec3(p_Delta * s_Direction.x * 3.f, 0.f, p_Delta * s_Direction.z * 3.f));
			s_DirtyMovement = true;
		}

		if (Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_S) || Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_S))
		{
			m_PlayerModel->Translate(glm::vec3(p_Delta * s_Direction.x * -3.f, 0.f, p_Delta * s_Direction.z * -3.f));
			s_DirtyMovement = true;
		}

		if (Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_A) || Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_A))
		{
			m_PlayerModel->Translate(glm::vec3(p_Delta * s_Right.x * -3.f, 0.f, p_Delta * s_Right.z * -3.f));
			s_DirtyMovement = true;
		}

		if (Managers::InputManager::GetInstance()->IsKeyboardKeyPressed(GLFW_KEY_D) || Managers::InputManager::GetInstance()->IsKeyboardKeyRepeated(GLFW_KEY_D))
		{
			m_PlayerModel->Translate(glm::vec3(p_Delta * s_Right.x * 3.f, 0.f, p_Delta * s_Right.z * 3.f));
			s_DirtyMovement = true;
		}
	}
	
	// Did we attempt to move during this frame?
	if (s_DirtyMovement)
	{
		// Check if we can move to the requested position.
		glm::vec3 s_FinalPosition;
		int32_t s_FinalX;
		int32_t s_FinalY;
		int32_t s_Level;

		Game::Main::GetInstance()->GetMaze()->CanMove(s_CurrentPosition, m_PlayerModel->Position(), s_FinalPosition, s_FinalX, s_FinalY, s_Level);

		if (!SetCoordinates(s_FinalX, s_FinalY, s_Level))
		{
			// Set the final corrected position.
			m_PlayerModel->Position(s_FinalPosition);

			// Update camera and light positions.
			m_Camera->Position(glm::vec3(m_PlayerModel->Position().x, m_PlayerModel->Position().y + 1.0, m_PlayerModel->Position().z));
			m_PlayerLight->Position(m_Camera->Position());
		}
	}

	m_Camera->Update(p_Delta);
}

void PlayerEntity::HasControl(bool p_Control)
{
	m_Camera->Active(p_Control);
	m_PlayerModel->ShouldRender(!p_Control);
	m_PlayerLight->Active(p_Control);
	m_HasControl = p_Control;
}

void PlayerEntity::Position(glm::vec3 p_Position)
{
	m_PlayerModel->Position(p_Position);
	m_PlayerLight->Position(glm::vec3(p_Position.x, p_Position.y + 0.6, p_Position.z));
	m_Camera->Position(glm::vec3(p_Position.x, p_Position.y + 1.0, p_Position.z));
}

bool PlayerEntity::SetCoordinates(int32_t p_X, int32_t p_Y, int32_t p_Level, bool p_Initial /* = false */)
{
	int32_t s_LastX = m_CurrentX;
	int32_t s_LastY = m_CurrentY;
	int32_t s_LastLevel = m_CurrentLevel;

	m_CurrentX = p_X;
	m_CurrentY = p_Y;
	m_CurrentLevel = p_Level;

	if (p_Initial)
		return false;

	if (m_CurrentX == s_LastX && m_CurrentY == s_LastY && m_CurrentLevel == s_LastLevel)
		return false;

	// Add move to scoring.
	Main::GetInstance()->GetScoring()->AddMove();

	auto s_Level = Main::GetInstance()->GetMaze()->GetLevel(p_Level);
	auto s_Block = s_Level->GetBlock(p_X, p_Y);

	// Handle warp-zones.
	if (s_Block->Type() == MazeBlockEntity::WarpZone)
	{
		// Find other warp zone.
		MazeBlockEntity* s_TargetWarpBlock = nullptr;

		for (uint32_t i = 0; i < s_Level->GetSize() * s_Level->GetSize(); ++i)
		{
			if (s_Block == s_Level->GetBlock(i))
				continue;

			if (s_Level->GetBlock(i)->Type() != MazeBlockEntity::WarpZone)
				continue;

			s_TargetWarpBlock = s_Level->GetBlock(i);
			break;
		}

		if (s_TargetWarpBlock != nullptr)
		{
			m_CurrentX = s_TargetWarpBlock->X();
			m_CurrentY = s_TargetWarpBlock->Y();

			// Set the final corrected position.
			m_PlayerModel->Position(glm::vec3(m_CurrentX * 2.f, (m_CurrentLevel * 2.f) - 1.f, m_CurrentY * 2.f));

			// Update camera and light positions.
			m_Camera->Position(glm::vec3(m_PlayerModel->Position().x, m_PlayerModel->Position().y + 1.0, m_PlayerModel->Position().z));
			m_PlayerLight->Position(m_Camera->Position());

			return true;
		}
	}

	// Check if we can fall down.
	if (m_CurrentLevel > 0)
	{
		auto s_BottomLevel = Main::GetInstance()->GetMaze()->GetLevel(m_CurrentLevel - 1);
		auto s_BottomBlock = s_BottomLevel->GetBlock(m_CurrentX, m_CurrentY);

		if ((s_BottomBlock->Type() == MazeBlockEntity::Empty ||
			s_BottomBlock->Type() == MazeBlockEntity::Destroyed ||
			s_BottomBlock->Type() == MazeBlockEntity::WarpZone) &&
			s_Block->Type() == MazeBlockEntity::Empty)
		{
			// Fall down.
			--m_CurrentLevel;

			// Set the final corrected position.
			m_PlayerModel->Position(glm::vec3(m_CurrentX * 2.f, (m_CurrentLevel * 2.f) - 1.f, m_CurrentY * 2.f));

			// Update camera and light positions.
			m_Camera->Position(glm::vec3(m_PlayerModel->Position().x, m_PlayerModel->Position().y + 1.0, m_PlayerModel->Position().z));
			m_PlayerLight->Position(m_Camera->Position());

			return true;
		}		
	}

	return false;
}