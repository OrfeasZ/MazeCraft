#version 120

varying vec2 TextureCoordinates;

// Projection
uniform mat4 p;

// Position
attribute vec2 v;

// UVs
attribute vec2 tc;

void main() 
{
	TextureCoordinates = tc;
	gl_Position = p * vec4(v, 0.0, 1.0);
}