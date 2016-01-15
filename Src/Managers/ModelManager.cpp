#include "ModelManager.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

#include <VFS/FileSystem.h>
#include <Util/Utils.h>
#include <Rendering/Objects/Model.h>

#include <sstream>

using namespace Managers;

ModelManager* ModelManager::m_Instance = nullptr;

ModelManager* ModelManager::GetInstance()
{
	if (!m_Instance)
		m_Instance = new ModelManager();

	return m_Instance;
}

void ModelManager::DestroyInstance()
{
	if (!m_Instance)
		return;

	delete m_Instance;
	m_Instance = nullptr;
}

//////////////////////////////////////////////////////////////////////////

ModelManager::ModelManager() 
{
}

ModelManager::~ModelManager()
{
	for (auto s_Model : m_CachedModels)
		delete s_Model.second;

	m_CachedModels.clear();
}

bool ModelManager::Init()
{
	return true;
}

bool ModelManager::PrecacheModel(const std::string& p_Name)
{
	uint32_t s_NameHash = Util::Utils::StringHash(p_Name);

	auto s_Iterator = m_CachedModels.find(s_NameHash);

	if (s_Iterator != m_CachedModels.end())
		return true;

	Logger(Util::LogLevel::Info, "PreCaching Model '%s'.", p_Name.c_str());

	std::string s_FullPath = "/data/models/" + p_Name + ".obj";

	std::string s_FileData;
	if (!VFS::FileSystem::GetInstance()->ReadFile(s_FullPath, s_FileData))
	{
		Logger(Util::LogLevel::Warn, "Could not find model file '%s'.", p_Name.c_str());
		return false;
	}

	std::vector<tinyobj::shape_t> s_Shapes;
	std::vector<tinyobj::material_t> s_Materials;

	std::istringstream s_Stream(s_FileData);

	std::string s_Error;
	if (!tinyobj::LoadObj(s_Shapes, s_Materials, s_Error, s_Stream))
	{
		Logger(Util::LogLevel::Error, "Failed to parse OBJ file '%s'. Error: %s", p_Name.c_str(), s_Error.c_str());
		return false;
	}

	if (s_Shapes.size() == 0)
	{
		Logger(Util::LogLevel::Error, "Failed to load OBJ file '%s'. No shapes in file.", p_Name.c_str());
		return false;
	}

	Model::ModelData* s_ModelData = new Model::ModelData();

	// TODO: Support for multiple shapes.
	auto s_Shape = *s_Shapes.begin();

	for (size_t i = 0; i < s_Shape.mesh.positions.size() / 3; ++i)
	{
		s_ModelData->Vertices.push_back(glm::vec3(
			s_Shape.mesh.positions[(3 * i) + 0],
			s_Shape.mesh.positions[(3 * i) + 1],
			s_Shape.mesh.positions[(3 * i) + 2]
		));
	}

	for (size_t i = 0; i < s_Shape.mesh.normals.size() / 3; ++i)
	{
		s_ModelData->Normals.push_back(glm::vec3(
			s_Shape.mesh.normals[(3 * i) + 0],
			s_Shape.mesh.normals[(3 * i) + 1],
			s_Shape.mesh.normals[(3 * i) + 2]
		));
	}

	for (size_t i = 0; i < s_Shape.mesh.texcoords.size() / 2; ++i)
	{
		s_ModelData->UVs.push_back(glm::vec2(
			s_Shape.mesh.texcoords[(2 * i) + 0],
			s_Shape.mesh.texcoords[(2 * i) + 1]
		));
	}

	for (size_t i = 0; i < s_Shape.mesh.indices.size(); ++i)
	{
		s_ModelData->Indices.push_back(s_Shape.mesh.indices[i]);
	}

	m_CachedModels[s_NameHash] = s_ModelData;
	return true;
}

Model::ModelData* ModelManager::GetModelData(const std::string& p_Name) const
{
	auto s_Iterator = m_CachedModels.find(Util::Utils::StringHash(p_Name));

	if (s_Iterator == m_CachedModels.end())
		return nullptr;

	return s_Iterator->second;
}

void ModelManager::RegisterModel(Rendering::Objects::Model* p_Model)
{
	if (!p_Model)
		return;

	if (m_Models.find(p_Model->ShaderProgram()->GetProgram()) == m_Models.end())
		m_Models[p_Model->ShaderProgram()->GetProgram()] = std::unordered_set<Rendering::Objects::Model*>();

	m_Models[p_Model->ShaderProgram()->GetProgram()].insert(p_Model);
}

void ModelManager::RemoveModel(Rendering::Objects::Model* p_Model)
{
	if (!p_Model)
		return;

	auto s_Iterator = m_Models.find(p_Model->ShaderProgram()->GetProgram());

	if (s_Iterator == m_Models.end())
		return;

	s_Iterator->second.erase(p_Model);
}