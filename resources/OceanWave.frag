#version 330

// Fragment shader

// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition;
in vec3 interpSurfNormal;
in vec2 textureCoords;

// Light Position in world coordinates
uniform vec4 lightPosition;

//Textures
uniform sampler2D _bumpMap;
uniform sampler2D _reflectionTextureMap;
uniform sampler2D _dudvMap;

// Material Properties
uniform vec3 eta; // Index of refraction
uniform float R; // The Fresnel reflectivity when the incident angle is 0

uniform vec3 ambientReflectionCoeff; //light intensities
uniform vec3 specularReflectionCoeff; //
uniform vec3 diffuseReflectionCoeff; // kd
//uniform vec3 absorptionCoeff; //Ka

//uniform vec3 C; // intrinsic color
//uniform vec3 Kr; // Proportion of energy Reflected
//uniform vec3 Kt; // Proportion of energy transmitted
//
//
//uniform vec3 It; //Intensity of transmitted light
uniform vec3 ambientLightIntensity; // Ia * kd * C + Ka
uniform vec3 diffuseLightIntensity; //
uniform vec3 specularLightIntensity; //Ir

uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

// Making water move
const float waveStrength = 0.5;

uniform float moveFactor;

void main() {
    fragColor.rgb = vec3(0.1, 0.15, 1.0);

    vec2 distortion1 = texture(_dudvMap, vec2(textureCoords.x+moveFactor, textureCoords.y+moveFactor)).rg*0.2*waveStrength;

	// Related lighting vectors
    
    vec3 E = normalize(eye_world-vec3(interpSurfPosition)); // E v: from the sur to cam
    vec3 L = normalize(vec3(lightPosition)- vec3(interpSurfPosition));//light going out
    vec3 H = normalize(L+E); //H = half-way vector
    vec3 N = normalize(interpSurfNormal);
    
    float NdotL = clamp(dot(N,L), 0.0, 1.0);
    
    float HdotN = clamp(dot(H,N), 0.0, 1.0);
    
    float EdotH = clamp(dot(E,H), 0.0, 1.0);
    
    float EdotN = clamp(dot(E,N), 0.0, 1.0);
    
    
    vec3 skyColor = vec3(0.65, 0.80, 0.95);
    
    
    // Intrinsic color
    vec3 C = vec3(0.65, 0.80, 0.95);
    
    //Ambient
    vec3 ambient = ambientReflectionCoeff*ambientLightIntensity;
    
    //Diffuse
    vec3 diffuse = diffuseLightIntensity * diffuseReflectionCoeff * NdotL;
    
    
    //Reflection from the envrionment light: Schlick’s approximation
    vec2 reflectTexCoord = vec2(normalize(reflect(-E, N))); // Takes in the incident light, reflects it around normal
    reflectTexCoord += distortion1;
    
    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);
    
    // Reflection color with cubeMap
    vec4 reflectionColor = texture(_reflectionTextureMap, vec2(reflectTexCoord));
    
    // Refleciton without cubeMap, only from skyColor

    // Calculate reflectance F(reflection coefficient), a percentage of how much light is reflected, (1-F) is refraction
//    float F = R + (1-R)*pow((1-EdotN),3);
    float F =pow((1-EdotN), 3);
    F = clamp(F,0,1);
    
    // T: fraction refracted/absorbed. Total energy is conserved, T = 1.0 − F
	// Tell OpenGL to use the mix of the refracted and reflected color based on the fresnel term, F and T
//    fragColor.rgb = (F * reflectionColor +(1-F) * final_refracted_Color).rgb; // change me

//    fragColor.rgb = (F * vec3(reflectionColor)).rgb; // change me
    fragColor.rgb = (F * vec3(reflectionColor)).rgb + fragColor.rgb + ambient + diffuse; // change me
    // And, set the alpha component to 1.0 (completely opaque, no transparency).
    fragColor.a = 1.0;
}
