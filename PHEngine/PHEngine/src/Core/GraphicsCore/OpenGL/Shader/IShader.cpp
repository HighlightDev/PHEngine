#include "IShader.h"

#include <fstream>

namespace Graphics
{
   namespace OpenGL
   {
      IShader::IShader(const std::string& shaderName)
         : m_vertexShaderID(-1)
         , m_fragmentShaderID(-1)
         , m_geometryShaderID(-1)
         , m_shaderProgramID(-1)
         , mShaderName(shaderName)
      {
      }
   
      IShader::~IShader()
      {
      }

      Uniform IShader::GetUniform(const std::string& uniformName, uint32_t shaderProgramID) const
      {
         try
         {
            return Uniform(shaderProgramID, uniformName);
         }
         catch (std::invalid_argument innerEx)
         {
            std::string message;
            EngineUtility::StringStreamWrapper::ToString("Shader with name", mShaderName, " could not bind uniform(s);", "Inner exception message :", '\n', innerEx.what());

            throw std::invalid_argument(EngineUtility::StringStreamWrapper::FlushString());
         }
      }

      UniformArray IShader::GetUniformArray(const std::string& uniformName, size_t countOfUniforms, uint32_t shaderProgramID) const
      {
         try
         {
            return UniformArray(shaderProgramID, countOfUniforms, uniformName);
         }
         catch (std::invalid_argument innerEx)
         {
            std::string message;
            EngineUtility::StringStreamWrapper::ToString("Shader with name", mShaderName, " could not bind uniform(s);", "Inner exception message :", '\n', innerEx.what());

            throw std::invalid_argument(EngineUtility::StringStreamWrapper::FlushString());
         }
      }

      std::vector<std::string> IShader::LoadShaderSrc(const std::string& pathToShader) const
      {
         std::ifstream stream(pathToShader);
         std::string line;

         std::vector<std::string> code;

         while (stream.is_open() && getline(stream, line))
         {
            code.emplace_back(line);
         }

         stream.clear();
         stream.close();

         return code;
      }

      void IShader::WriteShaderSrc(const std::string& pathToShader, const std::string& src) const
      {
         std::ofstream writeStream(pathToShader);
         writeStream << src;
      }

