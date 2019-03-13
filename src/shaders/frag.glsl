#version 320 es

in highp vec4 gl_FragCoord;
in highp vec2 position;

uniform sampler2D tex0;
uniform highp float pixel_width;
uniform highp float pixel_height;
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
    highp vec3 sum = vec3(0.0, 0.0, 0.0);
    for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 6; y++) {
            highp vec2 offset = vec2(pixel_width * float(x), pixel_height * float(y)) / 6.0;
            sum += srgb_to_rgb(texture(tex0, position*vec2(0.5, -0.5) + vec2(0.5, 0.5) + offset).xyz);
        }
    }
    highp vec3 rgb_color = sum / 36.0;
    out_color = rgb_to_srgb(rgb_color);
}
