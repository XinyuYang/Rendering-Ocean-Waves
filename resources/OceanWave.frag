#version 330

// Fragment shader

// These get passed in from the vertex shader and are interpolated (varying) properties,
// so they change for each pixel across the triangle:
in vec4 interpSurfPosition;
in vec3 interpSurfNormal;

// Light Position in world coordinates
uniform vec4 lightPosition;

//Textures
uniform samplerCube environmentMap;

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

	// TODO: fill this in

    vec3 E = normalize(eye_world-vec3(interpSurfPosition)); // E v: from the sur to cam
    vec3 L = normalize(vec3(interpSurfPosition)-vec3(lightPosition));
    vec3 u = -L; //u (light going out) = negative of incident_dir
    vec3 H = normalize(u+E); //H = half vector
    vec3 N = normalize(interpSurfNormal);

    // 3. Reflection from the envrionment light: Schlick’s approximation
    vec3 reflectTexCoord = normalize(reflect(-E, N)); // Takes in the incident light, reflects it around normal
    vec4 reflectionColor = texture(environmentMap, reflectTexCoord);

    // F: reflectance (reflection coefficient), a percentage of how much light is reflected, (1-F) is refraction
    float L_proj_N = abs(dot(u, N));
    float F = glassR0 + (1-glassR0)*pow((1-L_proj_N),5);
    F = clamp(F,0,1);

    vec3 refractTexCoord_r = normalize(refract(-E, N, glassEta.r ));
    vec4 refractedColor_r = texture(environmentMap , refractTexCoord_r);
    
    vec3 refractTexCoord_g = normalize(refract(-E, N, glassEta.g ));
    vec4 refractedColor_g = texture(environmentMap , refractTexCoord_g);
    
    vec3 refractTexCoord_b = normalize(refract(-E, N, glassEta.b));
    vec4 refractedColor_b = texture(environmentMap , refractTexCoord_b);

    vec4 final_refracted_Color = vec4(0, 0, 0, 0);
    final_refracted_Color.r = refractedColor_r.r;
    final_refracted_Color.g = refractedColor_g.g;
    final_refracted_Color.b = refractedColor_b.b;

    // T: fraction refracted/absorbed. Total energy is conserved, T = 1.0 − F
    
	// Tell OpenGL to use the mix of the refracted and reflected color based on the fresnel term, F and T
    fragColor.rgb = (F * reflectionColor +(1-F) * final_refracted_Color).rgb; // change me
//    fragColor.rgb = vec4(1, 0, 1, 0.5); // change me
	// And, set the alpha component to 1.0 (completely opaque, no transparency).
	fragColor.a = 1.0;
}