      void IShader::ProcessPredefine(const std::string&  pathToShader, const std::vector<ShaderGenericDefineConstant>& constantDefines, const std::vector<ShaderGenericDefine>& defines) const
      {
         if (pathToShader == "")
            return;

         auto shaderSrc = LoadShaderSrc(pathToShader);

         // src only with macros
         std::vector<ShaderGenericDefineConstant> existingConstantDefines;
         std::vector<ShaderGenericDefine> existingDefines;

         for (auto it = shaderSrc.begin(); it != shaderSrc.end(); ++it)
         {
            if (EngineUtility::StartsWith(*it, "#define"))
            {
               size_t indexName = EngineUtility::IndexOf(*it, " ");
               size_t indexValue = EngineUtility::IndexOf(*it, " ", indexName + 1);

               const std::string name = it->substr(indexName + 1, indexValue - indexName - 1);
               std::string value = it->substr(indexValue + 1);
               if (std::string::npos == indexValue)
               {
                  existingDefines.push_back(ShaderGenericDefine(name, true));
               }
               else
               {
                  existingConstantDefines.push_back(ShaderGenericDefineConstant(name, value));
               }
            }
         }

         // remove all macros from code
         for (auto it = shaderSrc.begin(); it != shaderSrc.end();)
         {
            if (EngineUtility::StartsWith(*it, "#define"))
               it = shaderSrc.erase(it);
            else
               ++it;
         }

         // update values for existing macros
         for (auto new_it = constantDefines.begin(); new_it != constantDefines.end(); ++new_it)
         {
            for (auto existing_it = existingConstantDefines.begin(); existing_it != existingConstantDefines.end();)
            {
               if (existing_it->m_Name == new_it->m_Name)
               {
                  existing_it = existingConstantDefines.erase(existing_it);
               }
               else
               {
                  ++existing_it;
               }
            }

            existingConstantDefines.push_back(*new_it);
         }

         // update defines
         for (auto new_it = defines.begin(); new_it != defines.end(); ++new_it)
         {
            for (auto existing_it = existingDefines.begin(); existing_it != existingDefines.end();)
            {
               if (existing_it->m_Name == new_it->m_Name)
               {
                  existing_it = existingDefines.erase(existing_it);
               }
               else
               {
                  ++existing_it;
               }
            }

            if (new_it->bDefined)
            {
               existingDefines.push_back(std::move(*new_it));
            }
         }

         for (auto it = constantDefines.begin(); it != constantDefines.end(); ++it)
         {
            EngineUtility::StringStreamWrapper::ToString("#define ", it->m_Name, " ", it->m_Value, '\n');
         }
         const std::string constantDefinesResult = std::move(EngineUtility::StringStreamWrapper::FlushString());

         for (auto it = existingDefines.begin(); it != existingDefines.end(); ++it)
         {
            EngineUtility::StringStreamWrapper::ToString("#define ", it->m_Name, '\n');
         }
         const std::string definesResult = std::move(EngineUtility::StringStreamWrapper::FlushString());

         std::vector<std::string>::iterator version_it = shaderSrc.begin();

         for (auto it = shaderSrc.begin(); it != shaderSrc.end(); ++it, ++version_it)
         {
            if (EngineUtility::StartsWith(*it, "#version"))
            {
               version_it += 2;
               break;
            }
         }

         if ("" != constantDefinesResult)
         {
            shaderSrc.insert(version_it, constantDefinesResult);
         }

         version_it = shaderSrc.begin();
         for (auto it = shaderSrc.begin(); it != shaderSrc.end(); ++it, ++version_it)
         {
            if (EngineUtility::StartsWith(*it, "#version"))
            {
               version_it += 2;
               break;
            }
         }

         if ("" != definesResult)
         {
            shaderSrc.insert(version_it, definesResult);
         }

         std::string codeResult = "";

         for (std::vector<std::string>::iterator it = shaderSrc.begin(); it != shaderSrc.end(); ++it)
         {
            std::string& str = *it;
            str = EngineUtility::TrimEnd(str);

            if (shaderSrc.end() - 1 != it)
               codeResult += str + "\n";
            else
               codeResult += str;
         }

         WriteShaderSrc(pathToShader, codeResult);
      }

      bool IShader::CompileShaders() const
      {
         glCompileShader(m_vertexShaderID);
         glCompileShader(m_fragmentShaderID);
         glCompileShader(m_geometryShaderID);

         return true;
      }


      bool IShader::LinkShaders() const
      {
         glAttachShader(m_shaderProgramID, m_vertexShaderID);
         glAttachShader(m_shaderProgramID, m_fragmentShaderID);
         if (m_geometryShaderID != -1)
         {
            glAttachShader(m_shaderProgramID, m_geometryShaderID);
         }
         glLinkProgram(m_shaderProgramID);

         glDetachShader(m_shaderProgramID, m_vertexShaderID);
         glDetachShader(m_shaderProgramID, m_fragmentShaderID);
         if (m_geometryShaderID != -1)
         {
            glDetachShader(m_shaderProgramID, m_geometryShaderID);
         }

         return true;
      }

