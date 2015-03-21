#version 120

uniform vec3 ambient;

varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

void main() {
    const float b = 0.55;
    const float y = 0.3;
    const float Ka = 1.0;
    const float Kd = 0.8;
    const float Ks = 0.9;
    
    vec3 specularColor = vec3(1.0,1.0,1.0);
    
    vec3 norm = normalize(normal);
    vec3 L = normalize(lightVec);
    vec3 V = normalize(viewVec);
    vec3 halfAngle = normalize(L+V);
    vec4 blue = vec4(0.0, 0.0, 0.1, 1.0);
    vec4 yellow = vec4(0.4, 0.4, 0.0, 1.0);
    
    float alpha = 0.25;
    float beta = 0.5;
    
    vec4 kCool = blue + alpha * gl_FrontMaterial.diffuse ;
    vec4 kWarm = yellow + beta * gl_FrontMaterial.diffuse;
    
    //float spec = dot();
   
    /*
    float NdotL = dot(L,norm);
    float NdotH = clamp(dot(halfAngle, norm), 0.0, 1.0);
    float specular = pow(NdotH, 64.0);
    
    float blendval = 0.5 * NdotL + 0.5;
    vec3 Cgooch = mix(kWarm, kCool, blendval);
    
    vec3 result = Kd * Cgooch + specularColor * Ks * specular;
     */
    
    vec4 result = vec4(kCool + kWarm);
    
    gl_FragColor = result;
}