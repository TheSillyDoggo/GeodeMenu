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
    vec4 tex1Color = texture(CC_Texture0, v_texCoord.xy / u_resolution.xy);
    
    const int kSize = 5;
    vec3 avg = vec3(0.0, 0.0, 0.0);
    for (int i=-kSize; i <= kSize; ++i) {
        for (int j = -kSize; j <= kSize; ++j) {
            avg = avg + texture(CC_Texture0, (v_texCoord.xy + vec2(float(i), float(j)))/u_resolution.xy).xyz;
        }
    }
    int area = (2*kSize + 1) * (2*kSize + 1);
    avg = avg.xyz/vec3(area, area, area);
    
	fragColor = vec4(avg, tex1Color.a);
}