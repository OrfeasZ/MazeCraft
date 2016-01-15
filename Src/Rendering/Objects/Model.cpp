#include "Model.h"

using namespace Rendering::Objects;

Model::Model(Managers::Model::ModelData* p_Data, Managers::Shader::Program* p_ShaderProgram) :
	m_ShaderProgram(p_ShaderProgram),
	m_ModelMatrix(1.0),
	m_Render(true),
	m_ModelColor(0.f, 1.f, 0.f, 1.f),
	m_Texture(nullptr),
	m_Shininess(0.f)
{
	ParseModelData(p_Data);
}

Model::Model(Managers::Shader::Program* p_ShaderProgram) :
	m_ShaderProgram(p_ShaderProgram),
	m_ModelMatrix(1.0),
	m_Render(true),
	m_ModelColor(0.f, 1.f, 0.f, 1.f),
	m_Texture(nullptr),
	m_Shininess(0.f)
{

}

Model::~Model()
{

}

void Model::ParseModelData(Managers::Model::ModelData* p_Data)
{
	m_Vertices = p_Data->Vertices;
	m_Normals = p_Data->Normals;
	m_UVs = p_Data->UVs;
	m_Indices = p_Data->Indices;

	// Generate buffers and bind data.
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), &m_Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_Normals.size() * sizeof(glm::vec3), &m_Normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_UVs.size() * sizeof(glm::vec2), &m_UVs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint16_t), &m_Indices[0], GL_STATIC_DRAW);
}
