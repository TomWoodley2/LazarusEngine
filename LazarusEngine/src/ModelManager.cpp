#include "ModelManager.h"

#include "Model.h"


Model* ModelManager::getModel(std::string filename)
{


	auto iter = m_models.find(filename);

	if (iter != std::end(m_models))
	{
		// if found return the pointer
		return iter->second;
	}
	else
	{
		// not found so load model
		Model* thisModel = new Model(filename);
		m_models[filename] = thisModel;


		return thisModel;
	}

}