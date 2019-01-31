#pragma once
#include <stdint.h>
#include <string>
#include <glm\vec2.hpp> 
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat2x2.hpp>
#include <glm\mat3x3.hpp>
#include <glm\mat4x4.hpp>

namespace Graphics
{
	namespace OpenGL
	{
		struct Uniform
		{
		private:

			int32_t uniformLocation;

		public:

			Uniform();

			explicit Uniform(int32_t programDescriptor, std::string&& uniformName);

			~Uniform();

			void LoadUniform(bool arg);

			void LoadUniform(float arg);

			void LoadUniform(int32_t arg);

			void LoadUniform(const glm::vec2& arg);

			void LoadUniform(glm::vec2&& arg);

			void LoadUniform(const glm::vec3& arg);

			void LoadUniform(glm::vec3&& arg);

			void LoadUniform(const glm::vec4& arg);

			void LoadUniform(glm::vec4&& arg);

			void LoadUniform(const glm::mat2& arg);

			void LoadUniform(const glm::mat3& arg);

			void LoadUniform(const glm::mat4& arg);

		};
	}
}