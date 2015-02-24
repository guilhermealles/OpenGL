uniform vec4 ambientMat;
uniform vec4 diffuseMat;
uniform vec4 specMat;
uniform float specPow;

varying vec3 N;
varying vec3 v;

void main (void)
{
    vec4 diffuse;
    vec4 spec;
    vec4 ambient;
    
    vec3 L = normalize(gl_LightSource[0].position.xyz - v);
    vec3 E = normalize(-v);
    vec3 R = normalize(reflect(-L,N));
    
   	ambient = ambientMat;
    
    diffuse = clamp( diffuseMat * max(dot(N,L), 0.0)  , 0.0, 1.0 ) ;
   	spec = clamp ( specMat * pow(max(dot(R,E),0.0),0.3*specPow) , 0.0, 1.0 );
    
    //gl_FragColor = vec4(0.5*normalize(N)+0.5,1.0);
    gl_FragColor = ambient + diffuse + spec;
}