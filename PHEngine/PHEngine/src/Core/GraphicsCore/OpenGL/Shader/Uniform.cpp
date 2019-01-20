#include "Uniform.h"

#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace Graphics
{
	namespace OpenGL
	{
		Uniform::Uniform() {}

		Uniform::Uniform(int32_t programDescriptor, std::string&& uniformName)
		{
			uniformLocation = glGetUniformLocation(programDescriptor, uniformName.c_str());
		/*	if (uniformLocation < 0)
				throw new std::invalid_argument(std::string("Could not bind uniform " + uniformName));*/
		}

		Uniform::~Uniform()
		{
		}

		void Uniform::LoadUniform(bool arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform1i(uniformLocation, arg);
		}
		void Uniform::LoadUniform(float arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform1f(uniformLocation, arg);
		}
		void Uniform::LoadUniform(int32_t arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform1i(uniformLocation, arg);
		}
		void Uniform::LoadUniform(glm::vec2& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform2f(uniformLocation, arg.x, arg.y);
		}
		void Uniform::LoadUniform(glm::vec2&& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform2f(uniformLocation, arg.x, arg.y);
		}
		void Uniform::LoadUniform(glm::vec3& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform3f(uniformLocation, arg.x, arg.y, arg.z);
		}
		void Uniform::LoadUniform(glm::vec3&& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform3f(uniformLocation, arg.x, arg.y, arg.z);
		}
		void Uniform::LoadUniform(glm::vec4& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform4f(uniformLocation, arg.x, arg.y, arg.z, arg.w);
		}
		void Uniform::LoadUniform(glm::vec4&& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniform4f(uniformLocation, arg.x, arg.y, arg.z, arg.w);
		}
		void Uniform::LoadUniform(glm::mat2& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniformMatrix2fv(uniformLocation, 1, false, glm::value_ptr(arg));
		}
		void Uniform::LoadUniform(glm::mat3& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniformMatrix3fv(uniformLocation, 1, false, glm::value_ptr(arg));
		}
		void Uniform::LoadUniform(glm::mat4& arg)
		{
			if (uniformLocation == -1)
				return;
			glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(arg));
		}
	}
}