#include "Main.h"

#include <Application.h>

#include <Managers/ModelManager.h>
#include <Managers/InputManager.h>
#include <Managers/TextureManager.h>
#include <Managers/AudioManager.h>
#include <Managers/LightManager.h>

#include <Rendering/UIRenderer.h>
#include <Rendering/Objects/Light.h>

#include <Game/Entities/MazeEntity.h>
#include <Game/Entities/PlayerEntity.h>
#include <Game/Entities/ScoringEntity.h>
#include <Game/Entities/MazeLevelEntity.h>
#include <Game/Entities/MazeBlockEntity.h>
#include <Game/Entities/FPSCameraEntity.h>
#include <Game/Entities/RotatingCameraEntity.h>

using namespace Game;

Main* Main::m_Instance = nullptr;

Main* Main::GetInstance()
{
	if (!m_Instance)
		m_Instance = new Main();

	return m_Instance;
}

void Main::DestroyInstance()
{
	if (!m_Instance)
		return;

	delete m_Instance;
	m_Instance = nullptr;
}

Main::Main() :
	m_CameraMode(Main::Overview),
	m_LastCameraMode(Main::Overview),
	m_GameEnded(false),
	m_Maze(nullptr),
	m_Player(nullptr),
	m_Scoring(nullptr),
	m_RotatingCamera(nullptr),
	m_OverviewCamera(nullptr),
	m_GlobalLight(nullptr)
{
}

Main::~Main()
{
	m_MainLoopAudio.stop();
}

bool Main::Init()
{
	glfwSetInputMode(Application::GetInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!Managers::ModelManager::GetInstance()->PrecacheModel("hatch"))
		return false;

	if (!Managers::ModelManager::GetInstance()->PrecacheModel("debris"))
		return false;

	if (!Managers::ModelManager::GetInstance()->PrecacheModel("frame"))
		return false;

	if (!Managers::ModelManager::GetInstance()->PrecacheModel("warp-base"))
		return false;

	if (!Managers::ModelManager::GetInstance()->PrecacheModel("warp-ring"))
		return false;

	if (!Managers::ModelManager::GetInstance()->PrecacheModel("floor"))
		return false;

	if (!Managers::TextureManager::GetInstance()->PrecachePNGTexture("T1"))
		return false;

	if (!Managers::TextureManager::GetInstance()->PrecachePNGTexture("T2"))
		return false;

	if (!Managers::TextureManager::GetInstance()->PrecachePNGTexture("T3"))
		return false;

	m_Maze = new Entities::MazeEntity("file");
	m_Maze->Init();

	if (m_Maze->GetLevelCount() == 0)
	{
		Logger(Util::LogLevel::Error, "No levels found in maze. Make sure your data is valid.");
		return false;
	}

	m_Player = new Entities::PlayerEntity();
	m_Player->Init();
	m_Player->HasControl(false);

	m_Scoring = new Entities::ScoringEntity(m_Maze->GetLevel(0)->GetSize());
	m_Scoring->Init();

	m_RotatingCamera = new Entities::RotatingCameraEntity();
	m_RotatingCamera->Init();
	m_RotatingCamera->Active(false);
	m_RotatingCamera->OrbitPosition(glm::vec3(m_Maze->GetLevel(0)->GetSize() - 1, m_Maze->GetLevelCount() - 1, m_Maze->GetLevel(0)->GetSize() - 1));
	m_RotatingCamera->RotationSpeed(0.35);
	m_RotatingCamera->OrbitAngle(-0.75);
	m_RotatingCamera->OrbitDistance(m_Maze->GetLevel(0)->GetSize() * 3.5);

	m_OverviewCamera = new Entities::CameraEntity();
	m_OverviewCamera->Init();
	m_OverviewCamera->Active(true);
	m_OverviewCamera->Position(glm::vec3(m_Maze->GetLevel(0)->GetSize() - 1, m_Maze->GetLevel(0)->GetSize() * 3.5, m_Maze->GetLevel(0)->GetSize() - 1));
	m_OverviewCamera->Angles(glm::vec2(0.0, -1.56));

	m_GlobalLight = new Rendering::Objects::Light();
	m_GlobalLight->Color(glm::vec3(0.8, 0.8, 0.8));
	m_GlobalLight->AmbientCoefficient(0.1f);
	m_GlobalLight->Position(glm::vec3(m_Maze->GetLevel(0)->GetSize() - 1, m_Maze->GetLevelCount() + 25.f, m_Maze->GetLevel(0)->GetSize() - 1));
	m_GlobalLight->Active(true);

	Managers::LightManager::GetInstance()->RegisterLight(m_GlobalLight);

	// Find a random empty block to put the player in.
	glm::vec3 s_StartingPosition;
	int32_t s_X;
	int32_t s_Y;

	if (!m_Maze->GetLevel(0)->GetRandomEmptyPosition(s_StartingPosition, s_X, s_Y))
	{
		Logger(Util::LogLevel::Error, "Failed to find starting position for player. This probably means there's no empty blocks.");
		return false;
	}

	m_Player->HammerCount(m_Maze->InitialHammers());
	m_Player->Position(s_StartingPosition);
	m_Player->SetCoordinates(s_X, s_Y, 0, true);

	// Make upwards levels transparent.
	for (size_t i = 1; i < m_Maze->GetLevelCount(); ++i)
	{
		auto s_Level = m_Maze->GetLevel(i);

		for (uint32_t j = 0; j < s_Level->GetSize() * s_Level->GetSize(); ++j)
			if (s_Level->GetBlock(j))
				s_Level->GetBlock(j)->Transparent(true);
	}

	return true;
}

