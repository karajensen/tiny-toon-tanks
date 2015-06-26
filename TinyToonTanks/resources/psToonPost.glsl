////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - psToonPost.glsl
////////////////////////////////////////////////////////////////////////////////////////

uniform sampler2D TexSampler;
uniform sampler2D NormalSampler;
varying vec2 UV;

void main()
{
    float offset = 0.0009;

    //sample normals of four corners around pixel
    vec4 scene = texture2D(TexSampler, UV);
    vec4 n1 = texture2D(NormalSampler, UV + vec2(-offset, -offset));
    vec4 n2 = texture2D(NormalSampler, UV + vec2( offset,  offset));
    vec4 n3 = texture2D(NormalSampler, UV + vec2(-offset,  offset));
    vec4 n4 = texture2D(NormalSampler, UV + vec2( offset, -offset));
    
    //determine the estimated difference between the pixel normal and surrounding
    vec4 diagonal1 = abs(n1 - n2);
    vec4 diagonal2 = abs(n3 - n4);
    vec4 diagonalDelta = diagonal1 + diagonal2;
    float edgeAmount = clamp(length(diagonalDelta), 0.0, 1.0);

    scene *= 1.0 - edgeAmount;
    gl_FragColor = scene;
}
