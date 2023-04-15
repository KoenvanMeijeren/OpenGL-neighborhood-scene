#include "object-builder.h"

object_builder::~object_builder()
{
	for (const auto object : objects_)
	{
		delete object;
	}
}

void object_builder::render()
{
	for (const auto object : objects_)
	{
		object->render();
	}
}
