#version 150

uniform float ambient;

in vec3 frag_color;
out vec4 final_color;

void main()
{
   	final_color = vec4(frag_color * (ambient+0.5), 1.0);
}