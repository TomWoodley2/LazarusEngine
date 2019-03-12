#version 430 core 

out vec4 FragColor; 

uniform vec3 colour;
								  
void main()						  
{		
	//vec3 colour = vec3(1.0f,0.0f,0.f);
	FragColor = vec4(colour, 1.0f); 
}									  