////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in vec2 ex_UVs;

uniform sampler2D DiffuseSampler;
 
void main(void)
{
    out_Color[ID_COLOUR].rgba = texture(DiffuseSampler, ex_UVs).rgba;
    out_Color[ID_NORMAL].rgba = vec4(0.0, 0.0, 0.0, out_Color[ID_COLOUR].a);
}