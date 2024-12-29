#include "ComponentStack.h"

ComponentStack::~ComponentStack()
{
	std::unordered_map<std::string, std::vector<std::shared_ptr<Component>>>::iterator it = m_Components.begin();

	while (it != m_Components.end())
	{
		auto& vec = it->second;
		
		for (auto& VecValue : vec)
		{
			VecValue->OnDetach();
		}
		
		it++;
	}

}

void ComponentStack::PrintComponents()
{
	std::unordered_map<std::string, std::vector<std::shared_ptr<Component>>>::iterator it
		= m_Components.begin();

	while (it != m_Components.end()) {

		std::string word = it->first;

		auto& vec = it->second;
		std::cout << word << " :: ";

		for (auto& element : vec)
		{

			std::cout << element->GetTag();


			if (element->GetTag() != vec.back()->GetTag())
			{
				std::cout << " , ";
			}

		}

		std::cout << std::endl;

		it++;
	}

	std::cout << "class Windows" << " :: ";

	for (auto& window : m_WindowComponents) 
	{

		if (window->GetTag() == m_WindowComponents.back()->GetTag())
		{
			std::cout << window->GetTag();
		}
		else
		{
			std::cout << window->GetTag() << " , ";
		}

	}

	std::cout << std::endl;
}

void ComponentStack::OnUpdate()
{
	std::unordered_map<std::string, std::vector<std::shared_ptr<Component>>>::iterator it = m_Components.begin();

	while (it != m_Components.end()) {

		auto& vector = it->second;

		for (auto& element : vector)
		{

			element->OnUpdate();

		}

		it++;
	}

	for (auto& window : m_WindowComponents)
	{
			
		window->OnUpdate();

	}

}