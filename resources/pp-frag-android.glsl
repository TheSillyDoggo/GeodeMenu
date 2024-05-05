#version 300 es

precision highp float;

in vec2 TexCoords;

uniform sampler2D screen;
uniform vec2 screenSize;
uniform bool fast;
uniform bool first;
uniform float radius;

out vec4 FragColor;

const float PI = 3.14159265359;
const float epsilon = 1e-5;

// Standard Gaussian Distribution
float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * PI) * sigma);
}

void main() {
    float scaledRadius = radius * screenSize.y * 0.5;
    vec2 texOffset = 1.0 / screenSize; // gets size of single texel

    vec3 result = texture(screen, TexCoords).rgb;
    
    if (fast) {
        scaledRadius *= radius * 10.0 / ((radius * 10.0 + 1.0) * (radius * 10.0 + 1.0) - 1.0);
    }

    float weightSum = 1.0;
    float weightDelta = 1.0 / scaledRadius;
    vec3 sampleColor = result;

    for (int i = 1; i < scaledRadius + epsilon; i++) {
        float weight = gaussian(float(i), scaledRadius);
        weightSum += weight * 2.0;

        if (!fast && i <= ceil(scaledRadius)) {
            vec2 offset = texOffset * float(i);
            vec3 color1 = texture(screen, TexCoords + offset).rgb;
            vec3 color2 = texture(screen, TexCoords - offset).rgb;
            sampleColor += color1 + color2;
        }

        if (fast || i > ceil(scaledRadius)) {
            result += sampleColor * weight;
            sampleColor = texture(screen, TexCoords + texOffset * float(i)).rgb +
                          texture(screen, TexCoords - texOffset * float(i)).rgb;
        }
    }

    result /= weightSum;

    FragColor = vec4(result, 1.0);
}