void Main::Update(double p_Delta)
{
	// User pressed X; exit the application.
	if (Managers::InputManager::GetInstance()->WentKeyboardKeyDown(GLFW_KEY_X))
	{
		glfwSetWindowShouldClose(Application::GetInstance()->GetWindow(), GL_TRUE);
		return;
	}

	// User pressed E; end the game.
	if (Managers::InputManager::GetInstance()->WentKeyboardKeyDown(GLFW_KEY_E))
		EndGame();

	if (Managers::InputManager::GetInstance()->WentKeyboardKeyDown(GLFW_KEY_R) && !m_GameEnded)
	{
		m_Maze->SetWallVisibility(false);

		if (m_CameraMode == Main::Overview)
		{
			// Activate orbit camera.
			m_OverviewCamera->Active(false);
			m_RotatingCamera->Active(true);
			m_CameraMode = Main::Orbit;
		}
		else if (m_CameraMode == Main::Orbit)
		{
			// Activate overview camera.
			m_OverviewCamera->Active(true);
			m_RotatingCamera->Active(false);
			m_CameraMode = Main::Overview;
		}
	}

	if (Managers::InputManager::GetInstance()->WentKeyboardKeyDown(GLFW_KEY_V) && !m_GameEnded)
	{
		if (m_CameraMode == Main::FirstPerson)
		{
			// Re-enable global light and disable player controls.
			m_GlobalLight->Active(true);
			m_Player->HasControl(false);
			
			// Disable wall rendering.
			m_Maze->SetWallVisibility(false);

			if (m_LastCameraMode == Main::Orbit)
			{
				// Activate orbit camera.
				m_OverviewCamera->Active(false);
				m_RotatingCamera->Active(true);
				m_CameraMode = Main::Orbit;
			}
			else if (m_LastCameraMode == Main::Overview)
			{
				// Activate overview camera.
				m_OverviewCamera->Active(true);
				m_RotatingCamera->Active(false);
				m_CameraMode = Main::Overview;
			}

			// Make upwards levels transparent.
			for (size_t i = m_Player->CurrentLevel() + 1; i < m_Maze->GetLevelCount(); ++i)
			{
				auto s_Level = m_Maze->GetLevel(i);

				for (uint32_t j = 0; j < s_Level->GetSize() * s_Level->GetSize(); ++j)
					if (s_Level->GetBlock(j))
						s_Level->GetBlock(j)->Transparent(true);
			}
		}
		else
		{
			// Activate first-person camera and controls.
			m_OverviewCamera->Active(false);
			m_RotatingCamera->Active(false);
			m_GlobalLight->Active(false);
			m_Player->HasControl(true);

			// Enable wall rendering.
			m_Maze->SetWallVisibility(true);

			m_LastCameraMode = m_CameraMode;
			m_CameraMode = Main::FirstPerson;

			// Make everything opaque again.
			for (size_t i = 0; i < m_Maze->GetLevelCount(); ++i)
			{
				auto s_Level = m_Maze->GetLevel(i);

				for (uint32_t j = 0; j < s_Level->GetSize() * s_Level->GetSize(); ++j)
					if (s_Level->GetBlock(j))
						s_Level->GetBlock(j)->Transparent(false);
			}
		}
	}

	if (!m_GameEnded)
	{
		m_Maze->Update(p_Delta);
		m_Player->Update(p_Delta);
		m_Scoring->Update(p_Delta);
		m_OverviewCamera->Update(p_Delta);
	}

	m_RotatingCamera->Update(p_Delta);
}

