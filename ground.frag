#version 150 // Specify which version of GLSL we are using.
 
//precision highp float; // Video card drivers require this line to function properly

in vec3 frag_color;
out vec4 final_color;

void main() 
{
    //final_color = vec4(frag_color, 1.0);
    final_color = vec4(0.0, 0.0, 1.0, 1.0);
}