////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Glcommon.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class RenderTarget;

/**
* Shader used to render a mesh
*/
class Shader
{
public:

    /**
    * Constructor
    * @param name The name of the shader
    * @param path The path to the shader
    */
    Shader(const std::string& name, const std::string& path);

    /**
    * Destructor
    */
    ~Shader();

    /**
    * Initialises the shader
    * @return whether initialisation was successful
    */
    bool Initialise();

    /**
    * Sets the shader as activated for rendering
    */
    void SetActive();

    /**
    * Sends a glm::mat4 to the shader
    * @param name Name of the glm::mat4 to send. This must match on the shader to be successful
    * @param glm::mat4 The glm::mat4 to send
    */
    void SendUniform(const std::string& name, const glm::mat4& matrix);

    /**
    * Sends the float to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The float value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, float value, int offset = -1);

    /**
    * Sends the vector to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The vector value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, const glm::vec2& value, int offset = -1);

    /**
    * Sends the vector to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The vector value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, const glm::vec3& value, int offset = -1);

    /**
    * Sends the vector to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The vector value to send
    * @param offset The index offset into the array if this value is apart of an array
    */
    void SendUniform(const std::string& name, const glm::vec4& value, int offset = -1);

    /**
    * Enables the vertex shader 'in' attributes for the shader
    * This is required after the shader is active and when a mesh buffer is bound
    * Also sends any arrays in the scratch buffer to the shader
    */
    void EnableShader();

    /**
    * Sends a texture to the shader
    * @param sampler Name of the shader texture sampler to use
    * @param id The unique id for the opengl texture
    */
    void SendTexture(const std::string& sampler, GLuint id);

    /**
    * Sends the render target texture to the shader
    * @param sampler Name of the shader texture sampler to use
    * @param target The render target to send
    * @param ID the id of the target texture to send
    */
    void SendTexture(const std::string& sampler, const RenderTarget& target, int ID = 0);

    /**
    * Clears the render target texture from the shader
    * @param sampler Name of the shader texture sampler to use
    * @param target The render target to clear
    * @param ID the id of the target texture to clear
    */
    void ClearTexture(const std::string& sampler, const RenderTarget& target);

    /**
    * @return the name of the shader
    */
    const std::string& Name() const;

    /**
    * Initialises the defined constants in the shaders
    * @param constants The constants to set
    */
    typedef std::vector<std::pair<std::string, std::string>> ShaderConstants;
    static void InitialiseConstants(const ShaderConstants& constants);

private:

    /**
    * Prevent copying
    */
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
    * Logs error information about the shader
    * @param text The text to log
    * @Param error Whether this entry is an error
    */
    void LogShader(const std::string& text);

    /**
    * Sends the array buffers to the shader if they have been updated
    */
    void SendUniformArrays();

    /**
    * Updates the cached array scratch buffer
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param count The number of floats to send
    * @param offset The index offset into the array
    */
    void UpdateUniformArray(const std::string& name, const float* value, int count, int offset);

    /**
    * Sends the float to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param count The number of floats to send
    */
    void SendUniformFloat(const std::string& name, const float* value, int count);

    /**
    * Sends the float to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param location Unique location within the shader
    * @param size The number of elements in the array (1 if not an array)
    * @param type Whether a float, vec2, vec3, vec4 
    */
    void SendUniformFloat(const std::string& name, const float* value, 
                          int location, int size, GLenum type);

    /**
    * Generates a shader from the given shader file
    * @param loadPath The path to the file
    * @param text The container to save the text to
    * @return whether the file was successfully loaded
    */
    bool LoadShaderFile(const std::string& loadPath, std::string& text);

    /**
    * Sends a texture to the shader
    * @param sampler Name of the shader texture sampler to use
    * @param id The unique id for the opengl texture
    * @param multisample Whether this texture is to be multisampled
    */
    void SendTexture(const std::string& sampler, GLuint id, bool multisample);

    /**
    * Clears the current texture set
    * @param sampler Name of the shader texture sampler to use
    * @param multisample Whether this texture is to be multisampled
    */
    void ClearTexture(const std::string& sampler, bool multisample);

    /**
    * Determines the output fragment attributes and binds them
    * @return if the call was successful
    */
    bool BindFragmentAttributes();

    /**
    * Determines the input vertex attributes and binds them
    * @return if the call was successful
    */
    bool BindVertexAttributes();

    /**
    * Determines the shader non-attribute uniform variables and samplers
    * @return if the call was successful
    */
    bool FindShaderUniforms();

    /**
    * Generates the shader for the engine
    * @param shader The description of the shader (vertex or fragment)
    * @param index The unique index of the shader (vs or fs) to compile
    * @param text Vertex or fragment shader text
    * @return Whether compilation was successful
    */
    bool CompileShader(const std::string& shader, GLint index, const std::string& text);

    /**
    * Links together all shaders within the shader program
    * @return if the call was successful
    */
    bool LinkShaderProgram();
                    
    /**
    * Determines the unique ID of the texture slot
    * @param slot The slot in the shader the texture will fill
    * @return the unique ID of the texture
    */
    static unsigned int GetTexture(int slot);

    /**
    * Determines the amount of float components from the OpenGL type
    * @param type The type of object (ie. float, vec2)
    * @return the number of floats in the object
    */
    static int GetComponents(GLenum type);

    /**
    * Information for each vertex input attribute
    */
    struct AttributeData
    {
        int components = 0;  ///< Number of float components in the type
        int location = 0;    ///< The index location of the attribute
        std::string name;    ///< The name of the attribute
    };

    /**
    * Information for a non-attribute shader uniform
    * @note scratch is not allocated for matrices
    */
    struct UniformData
    {
        GLenum type = 0;            ///< Whether a float, vec2, vec3, vec4
        int location = 0;           ///< Unique location within the shader
        int size = 0;               ///< The number of elements in the array (1 if not an array)
        std::vector<float> scratch; ///< Buffer allocated with the total floats used
        bool updated = false;       ///< Whether the scratch buffer has been updated
    };

    /**
    * Information for a sampler
    */
    struct SamplerData
    {
        int slot = 0;               ///< Order of usage in shader
        int allocated = -1;         ///< Currently allocated texture to this slot
        int location = 0;           ///< Unique location within the shader
        GLenum type = 0;            ///< Whether a texture, cubemap or ms
    };

    typedef std::unordered_map<std::string, UniformData> UniformMap;
    typedef std::unordered_map<std::string, SamplerData> SamplerMap;

private:

    UniformMap m_uniforms;                    ///< Vertex and fragment non-attribute uniform data
    SamplerMap m_samplers;                    ///< Fragment shader sampler locations
    std::vector<AttributeData> m_attributes;  ///< Vertex shader input attributes
    GLint m_program = -1;                     ///< Shader program
    GLint m_vs = -1;                          ///< GLSL Vertex Shader
    GLint m_fs = -1;                          ///< GLSL Fragment Shader
    GLsizei m_stride = 0;                     ///< Stride required for vertex attributes
    const std::string m_name;                 ///< name of the shader
    const std::string m_vertexFile;           ///< filename of the glsl shader
    const std::string m_fragmentFile;         ///< filename of the glsl shader
    std::string m_vertexText;                 ///< The vertex shader string
    std::string m_fragmentText;               ///< The fragment shader string
    static ShaderConstants sm_constants;      ///< Holds constant data to substitute into shaders
};                              