      std::string IShader::GetCompileLogInfo() const
      {
         std::string compileLog;

         EngineUtility::StringStreamWrapper::FlushString(); // Just to clear stream
         GLint vertex_compiled = 0;
         if (m_vertexShaderID != -1)
         {
            glGetShaderiv(m_vertexShaderID, GL_COMPILE_STATUS, &vertex_compiled);
            if (vertex_compiled != GL_TRUE)
            {
               GLint log_length = 0;
               glGetShaderiv(m_vertexShaderID, GL_INFO_LOG_LENGTH, &log_length);

               std::vector<char> message(log_length);
               glGetShaderInfoLog(m_vertexShaderID, log_length, nullptr, message.data());
               EngineUtility::StringStreamWrapper::ToString("Vertex shader : ", std::string(message.begin(), message.end()), '\n');
            }
         }

         /*Fragment shader log info*/
         GLint fragment_compiled = 0;
         if (m_fragmentShaderID != -1)
         {
            glGetShaderiv(m_fragmentShaderID, GL_COMPILE_STATUS, &fragment_compiled);
            if (fragment_compiled != GL_TRUE)
            {
               GLint log_length = 0;
               glGetShaderiv(m_fragmentShaderID, GL_INFO_LOG_LENGTH, &log_length);

               std::vector<char> message(log_length);
               glGetShaderInfoLog(m_fragmentShaderID, log_length, nullptr, message.data());
               EngineUtility::StringStreamWrapper::ToString("Fragment shader : ", std::string(message.begin(), message.end()), '\n');
            }
         }


         GLint geometry_compiled = 0;
         /*Geometry shader log info*/
         if (m_geometryShaderID != -1)
         {
            glGetShaderiv(m_geometryShaderID, GL_COMPILE_STATUS, &geometry_compiled);
            if (geometry_compiled != GL_TRUE)
            {
               GLint log_length = 0;
               glGetShaderiv(m_geometryShaderID, GL_INFO_LOG_LENGTH, &log_length);

               std::vector<char> message(log_length);
               glGetShaderInfoLog(m_geometryShaderID, log_length, nullptr, message.data());
               EngineUtility::StringStreamWrapper::ToString("Geometry shader : ", std::string(message.begin(), message.end()), '\n');
            }
         }

         if ((m_vertexShaderID != -1 && vertex_compiled != GL_TRUE) || (m_fragmentShaderID != -1 && fragment_compiled != GL_TRUE) || (m_geometryShaderID != -1 && geometry_compiled != GL_TRUE))
            compileLog += std::string("Unresolved mistakes at :" + mShaderName + '\n') + EngineUtility::StringStreamWrapper::FlushString() + "\n";

         return compileLog;
      }

      std::string IShader::GetLinkLogInfo() const
      {
         std::string  linkLog;
         EngineUtility::StringStreamWrapper::FlushString(); // Just to clear stream
         GLint isLinked = 0;
         glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &isLinked);

         if (isLinked == GL_FALSE)
         {
            GLint log_length = 0;
            glGetProgramiv(m_shaderProgramID, GL_INFO_LOG_LENGTH, &log_length);

            std::vector<char> message(log_length);
            glGetProgramInfoLog(m_shaderProgramID, log_length, nullptr, message.data());
            EngineUtility::StringStreamWrapper::ToString("Unsolved mistakes at :", mShaderName, "\n");
         }

         linkLog = std::move(EngineUtility::StringStreamWrapper::FlushString());
         linkLog = linkLog != "" ? linkLog + "\n" : linkLog;

         return linkLog;
      }

      void IShader::ExecuteShader() const
      {
         glUseProgram(m_shaderProgramID);
      }

      void IShader::StopShader() const
      {
         glUseProgram(0);
      }

      void IShader::CleanUp(bool bDeleteShaderProgram)
      {
         StopShader();
         glDetachShader(m_shaderProgramID, m_vertexShaderID);
         glDetachShader(m_shaderProgramID, m_fragmentShaderID);
         if (m_geometryShaderID != -1)
         {
            glDetachShader(m_shaderProgramID, m_geometryShaderID);
            glDeleteShader(m_geometryShaderID);
         }

         glDeleteShader(m_vertexShaderID);
         glDeleteShader(m_fragmentShaderID);

         if (bDeleteShaderProgram)
         {
            glDeleteShader(m_shaderProgramID);
         }
      }

   }
}