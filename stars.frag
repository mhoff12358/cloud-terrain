#version 150
 
precision highp float; // Video card drivers require this line to function properly

out vec4 final_color;

void main()
{
	final_color = vec4(1.0, 1.0, 1.0, 1.0);
}