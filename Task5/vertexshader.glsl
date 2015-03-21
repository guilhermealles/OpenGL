
uniform vec3  LightPosition;  // (0.0, 10.0, 4.0)

varying float NdotL;
varying vec3  ReflectVec;
varying vec3  ViewVec;

void main()
{
    vec3 ecPos      = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - ecPos);
    ReflectVec      = normalize(reflect(-lightVec, tnorm));
    ViewVec         = normalize(-ecPos);
    NdotL           = (dot(lightVec, tnorm) + 1.0) * 0.5;
    gl_Position     = ftransform();
}


/*
uniform vec3 LightPos;
varying vec3 N;
varying vec3 P;
varying vec3 V;
varying vec3 L;
varying vec3 ReflectVec;
varying vec3 ViewVec;


void main()
{
    vec3 ec_pos = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 t_norm = normalize (gl_NormalMatrix * gl_Normal);
    vec3 light_vector = normalize(LightPos - ec_pos);
    ReflectVec = normalize (reflect (- light_vector, t_norm));
    ViewVec = normalize (- ec_pos);
    float NdotL = dot (light_vector, t_norm) + 1.0;
    NdotL = NdotL * 0.5;
    gl_Position = ftransform();
    
}
*/