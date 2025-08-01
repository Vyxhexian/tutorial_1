#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 aOurPos;

void main()
{
    FragColor = vec4(aOurPos, 1.0f);
}