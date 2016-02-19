////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shadow_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;

uniform mat4 world;
uniform mat4 viewProjection;
uniform vec3 planePosition;
uniform vec3 planeNormal;

void main(void)
{
    float distance = abs(dot((in_Position.xyz - planePosition), planeNormal));
    vec3 pointToPlane = -distance * planeNormal;

    vec4 position;
    position.xyz = in_Position.xyz + pointToPlane;
    position.w = in_Position.w;

    gl_Position = viewProjection * world * position;
}