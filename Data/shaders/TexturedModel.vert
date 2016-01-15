#version 120

// Inputs
attribute vec3 mv;
attribute vec2 mu;
attribute vec3 mn;

// Outputs
varying vec2 UV;
varying vec4 Color;
varying vec3 ModelNormals;
varying vec3 ModelVertices;
varying mat4 Model;

// Projection
uniform mat4 p;

// View
uniform mat4 v;

// Model
uniform mat4 m;

// Color
uniform vec4 c;

void main()
{
	UV = mu;
	Color = c;
	ModelNormals = mn;
	ModelVertices = mv;
	Model = m;

	// Final position.
	gl_Position =  (p * v * m) * vec4(mv, 1);
}