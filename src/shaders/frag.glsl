#version 320 es

precision highp float;
precision highp int;
precision highp sampler2D;

in vec4 gl_FragCoord;
in vec2 texcoord;

uniform sampler2D tex0;
uniform float pixel_size;
uniform bool srgb_enabled;
uniform int filter_type;
uniform float gaussian_a;

out vec3 out_color;

float PI = 3.14159265358979;

float linear_to_srgb(float c) {
    if(c <= 0.0031308) {
        return 12.92 * c;
    } else {
        return 1.055 * pow(c, 1.0/2.4) - 0.055;
    }
}

float srgb_to_linear(float c) {
    if(c <= 0.04045) {
        return c / 12.92;
    } else {
        return pow((c + 0.055) / 1.055, 2.4);
    }
}

vec3 rgb_to_srgb(vec3 rgb) {
    if (!srgb_enabled) return rgb;
    return vec3(linear_to_srgb(rgb.r), linear_to_srgb(rgb.g), linear_to_srgb(rgb.b));
}

vec3 srgb_to_rgb(vec3 rgb) {
    if (!srgb_enabled) return rgb;
    return vec3(srgb_to_linear(rgb.r), srgb_to_linear(rgb.g), srgb_to_linear(rgb.b));
}

// Note: not normalized
float gauss(float x, float a) {
    return exp(-a * x * x);
}

float sinc(float x) {
    if (abs(x) < 0.00001) return 1.0;
    return sin(PI * x) / (PI * x);
}

float lanczos(float x, float a) {
    return sinc(x) * sinc(x / a);
}

float tent(float x) {
    return 1.0 - abs(x);
}

float filter_width() {
    if (filter_type == 0) {
        return 1.0; // Tent
    } else if (filter_type == 1) {
        float sigma = 1.0 / sqrt(2.0 * gaussian_a);
        return sigma * 3.0; // Gaussian
    } else if (filter_type == 2) {
        return 3.0; // Lanczos3
    } else if (filter_type == 3) {
        return 0.5; // Box
    }
}

float filter_weight(float x) {
    if (filter_type == 0) {
        return tent(x); // Tent
    } else if (filter_type == 1) {
        return gauss(x, gaussian_a); // Gaussian
    } else if (filter_type == 2) {
        return lanczos(x, 3.0); // Lanczos3
    } else if (filter_type == 3) {
        return 1.0; // Box
    }
}

// Wrap around mirrored (back and forth)
int wrap_single(int x, int limit) {
    float fx = float(x);
    float flimit = float(limit);
    return int(round(flimit - abs(flimit - (mod(abs(fx), 2.0*flimit)))));
}

ivec2 wrap_mirrored(ivec2 x, ivec2 size) {
    return ivec2(wrap_single(x.x, size.x), wrap_single(x.y, size.y));
}

vec3 apply_filter() {
    vec3 color = vec3(0.0);
    float total_weight = 0.0;
    float scale = pixel_size;
    ivec2 texsize = textureSize(tex0, 0);
    float width = scale * filter_width();
    ivec2 start = ivec2(ceil(texcoord.x - width), ceil(texcoord.y - width));
    ivec2 end = ivec2(floor(texcoord.x + width), floor(texcoord.y + width));
    for (int y = start.y; y <= end.y; y++) {
        for (int x = start.x; x <= end.x; x++) {
            vec2 delta = (vec2(x, y) - texcoord) / scale;
            float weight = filter_weight(delta.x) * filter_weight(delta.y);
            ivec2 pos = wrap_mirrored(ivec2(x, y), texsize);
            vec3 c = texelFetch(tex0, pos, 0).xyz;
            color += srgb_to_rgb(c) * weight;
            total_weight += weight;
        }
    }
    return rgb_to_srgb(color / total_weight);
}

void main()
{
    out_color = apply_filter();
}
