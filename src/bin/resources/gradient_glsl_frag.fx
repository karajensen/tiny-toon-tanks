////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gradient_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in vec2 ex_UVs;

void main(void)
{
    out_Color[ID_COLOUR].rgba = vec4(1.0, 1.0, 1.0, ex_UVs.y);
    out_Color[ID_NORMAL].rgba = vec4(0.0, 0.0, 0.0, 0.0);
}