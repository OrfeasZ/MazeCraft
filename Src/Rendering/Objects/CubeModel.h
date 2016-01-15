#pragma once

#include "Model.h"

namespace Rendering
{
	namespace Objects
	{
		class CubeModel :
			public Model
		{
		public:
			CubeModel(Managers::Shader::Program* p_ShaderProgram);
		};
	}
}