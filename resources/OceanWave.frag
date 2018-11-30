#version 330

// Fragment shader

// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition;
in vec3 interpSurfNormal;

// Light Position in world coordinates
uniform vec4 lightPosition;

//Textures
uniform sampler2D _bumpMap;
uniform sampler2D _reflectionTextureMap;

// Material Properties
uniform float eta; // The eta value to use initially. This reflects all light wavelengths in the same direction
uniform vec3 glassEta; // Contains one eta for each color channel (use eta.r, eta.g, eta.b in your code)
uniform float glassR0; // The Fresnel reflectivity when the incident angle is 0

uniform vec3 ambientReflectionCoeff; //light intensities
uniform vec3 specularReflectionCoeff;
uniform vec3 diffuseReflectionCoeff;

uniform vec3 ambientLightIntensity;
uniform vec3 diffuseLightIntensity;
uniform vec3 specularLightIntensity;


uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

void main() {

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
    
    //Ambient
    vec3 ambient = ambientReflectionCoeff*ambientLightIntensity;
    
    //Diffuse
    vec3 diffuse = diffuseLightIntensity * diffuseReflectionCoeff * NdotL;
    
    //Reflection from the envrionment light: Schlick’s approximation
    vec3 reflectTexCoord = normalize(reflect(-E, N)); // Takes in the incident light, reflects it around normal
    
    // Reflection color with cubeMap
    vec4 reflectionColor = texture(_reflectionTextureMap, vec2(reflectTexCoord));
    
    // Refleciton without cubeMap, only from skyColor

    // Calculate reflectance F(reflection coefficient), a percentage of how much light is reflected, (1-F) is refraction
    float LdotN = abs(dot(L, N));
    float F = glassR0 + (1-glassR0)*pow((1-LdotN),5);
    F = clamp(F,0,1);
//
//    vec3 refractTexCoord_r = normalize(refract(-E, N, glassEta.r ));
//    vec4 refractedColor_r = texture(environmentMap , refractTexCoord_r);
//
//    vec3 refractTexCoord_g = normalize(refract(-E, N, glassEta.g ));
//    vec4 refractedColor_g = texture(environmentMap , refractTexCoord_g);
//
//    vec3 refractTexCoord_b = normalize(refract(-E, N, glassEta.b));
//    vec4 refractedColor_b = texture(environmentMap , refractTexCoord_b);
//
//    vec4 final_refracted_Color = vec4(0, 0, 0, 0);
//    final_refracted_Color.r = refractedColor_r.r;
//    final_refracted_Color.g = refractedColor_g.g;
//    final_refracted_Color.b = refractedColor_b.b;

    // T: fraction refracted/absorbed. Total energy is conserved, T = 1.0 − F
	// Tell OpenGL to use the mix of the refracted and reflected color based on the fresnel term, F and T
//    fragColor.rgb = (F * reflectionColor +(1-F) * final_refracted_Color).rgb; // change me
    fragColor.rgb = (F * vec3(reflectionColor)).rgb; // change me
    // And, set the alpha component to 1.0 (completely opaque, no transparency).
    fragColor.a = 1.0;
}
