#version 150
 
precision highp float; // Video card drivers require this line to function properly

uniform vec3 sun_dir;

in vec3 frag_color;
in vec3 light_normal;
out vec4 final_color;

void main()
{
   	final_color = vec4(frag_color * (length(dot(sun_dir, light_normal))/2+0.5), 1.0);
   	//final_color = vec4(0.5+light_normal.x/2, 0.5+light_normal.y/2, 0.5+light_normal.z/2);
   	//final_color = vec4(0.5+(light_normal.x/2.0), 0.5+(light_normal.x/2.0), 0.5+(light_normal.x/2.0), 1.0);
}