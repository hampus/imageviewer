#version 320 es

in highp vec4 gl_FragCoord;
in highp vec3 color;

out highp vec3 out_color;

void main()
{
    out_color = color;
}
