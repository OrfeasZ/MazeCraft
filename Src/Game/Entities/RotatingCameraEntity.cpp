#include "RotatingCameraEntity.h"

#include <Managers/CameraManager.h>

using namespace Game::Entities;

RotatingCameraEntity::RotatingCameraEntity() :
	m_OrbitPosition(0, 0, 0),
	m_OrbitDistance(0.0),
	m_OrbitAngle(0.0),
	m_RotationAngle(0.0),
	m_RotationSpeed(10.0)
{
}

void RotatingCameraEntity::Init()
{
	CameraEntity::Init();
}

void RotatingCameraEntity::Update(double p_Delta)
{
	if (!m_Active)
		return;

	m_RotationAngle += m_RotationSpeed * p_Delta;

	double cosfi = cos(m_RotationAngle);
	double sinfi = sin(m_RotationAngle);

	double costheta = cos(m_OrbitAngle);
	double sintheta = sin(m_OrbitAngle);

	double cx = m_OrbitPosition.x + (m_OrbitDistance * sintheta * cosfi);
	double cy = m_OrbitPosition.y + (m_OrbitDistance * costheta);
	double cz = m_OrbitPosition.z + (m_OrbitDistance * sintheta * sinfi);

	Managers::CameraManager::GetInstance()->EyePosition(glm::vec3(cx, cy, cz));
	Managers::CameraManager::GetInstance()->LookAtPosition(m_OrbitPosition);
	Managers::CameraManager::GetInstance()->FOV(m_FOV);
	Managers::CameraManager::GetInstance()->Dirty(true);
}