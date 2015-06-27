////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - toon_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color;

in vec2 ex_UVs;
in vec3 ex_PositionWorld;
in vec3 ex_Normal;

uniform float lightActive[MAX_LIGHTS];
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightDiffuse[MAX_LIGHTS];
uniform vec3 lightAttenuation[MAX_LIGHTS];

uniform sampler2D DiffuseSampler;

void main(void)
{
    vec4 diffuseTex = texture(DiffuseSampler, ex_UVs);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(ex_Normal);

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        vec3 lightColour = lightDiffuse[i];
        vec3 vertToLight = lightPosition[i] - ex_PositionWorld;
        float lightLength = length(vertToLight);
        
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);

        vertToLight /= lightLength;
        
        float lightAmount = (dot(vertToLight, normal) + 1.0) * 0.5;

	    // Create bands of color depending on angle to light
        if (lightAmount <= 0.25)
        {
            lightAmount -= 0.6;
        }
        else if (lightAmount <= 0.5)
        {
            lightAmount -= 0.4;
        }
        else if (lightAmount <= 0.75)
        {
            lightAmount -= 0.2;
        }

        diffuse += lightColour * lightAmount * attenuation * lightActive[i];
    }

    out_Color = vec4(diffuseTex.rgb * diffuse, 1.0);
}