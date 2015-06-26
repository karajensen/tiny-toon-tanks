////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vsToon.glsl
////////////////////////////////////////////////////////////////////////////////////////

uniform vec3 LightPos;
uniform vec3 LightColor;
varying vec2 UV;
varying vec3 VertexNormal;
varying vec3 LightVector;
varying vec4 DiffuseColor;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec4 WorldViewPos = gl_ModelViewMatrix * gl_Vertex;
    VertexNormal = gl_NormalMatrix * gl_Normal;
    LightVector = normalize(LightPos - vec3(WorldViewPos));
    DiffuseColor = vec4(LightColor, 1.0);
    UV = gl_MultiTexCoord0.xy;
}
