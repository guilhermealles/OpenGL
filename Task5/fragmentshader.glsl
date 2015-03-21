#version 120

uniform vec3  SurfaceColor; // (0.75, 0.75, 0.75)
uniform vec3  WarmColor;    // (0.6, 0.6, 0.0)
uniform vec3  CoolColor;    // (0.0, 0.0, 0.6)
uniform float DiffuseWarm;  // 0.45
uniform float DiffuseCool;  // 0.45

varying float NdotL;
varying vec3  ReflectVec;
varying vec3  ViewVec;

void main()
{
    vec3 kcool    = min(CoolColor + DiffuseCool * SurfaceColor, 1.0);
    vec3 kwarm    = min(WarmColor + DiffuseWarm * SurfaceColor, 1.0);
    vec3 kfinal   = mix(kcool, kwarm, NdotL);
    
    vec3 nreflect = normalize(ReflectVec);
    vec3 nview    = normalize(ViewVec);
    
    float spec    = max(dot(nreflect, nview), 0.0);
    spec          = pow(spec, 32.0);
    
    gl_FragColor = vec4(min(kfinal + spec, 1.0), 1.0);
}



/*
varying vec3 normal;
varying vec3 v;
vec4 k_cool;
vec4 k_warm;
vec4 k_final;


void main (void)
{
    // Parameters
    vec4 cool_color = vec4(0, 0, 0.6, 1.0);
    vec4 warm_color = vec4(0.6, 0.6, 0, 1.0);
    float alpha = 0.45;
    float beta = 0.45;
    
    vec3 light_vector = normalize(gl_LightSource[0].position.xyz - v);
    //vec3 E = normalize(-v);
    vec3 reflection_vector = normalize(reflect(- light_vector, normal));
    normalize(normal);
    
    // b and y determine the strength of the overall temperature shift. Both between 0 and 1.
    // alpha and beta will determine the prominence of the object color and the strength of the luminance shift.
    float diffuse_component = dot(normal, light_vector);
    
    k_cool = cool_color + alpha * gl_FrontMaterial.diffuse;
    k_warm = warm_color + beta * gl_FrontMaterial.diffuse;
    k_final = mix(k_cool, k_warm, diffuse_component);
    
    float specular_component = max (dot (reflection_vector, normalize(v)), 0.0);
    specular_component = pow (specular_component, 32.0);
    
    gl_FragColor = vec4 (min(k_final + specular_component, 1.0));
    
}
*/