#pragma once
#include "Component.h"
#include <unordered_map>
#include <typeindex>
#include <string>

class GameObject
{
public:
	template <typename T>
	T* getComponent()
	{

		auto iter = m_components.find(typeid(T));

		if(iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			return dynamic_cast<T*>(iter->second);
		}
		
		// return null if we don't have a component of that type
		return nullptr;
		
	}

	template <typename T>
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		m_components[typeid(T)] = comp;
	}

	// we add these - class is now abstract...
	virtual void OnUpdate(float dt) = 0;
	virtual void OnMessage(const std::string m) = 0;

	// Set and get object type
	void setObjectType(const std::string type) { objectType = type; }
	std::string getObjectType() { return objectType; }

	// Set and get physics properties
	void setPhysicsProperties(int physicsIn) { physicsProperties = physicsIn; }
	int getPhysicsProperties() { return physicsProperties; }
private:
	std::unordered_map<std::type_index, Component*> m_components;
	std::string objectType;
	int physicsProperties = -1; // Defaults to 0
};


