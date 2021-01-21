// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

#include "../FMT.h"
#define PRINT_VEC(vec) fmt::print("{3}: [{0}, {1}, {2}]\n", vec.x, vec.y, vec.z, #vec)

glm::mat4 transform(glm::vec2 const& Orientation, glm::vec3 const& Translate, glm::vec3 const& Up)
{
    glm::mat4 Proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 10.f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), Translate);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Orientation.y, Up);
    glm::mat4 View = glm::rotate(ViewRotateX, Orientation.x, Up);
    glm::mat4 Model = glm::mat4(1.0f);
    glm::scale(Model, glm::vec3(2.0f, 2.0f, 2.0f));
    return Proj * View * Model;
}

#define PI 3.1415926f
void TestGLM()
{
	auto m2 = glm::rotate(glm::mat4(1.f), /*glm::radians(60.f)*/PI/3, glm::vec3(1.0f, 0.0f, 1.0f));
	auto m3 = glm::translate(glm::mat4(1.f), glm::vec3(10.0f, 0.0f, 0.0f));
	auto m4 = glm::scale(glm::mat4(1.f), glm::vec3(2.0f, 2.0f, 2.0f));

	glm::vec4 v(2.0f, 3.0f, 5.0f, 1.0f);
	auto m = m4 * m3 * m2;
	auto v1 = m * v;

	auto v2 = m2 * v;
	v2 = m3 * v2;
	v2 = m4 * v2;

	PRINT("GLM:\n");
	PRINT("glm::radians(60.f): {}\n", glm::radians(60.f));
	PRINT_VEC(v);
	PRINT_VEC(v1);
	PRINT_VEC(v2);
}