#pragma once

#include <Misc/StdAfx.h>

#include <SFML/Audio.hpp>
#include <queue>

namespace Rendering
{
	namespace Objects
	{
		class Light;
	}
}

namespace Game
{
	namespace Entities
	{
		class MazeEntity;
		class PlayerEntity;
		class CameraEntity;
		class ScoringEntity;
		class RotatingCameraEntity;
	}

	class Main
	{
	public:
		enum CameraMode
		{
			Overview,
			Orbit,
			FirstPerson
		};

	public:
		static Main* GetInstance();
		static void DestroyInstance();

	private:
		static Main* m_Instance;

	protected:
		Main();
		~Main();

	public:
		bool Init();
		void Update(double p_Delta);
		void Render(double p_Delta);
		void EndGame();

	public:
		inline Entities::MazeEntity* GetMaze() const { return m_Maze; }
		inline Entities::PlayerEntity* GetPlayer() const { return m_Player; }
		inline Entities::ScoringEntity* GetScoring() const { return m_Scoring; }

	protected:
		CameraMode m_CameraMode;
		CameraMode m_LastCameraMode;
		sf::Sound m_MainLoopAudio;
		bool m_GameEnded;

	protected:
		Entities::MazeEntity* m_Maze;
		Entities::PlayerEntity* m_Player;
		Entities::ScoringEntity* m_Scoring;
		Entities::RotatingCameraEntity* m_RotatingCamera;
		Entities::CameraEntity* m_OverviewCamera;
		Rendering::Objects::Light* m_GlobalLight;
	};
}