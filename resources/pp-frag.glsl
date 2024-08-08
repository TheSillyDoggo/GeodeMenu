#ifdef GL_ES
    #ifdef GL_FRAGMENT_PRECISION_HIGH
        precision highp float;
    #else
        precision mediump float;
    #endif
#endif

#if __VERSION__ == 300
    #version 300 es
    in vec2 TexCoords;
    out vec4 FragColor;
#else
    varying vec2 TexCoords;
#endif

uniform sampler2D screen;
uniform vec2 screenSize;
uniform float radius;
uniform bool first;

void main() {
    // Compute scaled radius
    float scaledRadius = clamp(radius * screenSize.y * 0.5, 1.0, 32.0);
    vec2 texOffset = 1.0 / screenSize;

    // Initialize result with the center pixel
    vec3 result = texture2D(screen, TexCoords).rgb;
    float weightSum = 1.0;
    float weight = 1.0;

    // Precompute inverse radius for weight reduction
    float inverseRadius = 1.0 / scaledRadius;

    for (float i = 1.0; i < scaledRadius; i++) {
        weight -= inverseRadius;
        weightSum += weight * 2.0;

        if (first) {
            vec2 offset = vec2(texOffset.x * i, 0.0);
            result += texture2D(screen, TexCoords + offset).rgb * weight;
            result += texture2D(screen, TexCoords - offset).rgb * weight;
        } else {
            vec2 offset = vec2(0.0, texOffset.y * i);
            result += texture2D(screen, TexCoords + offset).rgb * weight;
            result += texture2D(screen, TexCoords - offset).rgb * weight;
        }
    }

    // Normalize the result by the total weight
    result /= weightSum;

#if __VERSION__ == 300
    FragColor = vec4(result, 1.0);
#else
    gl_FragColor = vec4(result, 1.0);
#endif
}