#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform int visibleTexture;
uniform vec4 albedo;

void main()
{
	if(visibleTexture == 1){
		FragColor = texture(texture0,texCoord);
	}
	else{
		FragColor = albedo;
	}

	//FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}