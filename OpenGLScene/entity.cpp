#include "entity.h"

#include "matrix_transformations.h"

entity::entity()
{
	camera_ = camera::get_instance();
}

entity::~entity()
{
	delete camera_;

    for (const auto animation : animations_)
    {
	    delete animation;
    }
}

glm::mat4 entity::model()
{
	return model_;
}

void entity::add_animation(animation* animation)
{
	animations_.push_back(animation);
}

void entity::scale(const float x, const float y, const float z)
{
    model_ = matrix_scale(model_, x, y, z);
}

void entity::scale(const float scale)
{
    model_ = matrix_scale(model_, scale);
}

void entity::rotate(const float rotate_speed, const float x, const float y, const float z)
{
     model_ = matrix_rotate(model_, rotate_speed, x, y, z);
}

void entity::rotate(const float rotate_speed, const float rotate_value)
{
    model_ = matrix_rotate(model_, rotate_speed, rotate_value);
}

void entity::translate(const float x, const float y, const float z)
{
    model_ = matrix_translate(model_, x, y, z);
}

void entity::translate(const float translate_value)
{
    model_ = matrix_translate(model_, translate_value);
}
