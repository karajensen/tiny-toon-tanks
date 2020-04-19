////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - post_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec2 ex_UVs;
out vec4 out_Color;

uniform sampler2DMS SceneSampler;
uniform sampler2DMS NormalSampler;

uniform float finalMask;
uniform float sceneMask;
uniform float normalMask;
uniform float toonlineMask;
 
void main(void)
{
    // Multisampling for main scene
    vec2 window = vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    ivec2 uvs = ivec2(ex_UVs.x * window.x, ex_UVs.y * window.y);
    vec4 scene = vec4(0, 0, 0, 0);
    for (int i = 0; i < SAMPLES; ++i)
    {
        scene += texelFetch(SceneSampler, uvs, i);
    }
    scene *= (1.0 / SAMPLES);
    vec4 final = scene;

    // Sample normals of four corners around pixel
    float offset = 0.0009;
    vec4 normal = texelFetch(NormalSampler, uvs, 0);

    uvs = ivec2((ex_UVs.x - offset) * window.x, (ex_UVs.y - offset) * window.y);
    vec4 n1 = texelFetch(NormalSampler, uvs, 0);

    uvs = ivec2((ex_UVs.x + offset) * window.x, (ex_UVs.y + offset) * window.y);
    vec4 n2 = texelFetch(NormalSampler, uvs, 0);

    uvs = ivec2((ex_UVs.x + offset) * window.x, (ex_UVs.y - offset) * window.y);
    vec4 n3 = texelFetch(NormalSampler, uvs, 0);

    uvs = ivec2((ex_UVs.x - offset) * window.x, (ex_UVs.y + offset) * window.y);
    vec4 n4 = texelFetch(NormalSampler, uvs, 0);

    // Determine the estimated difference between the pixel normal and surrounding
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