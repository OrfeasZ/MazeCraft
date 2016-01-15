#version 120

varying vec2 TextureCoordinates;

// Texture
uniform sampler2D t;

// Color
uniform vec3 c;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(t, TextureCoordinates).a);
    gl_FragColor = vec4(c, 1.0) * sampled;
}