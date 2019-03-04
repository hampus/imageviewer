#version 320 es

out highp vec4 gl_Position;
out highp vec3 color;

in highp vec2 in_position;
in highp vec3 in_color;

void main()
{
    gl_Position = vec4(in_position, 0.0f, 1.0f);
    color = in_color;
}
