////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;

out vec2 ex_UVs;

uniform mat4 world;
uniform mat4 viewProjection;

void main(void)
{
    gl_Position = viewProjection * world * in_Position;
    ex_UVs = in_UVs;
}
