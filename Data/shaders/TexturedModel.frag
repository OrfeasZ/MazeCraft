#version 120

// Inputs from Vertex Shader
varying vec2 UV;
varying vec4 Color;
varying vec3 ModelNormals;
varying vec3 ModelVertices;
varying mat4 Model;

// Camera Position
uniform vec3 cp;

// Light Count
uniform int lc;

// Material Shininess
uniform float ms;

// Texture
uniform sampler2D t;

// Lights
uniform struct LightSource
{
   vec3 p; // Position
   vec3 c; // Color
   float a; // Attenuation
   float ac; // Ambient Coefficient
} l[5]; // Allow up to 5 lights.

// Custom inverse function since 1.2 doesn't provide it.
mat3 inverse(mat3 m)
{
   mat3 adj;
   adj[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]);
   adj[1][0] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]);
   adj[2][0] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
   adj[0][1] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]);
   adj[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]);
   adj[2][1] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]);
   adj[0][2] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
   adj[1][2] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]);
   adj[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
   float det = (+ m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
        - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
        + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
   return adj / det;
}

vec3 CombineLight(LightSource p_Light, vec3 p_SurfaceColor, vec3 p_Normal, vec3 p_SurfacePosition, vec3 p_SurfaceToCamera) 
{
    vec3 s_SurfaceToLight = normalize(p_Light.p - p_SurfacePosition);
    float s_DistanceToLight = length(p_Light.p - p_SurfacePosition);
    float s_Attenuation = 1.0 / (1.0 + p_Light.a * pow(s_DistanceToLight, 2));

    // Calculate Ambient.
    vec3 s_Ambient = p_Light.ac * p_SurfaceColor.rgb * p_Light.c;

    // Calculate Diffuse.
    float s_DiffuseCoefficient = max(0.0, dot(p_Normal, s_SurfaceToLight));
    vec3 s_Diffuse = p_SurfaceColor.rgb * p_Light.c * s_DiffuseCoefficient;
    
    // Calculate Specular.
    float s_SpecularCoefficient = 0.0;

    if (s_DiffuseCoefficient > 0.0)
        s_SpecularCoefficient = pow(max(0.0, dot(p_SurfaceToCamera, reflect(-s_SurfaceToLight, p_Normal))), ms);
    
    // NOTE: Right now specular color is black because we don't need reflections.
    // TODO: Change this to use a proper specular material.
    vec3 s_Specular = s_SpecularCoefficient * p_Light.c * vec3(0.0, 0.0, 0.0);

    // Calculate Final Color.
    return (s_Attenuation * (s_Specular + s_Diffuse)) + s_Ambient;
}

void main()
{
	vec3 s_Normal = normalize(transpose(inverse(mat3(Model))) * ModelNormals);

    vec3 s_SurfacePosition = vec3(Model * vec4(ModelVertices, 1));
    vec3 s_SurfaceToCamera = normalize(cp - s_SurfacePosition);

    vec4 s_SurfaceColor = vec4(texture2D(t, UV).rbg, 1.0) * Color;

    // Combine all light sources.
    vec3 s_FinalColor = vec3(0);

    for (int i = 0; i < lc; ++i)
    {
        s_FinalColor += CombineLight(l[i], s_SurfaceColor.rgb, s_Normal, s_SurfacePosition, s_SurfaceToCamera);
    }
    
    // Final color post-gamma correction.   v (gamma correction)
    gl_FragColor = vec4(pow(s_FinalColor, vec3(1.0 / 2.2)), s_SurfaceColor.a);
}