#version 320 es

out highp vec4 gl_Position;
out highp vec2 texcoord;

in highp vec2 in_position;
in highp vec2 in_texcoord;

uniform highp mat4 transform_pos;
uniform highp vec2 image_size;

void main()
{
    gl_Position = transform_pos * vec4(in_position, 0.0f, 1.0f);
    texcoord = in_texcoord * (image_size - vec2(1.0, 1.0));
}
