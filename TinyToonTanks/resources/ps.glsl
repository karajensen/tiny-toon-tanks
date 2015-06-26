////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ps.glsl
////////////////////////////////////////////////////////////////////////////////////////

uniform sampler2D TexSampler;
varying vec2 UV;
varying vec3 VertexNormal;
varying vec3 LightVector;
varying vec4 DiffuseColor;
void main()
{
	vec4 Texture = texture2D(TexSampler, UV);
	vec3 normalisedNormal = normalize(VertexNormal);
	float diffuseValue = (dot(LightVector,normalisedNormal) + 1.0)*0.5;
	vec4 Diffuse = vec4(diffuseValue,diffuseValue,diffuseValue,1.0)*DiffuseColor;
	gl_FragColor = Diffuse*Texture;
}
