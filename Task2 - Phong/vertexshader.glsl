#version 120

varying vec3 N;

void main()
{
    
    N = gl_Normal;
    
	gl_Position = ftransform();
}
