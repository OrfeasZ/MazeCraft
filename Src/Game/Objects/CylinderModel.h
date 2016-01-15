#pragma once

#include <Rendering/Objects/Model.h>

namespace Game
{
	namespace Objects
	{
		class CylinderModel :
			public Rendering::Objects::Model
		{
		public:
			CylinderModel(Managers::Shader::Program* p_ShaderProgram);
		};
	}
}