#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform vec2 u_resolution;
uniform sampler2D CC_Texture0;
uniform bool _first;
uniform bool fast = false;
uniform float radius = 3;

void main()
{
    float scaledRadius = radius * u_resolution.y * 0.5;
    vec2 texOffset = 1.0 / u_resolution;

    vec3 result = texture2D(CC_Texture0, v_texCoord).rgb;
    scaledRadius *= radius * 10.0 / ((radius * 10.0 + 1.0) * (radius * 10.0 + 1.0) - 1.0);
    float weight = 1.0;
    float weightSum = weight;
    if (_first) {
        for (int i = 1; float(i) < scaledRadius; i++) {
            weight -= 1.0 / scaledRadius;
            weightSum += weight * 2.0;
            result += texture2D(CC_Texture0, v_texCoord + vec2(texOffset.x * float(i), 0.0)).rgb * weight;
            result += texture2D(CC_Texture0, v_texCoord - vec2(texOffset.x * float(i), 0.0)).rgb * weight;
        }
    } else {
        for (int i = 1; float(i) < scaledRadius; i++) {
            weight -= 1.0 / scaledRadius;
            weightSum += weight * 2.0;
            result += texture2D(CC_Texture0, v_texCoord + vec2(0.0, texOffset.y * float(i))).rgb * weight;
            result += texture2D(CC_Texture0, v_texCoord - vec2(0.0, texOffset.y * float(i))).rgb * weight;
        }
    }
    
    result /= weightSum;
    gl_FragColor = vec4(result, 1.0);
}