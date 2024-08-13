#ifdef GL_ES
    #ifdef GL_FRAGMENT_PRECISION_HIGH
        precision highp float;
    #else
        precision mediump float;
    #endif
#endif

#if __VERSION__ == 300
    layout(location = 0) in vec2 aPosition;
    layout(location = 1) in vec2 aTexCoords;
    out vec2 TexCoords;
#else
    attribute vec2 aPosition;
    attribute vec2 aTexCoords;
    varying vec2 TexCoords;
#endif

void main() {
    gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}
