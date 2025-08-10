#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord; // Texture coordinates, not used in this shader but can be used for texture mapping

out vec3 ourColor; //Data like color is passed from vertex shader to fragment shader using out/in variables.
out vec2 TexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;




void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // add the xOffset to the x position of the vertex position
    ourColor = aColor;
    TexCoord = aTexCoord;

}
