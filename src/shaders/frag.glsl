#version 320 es

in highp vec4 gl_FragCoord;
in highp vec2 texcoord;

uniform sampler2D tex0;
uniform highp float pixel_size;
uniform bool srgb_enabled;
uniform bool gaussian_enabled;
uniform highp float gaussian_sigma;

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

highp vec3 gaussian_filter() {
    highp vec3 color = vec3(0.0);
    highp float total_weight = 0.0;
    highp float a = 1.0 / gaussian_sigma;
    ivec2 texsize = textureSize(tex0, 0);
    ivec2 start = ivec2(ceil(texcoord.x - gaussian_sigma*3.0), ceil(texcoord.y - gaussian_sigma*3.0));
    ivec2 end = ivec2(floor(texcoord.x + gaussian_sigma*3.0), floor(texcoord.y + gaussian_sigma*3.0));
    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            highp vec2 delta = (vec2(x, y) - texcoord);
            highp float weight = exp(-a*a*(delta.x*delta.x + delta.y*delta.y));
            highp ivec2 pos = ivec2(x, y);
            if (pos.x >= texsize.x) pos.x = texsize.x*2 - pos.x;
            if (pos.x < 0) pos.x = -pos.x;
            if (pos.y >= texsize.y) pos.y = texsize.y*2 - pos.y;
            if (pos.y < 0) pos.y = -pos.y;
            highp vec3 c = texelFetch(tex0, pos, 0).xyz;
            color += srgb_to_rgb(c) * weight;
            total_weight += weight;
        }
    }
    return rgb_to_srgb(color / total_weight);
}

highp vec3 box_filter() {
    highp vec3 color = vec3(0.0);
    highp float total_weight = 0.0;
    ivec2 start = ivec2(ceil(texcoord.x - pixel_size*0.5), ceil(texcoord.y - pixel_size*0.5));
    ivec2 end = ivec2(floor(texcoord.x + pixel_size*0.5), floor(texcoord.y + pixel_size*0.5));
    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            highp vec3 c = texelFetch(tex0, ivec2(x, y), 0).xyz;
            color += srgb_to_rgb(c);
            total_weight += 1.0;
        }
    }
    return rgb_to_srgb(color / total_weight);
}


void main()
{
    if (gaussian_enabled) {
        out_color = gaussian_filter();
    } else {
        out_color = box_filter();
    }
}
