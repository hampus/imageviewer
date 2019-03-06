#version 320 es

out highp vec4 gl_Position;
out highp vec2 position;

in highp vec2 in_position;

void main()
{
    gl_Position = vec4(in_position, 0.0f, 1.0f);
    position = in_position;
}