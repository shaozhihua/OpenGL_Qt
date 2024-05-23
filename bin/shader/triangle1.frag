#version 450 core
in vec4 outColor;
out vec4 FragColor;

uniform vec4 uniColor;

void main()
{
    // FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
    //FragColor = outColor;
    FragColor = uniColor;
}
