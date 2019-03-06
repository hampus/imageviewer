#version 320 es

in highp vec4 gl_FragCoord;
in highp vec2 position;

out highp vec3 out_color;

void main()
{
    out_color = vec3(position.x*0.5f + 0.5f, position.y*0.5f + 0.5f, 0.0f);
}
