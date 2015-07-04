////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normal_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
uniform mat4 world;
uniform mat4 viewProjection;
 
void main(void)
{
    gl_Position = viewProjection * world * in_Position;
}
