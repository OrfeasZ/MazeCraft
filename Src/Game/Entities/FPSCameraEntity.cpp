#include "FPSCameraEntity.h"

#include <Application.h>
#include <Managers/InputManager.h>

using namespace Game::Entities;

FPSCameraEntity::FPSCameraEntity() :
	m_LastMouseX(0.0),
	m_LastMouseY(0.0)
{
}

void FPSCameraEntity::Init()
{
	CameraEntity::Init();

	m_LastMouseX = Managers::InputManager::GetInstance()->GetCursorX();
	m_LastMouseY = Managers::InputManager::GetInstance()->GetCursorY();
}

void FPSCameraEntity::Update(double p_Delta)
{
	if (!m_Active)
		return;

	double s_CursorX = Managers::InputManager::GetInstance()->GetCursorX();
	double s_CursorY = Managers::InputManager::GetInstance()->GetCursorY();

	float s_LastHorizontalAngle = m_Angles.x;
	float s_LastVerticalAngle = m_Angles.y;

	if (m_LastMouseX != s_CursorX)
	{
		m_Angles.x += 0.05f * p_Delta * (m_LastMouseX - s_CursorX);
		m_LastMouseX = s_CursorX;
	}

	if (m_LastMouseY != s_CursorY)
	{
		m_Angles.y += 0.05f * p_Delta * (m_LastMouseY - s_CursorY);
		m_LastMouseY = s_CursorY;
	}

	if (m_Angles.y > 1.56)
		m_Angles.y = 1.56;

	if (m_Angles.y < -1.56)
		m_Angles.y = -1.56;

	if (m_Angles.x > 6.28319)
		m_Angles.x = 0;

	if (m_Angles.x < 0)
		m_Angles.x = 6.28319;

	if (s_LastHorizontalAngle != m_Angles.x ||
		s_LastVerticalAngle != m_Angles.y)
		m_Dirty = true;

	CameraEntity::Update(p_Delta);
}