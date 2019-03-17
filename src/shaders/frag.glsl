#version 320 es

in highp vec4 gl_FragCoord;
in highp vec2 texcoord;

uniform sampler2D tex0;
uniform highp float pixel_size;
uniform bool srgb_enabled;
uniform int filter_type;
uniform highp float gaussian_sigma;

out highp vec3 out_color;

highp float PI = 3.14159265358979;

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

// Note: not normalized
highp float gauss(highp float x, highp float a) {
    return exp(-a * x * x);
}

highp float sinc(highp float x) {
    if (abs(x) < 0.00001) return 1.0;
    return sin(PI * x) / (PI * x);
}

highp float lanczos(highp float x, highp float a) {
    return sinc(x) * sinc(x / a);
}

highp float tent(highp float x) {
    return 1.0 - abs(x);
}

highp vec3 gaussian_filter() {
    highp vec3 color = vec3(0.0);
    highp float total_weight = 0.0;
    highp float a = 1.0 / (2.0 * gaussian_sigma * gaussian_sigma);
    ivec2 texsize = textureSize(tex0, 0);
    ivec2 start = ivec2(ceil(texcoord.x - gaussian_sigma*3.0), ceil(texcoord.y - gaussian_sigma*3.0));
    ivec2 end = ivec2(floor(texcoord.x + gaussian_sigma*3.0), floor(texcoord.y + gaussian_sigma*3.0));
    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            highp vec2 delta = (vec2(x, y) - texcoord);
            highp float weight = gauss(delta.x, a) * gauss(delta.y, a);
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

highp vec3 lanczos_filter(highp float a) {
    highp vec3 color = vec3(0.0);
    highp float total_weight = 0.0;
    highp float scale = pixel_size;
    ivec2 texsize = textureSize(tex0, 0);
    ivec2 start = ivec2(ceil(texcoord.x - a*scale), ceil(texcoord.y - a*scale));
    ivec2 end = ivec2(floor(texcoord.x + a*scale), floor(texcoord.y + a*scale));
    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            highp vec2 delta = (vec2(x, y) - texcoord) / scale;
            highp float weight = lanczos(delta.x, a) * lanczos(delta.y, a);
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

highp vec3 tent_filter() {
    highp vec3 color = vec3(0.0);
    highp float total_weight = 0.0;
    highp float scale = pixel_size;
    ivec2 texsize = textureSize(tex0, 0);
    ivec2 start = ivec2(ceil(texcoord.x - scale), ceil(texcoord.y - scale));
    ivec2 end = ivec2(floor(texcoord.x + scale), floor(texcoord.y + scale));
    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            highp vec2 delta = (vec2(x, y) - texcoord) / scale;
            highp float weight = tent(delta.x) * tent(delta.y);
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
    if (filter_type == 0) {
        out_color = tent_filter();
    } else if (filter_type == 1) {
        out_color = gaussian_filter();
    } else if (filter_type == 2) {
        out_color = lanczos_filter(3.0);
    } else if (filter_type == 3) {
        out_color = box_filter();
    }
}
