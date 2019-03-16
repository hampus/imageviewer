#version 320 es

in highp vec4 gl_FragCoord;
in highp vec2 texcoord;

uniform sampler2D tex0;
uniform highp float pixel_size;
uniform bool srgb_enabled;

out highp vec3 out_color;

highp float linear_to_srgb(highp float c) {
    if(c <= 0.0031308) {
        return 12.92 * c;
    } else {
        return 1.055 * pow(c, 1.0/2.4) - 0.055;
    }
}

highp float srgb_to_linear(highp float c) {
    if(c <= 0.04045) {
        return c / 12.92;
    } else {
        return pow((c + 0.055) / 1.055, 2.4);
    }
}

highp vec3 rgb_to_srgb(vec3 rgb) {
    if (!srgb_enabled) return rgb;
    return vec3(linear_to_srgb(rgb.r), linear_to_srgb(rgb.g), linear_to_srgb(rgb.b));
}

highp vec3 srgb_to_rgb(vec3 rgb) {
    if (!srgb_enabled) return rgb;
    return vec3(srgb_to_linear(rgb.r), srgb_to_linear(rgb.g), srgb_to_linear(rgb.b));
}

void main()
{
    out_color = texelFetch(tex0, ivec2(texcoord), 0).xyz;
}
