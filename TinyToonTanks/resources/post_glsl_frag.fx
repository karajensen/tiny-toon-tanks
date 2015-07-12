////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_effects_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2DMS SceneSampler;
uniform sampler2D NormalSampler;

uniform float finalMask;
uniform float sceneMask;
uniform float normalMask;
uniform float toonlineMask;
 
void main(void)
{
    // Multisampling for main scene
    ivec2 uvs = ivec2(ex_UVs.x * WINDOW_WIDTH, ex_UVs.y * WINDOW_HEIGHT);
    vec4 scene = vec4(0, 0, 0, 0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        scene += texelFetch(SceneSampler, uvs, i);
    }
    scene *= (1.0 / SAMPLES);
    vec4 final = scene;

    // Sample normals of four corners around pixel
    float offset = 0.0009;
    vec4 normal = texture(NormalSampler, ex_UVs);
    vec4 n1 = texture(NormalSampler, ex_UVs + vec2(-offset, -offset));
    vec4 n2 = texture(NormalSampler, ex_UVs + vec2( offset,  offset));
    vec4 n3 = texture(NormalSampler, ex_UVs + vec2(-offset,  offset));
    vec4 n4 = texture(NormalSampler, ex_UVs + vec2( offset, -offset));
    
    //determine the estimated difference between the pixel normal and surrounding
    vec4 diagonal1 = abs(n1 - n2);
    vec4 diagonal2 = abs(n3 - n4);
    vec4 diagonalDelta = diagonal1 + diagonal2;
    float edgeAmount = 1.0 - clamp(length(diagonalDelta), 0.0, 1.0);
    final *= edgeAmount;

    // Masking the selected texture
    out_Color.rgb = final.rgb * finalMask;
    out_Color.rgb += scene.rgb * sceneMask;
    out_Color.rgb += normal.rgb * normalMask;
    out_Color.rgb += vec3(edgeAmount) * toonlineMask;
}