#pragma once

#include "Component.h"
#include "WindowComponent.h"
#include "../utilities/Log.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>

class ComponentStack
{
public:
	~ComponentStack();

	template <typename T, typename... Args>
	std::weak_ptr<T> AttachComponent(const std::string& Tag, Args&&... args)
	{

		std::string ComponentName(typeid(T).name());
		std::shared_ptr<T> ComponentPtr = std::make_shared<T>(Tag, std::forward<Args>(args)...);

		bool VectorExists = m_Components.find(ComponentName) != m_Components.end();

		if (VectorExists)
		{

			auto& ComponentVector = m_Components[ComponentName];
			ComponentVector.push_back(ComponentPtr);

		}

		else
		{

			std::vector<std::shared_ptr<Component>> NewVector = { ComponentPtr };
			m_Components.emplace(ComponentName, NewVector);

		}

		ComponentPtr->OnAttach();

		return ComponentPtr;

	}
	template <typename... Args>
	std::weak_ptr<WindowComponent> AttachWindowComponent(const std::string& Tag, Args&&... args)
	{
		std::shared_ptr<WindowComponent> ComponentPtr = std::make_shared<WindowComponent>(Tag, std::forward<Args>(args)...);
		m_WindowComponents.push_back(ComponentPtr);
		ComponentPtr->OnAttach();

		return ComponentPtr;
	}

	template <typename T>
	void DetachComponent(const std::string& Tag)
	{
		std::string ComponentTypeName = typeid(T).name();

		if (ComponentTypeName == typeid(WindowComponent).name())
		{
			for (auto& window : m_WindowComponents)
			{

				if (window->GetTag() == Tag)
				{
					window->OnDetach();

					std::swap(window, m_WindowComponents.back());
					m_WindowComponents.pop_back();

					return;
				}

			}
		}

		bool VectorExists = m_Components.find(ComponentTypeName) != m_Components.end();

		if (!VectorExists)
		{
			ATLAS_WARN("Component with tag : " + Tag + " is currently not attached and can therefore not detach!");
			return;

		}

		
		// Detach Component
		auto& ComponentVector = m_Components[ComponentTypeName];
		for (auto& element : ComponentVector)
		{

			if (element->GetTag() == Tag)
			{

				element->OnDetach();

				std::swap(element, ComponentVector.back());
				ComponentVector.pop_back();


				break;

			}

		}

		if (ComponentVector.empty())
		{
			m_Components.erase(ComponentTypeName);
		}

	}

	template <typename T>
	std::weak_ptr<T> GetComponent(const std::string& Tag)
	{
		std::string ComponentTypeName = typeid(T).name();

		bool VectorExists = m_Components.find(ComponentTypeName) != m_Components.end();

		if (!VectorExists)
		{
			ATLAS_WARN("Component : " + Tag + " currently not attached...");
			return std::shared_ptr<T>(nullptr);

		}

		auto& ComponentVector = m_Components[ComponentTypeName];
		for (auto& element : ComponentVector)
		{

			if (element->GetTag() == Tag)
			{

				return std::static_pointer_cast<T>(element);

			}

		}

		ATLAS_WARN("No component with tag : " + Tag + " was found in the stack");
		return std::shared_ptr<T>(nullptr);

	}
	std::weak_ptr<WindowComponent> GetWindowComponent(const std::string& Tag)
	{
		for (auto& window : m_WindowComponents)
		{

			if (window->GetTag() == Tag)
			{
			
				return window;

			}

		}

		ATLAS_WARN("No window components with tag : " + Tag + " currently attached");
		return std::shared_ptr<WindowComponent>(nullptr);

	}

	template <typename T>
	std::unique_ptr<std::vector<std::shared_ptr<Component>>> GetComponents()
	{

		std::string ComponentName = typeid(T).name();

		if (m_Components.find(ComponentName) != m_Components.end())
		{

			
			return std::make_unique<std::vector<std::shared_ptr<Component>>>(m_Components[ComponentName]);

		}

		ATLAS_WARN("No components of type " + ComponentName + " were found...");
		return nullptr;

	}
	std::unique_ptr<std::vector<std::shared_ptr<WindowComponent>>> GetWindowComponents()
	{
		return std::make_unique<std::vector<std::shared_ptr<WindowComponent>>>(m_WindowComponents);
	}

	// Runtime Events
	void OnUpdate();

	// DEBUG
	void PrintComponents();

private:

	std::unordered_map<std::string, std::vector<std::shared_ptr<Component>>> m_Components;
	std::vector<std::shared_ptr<WindowComponent>> m_WindowComponents;
};
