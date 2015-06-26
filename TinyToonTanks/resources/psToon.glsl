////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - psToon.glsl
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

	// Create bands of color depending on angle to light
    if (diffuseValue > 0.75)
    {
        Texture -= vec4(0.0,0.0,0.0,0.0); 
    }
    else if (diffuseValue > 0.5)
    {
        Texture -= vec4(0.2,0.2,0.2,0.0);
    }
    else if (diffuseValue > 0.25)
    {
        Texture -= vec4(0.4,0.4,0.4,0.0);
    }
    else
    {
        Texture -= vec4(0.6,0.6,0.6,0.0);
    }
    gl_FragColor = Texture;
}
