#pragma once

#include <Misc/StdAfx.h>
#include "IManager.h"

#include <unordered_set>

namespace Rendering
{
	namespace Objects
	{
		class Light;
	}
}

namespace Managers
{
	class LightManager : public IManager
	{
	public:
		static LightManager* GetInstance();
		static void DestroyInstance();

	private:
		static LightManager* m_Instance;

	protected:
		LightManager();
		~LightManager();

	public:
		virtual bool Init() override;

	public:
		bool RegisterLight(Rendering::Objects::Light* p_Light);
		void RemoveLight(Rendering::Objects::Light* p_Light);
		void ClearLights();

	public:
		inline std::unordered_set<Rendering::Objects::Light*> Lights() const { return m_Lights; }

	protected:
		std::unordered_set<Rendering::Objects::Light*> m_Lights;
		
	};
}