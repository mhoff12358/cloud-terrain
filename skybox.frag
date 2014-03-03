#version 110 // Specify which version of GLSL we are using.
 
precision highp float; // Video card drivers require this line to function properly

void main() 
{
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}