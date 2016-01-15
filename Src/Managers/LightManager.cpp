#include "LightManager.h"

using namespace Managers;

LightManager* LightManager::m_Instance = nullptr;

LightManager* LightManager::GetInstance()
{
	if (!m_Instance)
		m_Instance = new LightManager();

	return m_Instance;
}

void LightManager::DestroyInstance()
{
	if (!m_Instance)
		return;

	delete m_Instance;
	m_Instance = nullptr;
}

//////////////////////////////////////////////////////////////////////////

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
	ClearLights();
}

bool LightManager::Init()
{
	return true;
}

bool LightManager::RegisterLight(Rendering::Objects::Light* p_Light)
{
	if (m_Lights.find(p_Light) != m_Lights.end())
		return true;

	// Limit max lights to 5.
	if (m_Lights.size() >= 5)
		return false;

	m_Lights.insert(p_Light);
	return true;
}

void LightManager::RemoveLight(Rendering::Objects::Light* p_Light)
{
	m_Lights.erase(p_Light);
}

void LightManager::ClearLights()
{
	m_Lights.clear();
}