void Main::Render(double p_Delta)
{
	if (!m_Scoring || !m_Player)
		return;

	float s_Size = Application::GetInstance()->WindowHeight() * 1.1461538f;
	float s_TextSize = ((s_Size - 250.f) / 350.f) / 10.f;
	float s_FinalTextSize = ((s_Size - 250.f) / 350.f) / 5.f;

	if (!m_GameEnded)
	{
		float s_TextLeftOffset = (Application::GetInstance()->WindowWidth() / 2.f) + ((Application::GetInstance()->WindowWidth() / 2.f) * 0.6f);

		float s_ScoreTopOffset = ((Application::GetInstance()->WindowHeight() - s_Size) / 2.f) + (s_Size * 0.1f);
		float s_HammersTopOffset = ((Application::GetInstance()->WindowHeight() - s_Size) / 2.f) + (s_Size * 0.125f);

		char s_Score[128];
		sprintf(s_Score, "Score: %lld", m_Scoring->GetScore());

		char s_Hammers[128];
		sprintf(s_Hammers, "Hammers: %u", m_Player->HammerCount());

		Rendering::UIRenderer::GetInstance()->RenderText(s_Score, s_TextLeftOffset, s_ScoreTopOffset, s_TextSize, glm::vec3(1.0, 1.0, 1.0));
		Rendering::UIRenderer::GetInstance()->RenderText(s_Hammers, s_TextLeftOffset, s_HammersTopOffset, s_TextSize, glm::vec3(1.0, 1.0, 1.0));

		return;
	}

	float s_FinalTextLeftOffset = (Application::GetInstance()->WindowWidth() / 2.f) - ((Application::GetInstance()->WindowWidth() / 2.f) * 0.26f);
	float s_FinalTextTopOffset = ((Application::GetInstance()->WindowHeight() - s_Size) / 2.f) + (s_Size * 0.135f);

	char s_FinalScore[128];
	sprintf(s_FinalScore, "Final Score: %lld", m_Scoring->GetScore());

	Rendering::UIRenderer::GetInstance()->RenderText(s_FinalScore, s_FinalTextLeftOffset, s_FinalTextTopOffset, s_FinalTextSize, glm::vec3(1.0, 1.0, 1.0));
}

void Main::EndGame()
{
	if (m_GameEnded)
		return;

	m_GameEnded = true;

	// Re-enable global light and disable player controls.
	m_GlobalLight->Active(true);
	m_Player->HasControl(false);

	// Activate orbit camera.
	m_OverviewCamera->Active(false);
	m_RotatingCamera->Active(true);
	m_CameraMode = Main::Orbit;

	// Make upwards levels transparent.
	for (size_t i = m_Player->CurrentLevel() + 1; i < m_Maze->GetLevelCount(); ++i)
	{
		auto s_Level = m_Maze->GetLevel(i);

		for (uint32_t j = 0; j < s_Level->GetSize() * s_Level->GetSize(); ++j)
			if (s_Level->GetBlock(j))
				s_Level->GetBlock(j)->Transparent(true);
	}
}