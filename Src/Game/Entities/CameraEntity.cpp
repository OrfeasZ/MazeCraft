#include "CameraEntity.h"

#include <Managers/CameraManager.h>

#include <glm/gtc/constants.hpp>

using namespace Game::Entities;

CameraEntity::CameraEntity() :
	m_Active(false),
	m_Position(0.0, 0.0, 0.0),
	m_Angles(glm::pi<float>(), 0.f),
	m_FOV(60.f),
	m_Dirty(false)
{

}

CameraEntity::~CameraEntity()
{

}

void CameraEntity::Init()
{
	m_Dirty = true;
}

void CameraEntity::Update(double p_Delta)
{
	if (!m_Active)
		return;

	if (!m_Dirty)
		return;

	m_Dirty = false;

	glm::vec3 s_Direction(cos(m_Angles.y) * sin(m_Angles.x), sin(m_Angles.y), cos(m_Angles.y) * cos(m_Angles.x));
	glm::vec3 s_Right = glm::vec3(sin(m_Angles.x - glm::pi<float>() / 2.f), 0, cos(m_Angles.x - glm::pi<float>() / 2.f));
	glm::vec3 s_Up = glm::cross(s_Right, s_Direction);

	Managers::CameraManager::GetInstance()->EyePosition(m_Position);
	Managers::CameraManager::GetInstance()->LookAtPosition(m_Position + s_Direction);
	Managers::CameraManager::GetInstance()->FOV(m_FOV);
	Managers::CameraManager::GetInstance()->Dirty(true);
}