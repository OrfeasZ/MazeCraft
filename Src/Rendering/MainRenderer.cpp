#include "MainRenderer.h"

#include <Application.h>

#include <Managers/ModelManager.h>
#include <Managers/InputManager.h>
#include <Managers/CameraManager.h>
#include <Managers/ShaderManager.h>
#include <Managers/TextureManager.h>
#include <Managers/LightManager.h>

#include <Rendering/Objects/Model.h>
#include <Rendering/Objects/Light.h>

#include <Rendering/Textures/Texture.h>
#include <Rendering/Textures/CharacterTexture.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace Rendering;

MainRenderer* MainRenderer::m_Instance = nullptr;

MainRenderer* MainRenderer::GetInstance()
{
	if (!m_Instance)
		m_Instance = new MainRenderer();

	return m_Instance;
}

void MainRenderer::DestroyInstance()
{
	if (!m_Instance)
		return;

	delete m_Instance;
	m_Instance = nullptr;
}

//////////////////////////////////////////////////////////////////////////

MainRenderer::MainRenderer()
{
}

MainRenderer::~MainRenderer()
{
}

bool MainRenderer::Init()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(Managers::CameraManager::GetInstance()->FOV()),
		(float) Application::GetInstance()->WindowWidth() / (float) Application::GetInstance()->WindowHeight(), 
		0.1f, 1000.f);

	m_OrthoProjection = glm::ortho(0.f, (float) Application::GetInstance()->WindowWidth(), (float) Application::GetInstance()->WindowHeight(), 0.f);

	m_ViewMatrix = glm::lookAt(
		Managers::CameraManager::GetInstance()->EyePosition(),
		Managers::CameraManager::GetInstance()->LookAtPosition(),
		glm::vec3(0, 1, 0)
	);

	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	return true;
}

void MainRenderer::Update(double p_Delta)
{
}

glm::vec3 MainRenderer::ScreenToWorld(int p_X, int p_Y)
{
	auto s_ViewProjection = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);

	auto s_WorldCoordinates = s_ViewProjection * glm::vec4(
		2.0 * Managers::InputManager::GetInstance()->GetCursorX() / Application::GetInstance()->WindowWidth() - 1, 
		-2.0 * Managers::InputManager::GetInstance()->GetCursorY() / Application::GetInstance()->WindowHeight() + 1, 
		0.0,
		1.0
	);

	s_WorldCoordinates *= Managers::CameraManager::GetInstance()->EyePosition().y;

	return glm::vec3(s_WorldCoordinates.x, s_WorldCoordinates.y, s_WorldCoordinates.z);
}

void MainRenderer::Render(double p_Delta)
{
	if (Managers::CameraManager::GetInstance()->Dirty())
	{
		Managers::CameraManager::GetInstance()->Dirty(false);

		m_ProjectionMatrix = glm::perspective(glm::radians(Managers::CameraManager::GetInstance()->FOV()),
			(float) Application::GetInstance()->WindowWidth() / (float) Application::GetInstance()->WindowHeight(),
			0.1f, 1000.f);

		m_OrthoProjection = glm::ortho(0.f, (float) Application::GetInstance()->WindowWidth(), (float) Application::GetInstance()->WindowHeight(), 0.f);

		m_ViewMatrix = glm::lookAt(
			Managers::CameraManager::GetInstance()->EyePosition(),
			Managers::CameraManager::GetInstance()->LookAtPosition(),
			glm::vec3(0, 1, 0)
		);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.25f, 0.7f, 1.f, 1.f);
	RenderModels();
}

