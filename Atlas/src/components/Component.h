#pragma once

#include <string>

class Component
{
public:
	Component(const std::string& tag) : Tag(tag) {}

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}

	inline const std::string& GetTag() { return Tag; }

protected:
	std::string Tag{ "Unnamed" };
};

