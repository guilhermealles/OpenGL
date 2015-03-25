#version 120
varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

void main()
{
    const float b = 0.55;
    const float y = 0.3;
    
    vec3 norm = normalize(normal);
    vec3 L = normalize(lightVec);
    vec3 V = normalize(viewVec);
<<<<<<< HEAD
    vec3 half_angle = normalize(L+V);
    vec3 reflect_vector = reflect(-L, norm);
    vec4 blue = vec4(0.0, 0.0, 0.55, 1.0);
    vec4 yellow = vec4(0.3, 0.3, 0.0, 1.0);
=======
    vec3 halfAngle = normalize(L+V);
    vec4 blue = vec4(0.0, 0.0, 0.1, 1.0);
    vec4 yellow = vec4(0.4, 0.4, 0.0, 1.0);
    
    float alpha = 0.25;
    float beta = 0.5;
>>>>>>> origin/master
    
    float alpha = 0.25;
    float beta = 0.5;
    
    vec4 k_cool = blue + alpha * gl_FrontMaterial.diffuse ;
    vec4 k_warm = yellow + beta * gl_FrontMaterial.diffuse;
    
    float spec = max (dot (normalize(reflect_vector), V), 0.0);
    spec = pow (spec, 64.0);

    vec4 result = vec4(k_cool * (1 - dot(norm, L))/2 + k_warm * (1 + dot(norm, L))/2 + spec);
    
    gl_FragColor = result;
}