void MainRenderer::RenderModels()
{
	auto s_Models = Managers::ModelManager::GetInstance()->GetModels();

	// Models are grouped by the shader program they use.
	// Iterate the groups, start using the program, and render every model in the group.
	for (auto s_ModelGroup : s_Models)
	{
		if (s_ModelGroup.second.size() == 0)
			continue;

		auto s_Program = (*s_ModelGroup.second.begin())->ShaderProgram();

		glUseProgram(s_Program->GetProgram());

		GLint s_ProjectionMatrixLocation = s_Program->GetUniformLocation("p");
		GLint s_ViewMatrixLocation = s_Program->GetUniformLocation("v");
		GLint s_ModelMatrixLocation = s_Program->GetUniformLocation("m");
		GLint s_ColorVectorLocation = s_Program->GetUniformLocation("c");
		GLint s_TextureLocation = s_Program->GetUniformLocation("t");		
		GLint s_CameraPositionLocation = s_Program->GetUniformLocation("cp");
		GLint s_LightCountLocation = s_Program->GetUniformLocation("lc");
		GLint s_MaterialShininessLocation = s_Program->GetUniformLocation("ms");

		GLint s_ModelVerticesLocation = s_Program->GetAttributeLocation("mv");
		GLint s_ModelUVsLocation = s_Program->GetAttributeLocation("mu");
		GLint s_ModelNormalsLocation = s_Program->GetAttributeLocation("mn");

		for (auto s_Model : s_ModelGroup.second)
		{
			if (!s_Model->ShouldRender())
				continue;

			// Set all the matrices.
			if (s_ProjectionMatrixLocation != -1)
				glUniformMatrix4fv(s_ProjectionMatrixLocation, 1, GL_FALSE, &m_ProjectionMatrix[0][0]);

			if (s_ViewMatrixLocation != -1)
				glUniformMatrix4fv(s_ViewMatrixLocation, 1, GL_FALSE, &m_ViewMatrix[0][0]);

			if (s_ModelMatrixLocation != -1)
				glUniformMatrix4fv(s_ModelMatrixLocation, 1, GL_FALSE, &s_Model->ModelMatrix()[0][0]);

			// Set the model color.
			if (s_ColorVectorLocation != -1)
				glUniform4f(s_ColorVectorLocation, s_Model->Color().x, s_Model->Color().y, s_Model->Color().z, s_Model->Color().w);

			// Set the camera position.
			if (s_CameraPositionLocation != -1)
				glUniform3f(s_CameraPositionLocation, Managers::CameraManager::GetInstance()->EyePosition().x, Managers::CameraManager::GetInstance()->EyePosition().y, Managers::CameraManager::GetInstance()->EyePosition().z);

			// Set the material shininess.
			if (s_MaterialShininessLocation != -1)
				glUniform1f(s_MaterialShininessLocation, s_Model->Shininess());
			
			// Set the model texture.
			if (s_TextureLocation != -1 && s_Model->Texture() != nullptr)
			{
				glBindTexture(GL_TEXTURE_2D, s_Model->Texture()->GetGLTexture());
				glUniform1i(s_TextureLocation, 0);
			}

			// Light sources.
			if (s_LightCountLocation != -1)
			{
				int s_LightCount = 0;

				for (auto s_Light : Managers::LightManager::GetInstance()->Lights())
				{
					if (!s_Light->Active())
						continue;

					char s_LightAccessor[12];
					sprintf(s_LightAccessor, "l[%d]", s_LightCount);

					GLint s_LightPositionLocation = s_Program->GetUniformLocation(std::string(s_LightAccessor) + ".p");
					GLint s_LightColorLocation = s_Program->GetUniformLocation(std::string(s_LightAccessor) + ".c");
					GLint s_LightAttenuationLocation = s_Program->GetUniformLocation(std::string(s_LightAccessor) + ".a");
					GLint s_LightAmbientCoefficientLocation = s_Program->GetUniformLocation(std::string(s_LightAccessor) + ".ac");

					if (s_LightPositionLocation != -1)
						glUniform3f(s_LightPositionLocation, s_Light->Position().x, s_Light->Position().y, s_Light->Position().z);

					if (s_LightColorLocation != -1)
						glUniform3f(s_LightColorLocation, s_Light->Color().x, s_Light->Color().y, s_Light->Color().z);
					
					if (s_LightAttenuationLocation != -1)
						glUniform1f(s_LightAttenuationLocation, s_Light->Attenuation());

					if (s_LightAmbientCoefficientLocation != -1)
						glUniform1f(s_LightAmbientCoefficientLocation, s_Light->AmbientCoefficient());

					++s_LightCount;
				}
				
				glUniform1i(s_LightCountLocation, s_LightCount);
			}

			// Set Vertices.
			glEnableVertexAttribArray(s_ModelVerticesLocation);
			glBindBuffer(GL_ARRAY_BUFFER, s_Model->VertexBuffer());
			glVertexAttribPointer(s_ModelVerticesLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			// Set UVs.
			glEnableVertexAttribArray(s_ModelUVsLocation);
			glBindBuffer(GL_ARRAY_BUFFER, s_Model->UVBuffer());
			glVertexAttribPointer(s_ModelUVsLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

			// Set Normals.
			glEnableVertexAttribArray(s_ModelNormalsLocation);
			glBindBuffer(GL_ARRAY_BUFFER, s_Model->NormalBuffer());
			glVertexAttribPointer(s_ModelNormalsLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			// Set Indices.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Model->IndexBuffer());

			glEnable(GL_DEPTH_TEST);

			if (s_Model->Color().w < 1.0)
			{
				glEnable(GL_BLEND);
				glDepthMask(GL_FALSE);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			// Draw all our triangles.
			glDrawElements(GL_TRIANGLES, (GLsizei) s_Model->IndexCount(), GL_UNSIGNED_SHORT, nullptr);
			
			if (s_Model->Color().w < 1.0)
			{
				glDepthMask(GL_TRUE);
				glDisable(GL_BLEND);
			}

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			if (s_TextureLocation != -1 && s_Model->Texture() != nullptr)
				glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void MainRenderer::OnResize(int p_Width, int p_Height)
{
	m_ProjectionMatrix = glm::perspective(glm::radians(Managers::CameraManager::GetInstance()->FOV()),
		(float) p_Width / (float) p_Height,
		0.1f, 1000.f);

	m_OrthoProjection = glm::ortho(0.f, (float) p_Width, (float) p_Height, 0.f);
}
