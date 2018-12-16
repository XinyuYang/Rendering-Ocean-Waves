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
uniform sampler2D _normalMap;


uniform vec3 lightColor;

// Material Properties
uniform float R; // The Fresnel reflection coefficient when the incident angle is 0

uniform vec3 ambientReflectionCoeff; //light intensities
uniform vec3 specularReflectionCoeff; //
uniform vec3 diffuseReflectionCoeff; // kd
uniform vec3 sunReflectionCoeff;
uniform vec3 sunColor;

//uniform vec3 C; // intrinsic color
//uniform vec3 Kr; // Proportion of energy Reflected
//uniform vec3 Kt; // Proportion of energy transmitted

//uniform vec3 It; //Intensity of transmitted light
uniform vec3 ambientLightIntensity; // Ia * kd * C + Ka
uniform vec3 diffuseLightIntensity; //
uniform vec3 specularLightIntensity; //Ir

uniform vec3 eye_world;

// This is an out variable for the final color we want to render this fragment.
out vec4 fragColor;

//// Making water move
const float waveStrength = 0.5;
const float shineDamper = 20.0;
const float reflectivity = 0.6;

uniform float moveFactor;

void main() {
    fragColor.rgb = vec3(0.1, 0.25, .45);

    vec2 distortion1 = texture(_bumpMap, vec2(textureCoords.x+moveFactor, textureCoords.y+moveFactor)).rg*0.2*waveStrength;

	// Related lighting vectors
    
    vec2 distortedTexCoords = texture(_normalMap, vec2(textureCoords.x, textureCoords.y)).rg;

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
    
    // Reflection color caused by envrionment: sky reflection, sun light, and local reflection
//   vec3 C_result = C_skylight+C_sunlight+C_localreflection

    // Sky reflection is baed on envrionment mapping which stores cloud thickness in the alpha channel Ccloud*alpha for various atmospheric conditions
//    float cloudThinkness = 2.0;
//    C_skyLight = SkyColor *(1-C_cloud * cloudThickness)+ C_cloud.rgb *C_cloud

    //Reflection from the envrionment map: Schlick’s approximation
    vec2 reflectTexCoord = vec2(normalize(reflect(-E, N))); // Takes in the incident light, reflects it around normal
    reflectTexCoord += distortion1;
//
//    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
//    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

    // Reflection color
    vec4 reflectionColor = texture(_reflectionTextureMap, vec2(reflectTexCoord));

    // Calculate reflectance F(reflection coefficient), a percentage of how much light is reflected, (1-F) is refraction
//    float F = R + (1-R)*pow((1-EdotN),3);
    float F = 0.02+0.98*pow((1-EdotN), 5);
    F = clamp(F,0,1);
    
    vec4 normalMapColor = texture(_normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r , normalMapColor.b, normalMapColor.g);
    normal = normalize(normal);

    vec3 reflectedLight = reflect(normalize(L), N);
    float specular = max(dot(reflectedLight, E), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity;

    // T: fraction refracted/absorbed. Total energy is conserved, T = 1.0 − F
	// Tell OpenGL to use the mix of the refracted and reflected color based on the fresnel term, F and T
//    fragColor.rgb = (F * reflectionColor +(1-F) * final_refracted_Color).rgb; // change me

//    fragColor.rgb = (F * vec3(reflectionColor)).rgb; // change me
    fragColor.rgb = (F * vec3(reflectionColor)).rgb + fragColor.rgb; // change me
//    fragColor.rgb = vec3(normalMapColor);


    // And, set the alpha component to 1.0 (completely opaque, no transparency).
    fragColor.a = 1.0;
}
