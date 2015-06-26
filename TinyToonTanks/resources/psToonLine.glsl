////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - psToonLine.glsl
////////////////////////////////////////////////////////////////////////////////////////

varying float alpha;
varying vec3 VertexNormal;
void main()
{
    vec3 normal = normalize(VertexNormal);
    vec4 DiffuseColor;
    DiffuseColor.r = normal.x;
    DiffuseColor.g = normal.y;
    DiffuseColor.b = normal.z;
    DiffuseColor.a = alpha;
    gl_FragColor = DiffuseColor;
}
