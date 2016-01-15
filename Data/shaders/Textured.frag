#version 120

varying vec2 TextureCoordinates;

uniform sampler2D t;

void main()
{
    gl_FragColor = texture2D(t, TextureCoordinates);
}