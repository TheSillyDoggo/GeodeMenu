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
uniform bool fast;
uniform bool first;
uniform float radius;

void main() {
    float scaledRadius = radius * screenSize.y * 0.5;
    vec2 texOffset = 1.0 / screenSize;

    //float texelRounding = screenSize.y / 4;

    // vec2 _texCoords = floor(TexCoords * texelRounding) / texelRounding;
    vec2 _texCoords = TexCoords;

    vec3 result = texture2D(screen, _texCoords).rgb;
    scaledRadius *= radius * 10.0 / ((radius * 10.0 + 1.0) * (radius * 10.0 + 1.0) - 1.0);
    float weight = 1.0;
    float weightSum = weight;
    if (first) {
        for (int i = 1; float(i) < scaledRadius; i++) {
            weight -= 1.0 / scaledRadius;
            weightSum += weight * 2.0;
            result += texture2D(screen, _texCoords + vec2(texOffset.x * float(i), 0.0)).rgb * weight;
            result += texture2D(screen, _texCoords - vec2(texOffset.x * float(i), 0.0)).rgb * weight;
        }
    } else {
        for (int i = 1; float(i) < scaledRadius; i++) {
            weight -= 1.0 / scaledRadius;
            weightSum += weight * 2.0;
            result += texture2D(screen, _texCoords + vec2(0.0, texOffset.y * float(i))).rgb * weight;
            result += texture2D(screen, _texCoords - vec2(0.0, texOffset.y * float(i))).rgb * weight;
        }
    }
    result /= weightSum;

#if __VERSION__ == 300
    FragColor = vec4(result, 1.0);
#else
    gl_FragColor = vec4(result, 1.0);
#endif
}