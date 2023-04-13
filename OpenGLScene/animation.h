#pragma once
#include <glm/gtc/type_ptr.hpp>

class animation
{
public:
	virtual ~animation();
	virtual glm::mat4 execute(const glm::mat4& model) const;
};
