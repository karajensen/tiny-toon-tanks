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
        
        lightColour *= ((dot(vertToLight, normal) + 1.0) *
            ((1.0 - meshDiffuse) * 0.5)) + meshDiffuse;

	    // Create bands of color depending on angle to light
        if (lightColour > 0.75)
        {
            lightColour -= vec4(0.0, 0.0, 0.0, 0.0); 
        }
        else if (lightColour > 0.5)
        {
            lightColour -= vec4(0.2, 0.2, 0.2, 0.0);
        }
        else if (lightColour > 0.25)
        {
            lightColour -= vec4(0.4, 0.4, 0.4, 0.0);
        }
        else
        {
            lightColour -= vec4(0.6, 0.6, 0.6, 0.0);
        }

        diffuse += lightColour * attenuation * lightActive[i];
    }

    out_Color = vec4(diffuseTex.rgb * diffuse, 1.0);
}