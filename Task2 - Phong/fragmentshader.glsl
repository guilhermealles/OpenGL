#version 120

varying vec3 N;
varying vec3 v;
void main (void)
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    vec3 L = normalize(gl_LightSource[0].position.xyz - v);
    vec3 E = normalize(-v);
    vec3 R = normalize(reflect(-L,N));
    
   	ambient = gl_FrontMaterial.ambient;
    diffuse = gl_FrontMaterial.diffuse * max(dot(N,L), 0.0);
   	specular = gl_FrontMaterial.specular * pow(max(dot(R,E),0.0),gl_FrontMaterial.shininess);
    
    gl_FragColor = clamp(ambient + diffuse + specular, 0.0, 1.0);
}