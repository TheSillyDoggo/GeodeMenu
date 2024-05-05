#version 300 es

precision highp float;

in vec2 TexCoords;

uniform sampler2D screen;
uniform vec2 screenSize;
uniform bool fast;
uniform bool first;
uniform float radius;

out vec4 FragColor;

void main() {
    float scaledRadius = radius * screenSize.y * 0.5;
    vec2 texOffset = 1.0 / screenSize; // gets size of single texel

    vec3 result = texture(screen, TexCoords).rgb;
    if (fast) {
        scaledRadius *= radius * 10.0 / ((radius * 10.0 + 1.0) * (radius * 10.0 + 1.0) - 1.0);
        float weightSum = 1.0;
        float weightDelta = 1.0 / scaledRadius;
        vec3 sampleOffset = vec3(texOffset, 0.0);

        if (!first) {
            sampleOffset.xy *= -1.0;
        }

        for (int i = 1; float(i) < scaledRadius; i++) {
            vec2 offset = texOffset * float(i);
            vec3 sampleColor = texture(screen, TexCoords + offset).rgb + texture(screen, TexCoords - offset).rgb;
            result += sampleColor * (1.0 - weightSum);
            weightSum += weightDelta;
        }

        result /= weightSum;
    }
    else {
        float firstWeight = 0.84089642 / pow(scaledRadius, 0.96);
        result *= firstWeight;
        float weightSum = firstWeight;

        for (int i = 1; float(i) <= ceil(scaledRadius); i++) {
            float weight = firstWeight * exp(-float(i) * float(i) / (2.0 * scaledRadius));
            weightSum += weight * 2.0;
            vec2 offset = texOffset * float(i);
            vec3 sampleColor = texture(screen, TexCoords + offset).rgb + texture(screen, TexCoords - offset).rgb;
            result += sampleColor * weight;
        }

        result /= weightSum;
    }

    FragColor = vec4(result, 1.0);
}