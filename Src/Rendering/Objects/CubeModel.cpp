#include "CubeModel.h"

using namespace Rendering::Objects;

CubeModel::CubeModel(Managers::Shader::Program* p_ShaderProgram) :
	Model(p_ShaderProgram)
{
	m_Vertices.push_back(glm::vec3(1.000000, -1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, -1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, -1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, 1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, 1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, 1.000000, 1.000001));
	m_Vertices.push_back(glm::vec3(1.000000, 1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, 1.000000, 1.000001));
	m_Vertices.push_back(glm::vec3(1.000000, -1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, 1.000000, 1.000001));
	m_Vertices.push_back(glm::vec3(-1.000000, 1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, -1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, -1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, 1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, 1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, -1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, -1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, 1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, -1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, 1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, -1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, -1.000000, 1.000000));
	m_Vertices.push_back(glm::vec3(-1.000000, -1.000000, -1.000000));
	m_Vertices.push_back(glm::vec3(1.000000, 1.000000, -1.000000));

	m_Normals.push_back(glm::vec3(0.000000, -1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(0.000000, -1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(0.000000, -1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(0.000000, 1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(0.000000, 1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(0.000000, 1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(1.000000, 0.000000, 0.000000));
	m_Normals.push_back(glm::vec3(1.000000, 0.000000, 0.000000));
	m_Normals.push_back(glm::vec3(1.000000, 0.000000, 0.000000));
	m_Normals.push_back(glm::vec3(-0.000000, 0.000000, 1.000000));
	m_Normals.push_back(glm::vec3(-0.000000, 0.000000, 1.000000));
	m_Normals.push_back(glm::vec3(-0.000000, 0.000000, 1.000000));
	m_Normals.push_back(glm::vec3(-1.000000, -0.000000, -0.000000));
	m_Normals.push_back(glm::vec3(-1.000000, -0.000000, -0.000000));
	m_Normals.push_back(glm::vec3(-1.000000, -0.000000, -0.000000));
	m_Normals.push_back(glm::vec3(0.000000, 0.000000, -1.000000));
	m_Normals.push_back(glm::vec3(0.000000, 0.000000, -1.000000));
	m_Normals.push_back(glm::vec3(0.000000, 0.000000, -1.000000));
	m_Normals.push_back(glm::vec3(0.000000, -1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(0.000000, 1.000000, 0.000000));
	m_Normals.push_back(glm::vec3(1.000000, 0.000000, 0.000000));
	m_Normals.push_back(glm::vec3(-0.000000, 0.000000, 1.000000));
	m_Normals.push_back(glm::vec3(-1.000000, -0.000000, -0.000000));
	m_Normals.push_back(glm::vec3(0.000000, 0.000000, -1.000000));

	m_UVs.push_back(glm::vec2(1.000000, -1.000000));
	m_UVs.push_back(glm::vec2(-0.000000, -1.000000));
	m_UVs.push_back(glm::vec2(0.000000, 0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -1.000000));
	m_UVs.push_back(glm::vec2(-0.000000, -1.000000));
	m_UVs.push_back(glm::vec2(0.000000, 0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -1.000000));
	m_UVs.push_back(glm::vec2(-0.000000, -1.000000));
	m_UVs.push_back(glm::vec2(0.000000, 0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -1.000000));
	m_UVs.push_back(glm::vec2(-0.000000, -1.000000));
	m_UVs.push_back(glm::vec2(0.000000, 0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -1.000000));
	m_UVs.push_back(glm::vec2(-0.000000, -1.000000));
	m_UVs.push_back(glm::vec2(1.000000, -1.000000));
	m_UVs.push_back(glm::vec2(-0.000000, -1.000000));
	m_UVs.push_back(glm::vec2(0.000000, 0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -0.000000));
	m_UVs.push_back(glm::vec2(0.000000, 0.000000));
	m_UVs.push_back(glm::vec2(1.000000, -0.000000));

	m_Indices.push_back(0);
	m_Indices.push_back(1);
	m_Indices.push_back(2);
	m_Indices.push_back(3);
	m_Indices.push_back(4);
	m_Indices.push_back(5);
	m_Indices.push_back(6);
	m_Indices.push_back(7);
	m_Indices.push_back(8);
	m_Indices.push_back(9);
	m_Indices.push_back(10);
	m_Indices.push_back(11);
	m_Indices.push_back(12);
	m_Indices.push_back(13);
	m_Indices.push_back(14);
	m_Indices.push_back(15);
	m_Indices.push_back(16);
	m_Indices.push_back(17);
	m_Indices.push_back(18);
	m_Indices.push_back(0);
	m_Indices.push_back(2);
	m_Indices.push_back(19);
	m_Indices.push_back(3);
	m_Indices.push_back(5);
	m_Indices.push_back(20);
	m_Indices.push_back(6);
	m_Indices.push_back(8);
	m_Indices.push_back(21);
	m_Indices.push_back(9);
	m_Indices.push_back(11);
	m_Indices.push_back(22);
	m_Indices.push_back(12);
	m_Indices.push_back(14);
	m_Indices.push_back(23);
	m_Indices.push_back(15);
	m_Indices.push_back(17);

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