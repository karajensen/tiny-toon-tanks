////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shadow_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in vec2 ex_UVs;
in vec3 ex_Normal;

void main(void)
{
    out_Color[ID_COLOUR].rgba = vec4(0.0, 0.0, 0.0, 1.0);

    // uvs and normal will not have an affect but are required to be set here
    // as otherwise they are optimized out which causes issues with the vertex shader
    out_Color[ID_NORMAL].rgba = vec4(ex_UVs.x, ex_Normal.x, 0.0, 0.0);
}
