////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - proxy_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;
in vec3 in_Normal;

out vec2 ex_UVs;
out vec3 ex_Normal;

uniform mat4 world;
uniform mat4 viewProjection;
 
void main(void)
{
    gl_Position = viewProjection * world * in_Position;
    ex_UVs = in_UVs;
    ex_Normal = in_Normal;
}
