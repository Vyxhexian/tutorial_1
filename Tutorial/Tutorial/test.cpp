#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// resolution settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;




int main()
{




	glfwInit(); // First we initialize GLFW 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Configurations
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //1 created glfw obj

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return-1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //2 





	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return-1;
	}


	//shader program set up.
	shader ourShader("shader.vs", "shader.fs");



	//Input vertex data
	float vertices[] = {
		// positions       //colors          //texture coords
		 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		 0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
	};

	unsigned int indices[] = { //we start from 0
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};





	//VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); //ID

	glBindVertexArray(VAO); //bind



	//VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO); // generate ID

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //upload vertex data

	 

	//EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO); //ID


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	//Linking Vertex Attributes
	//what part of input data goes to which vertex attribute in vertex shader

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //stride is 24 now ,
	glEnableVertexAttribArray(0); // 0 = location 0

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  //at the end offset is 12 
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //at the end offset is 24 
	glEnableVertexAttribArray(2);






	unsigned int texture;
	glGenTextures(1, &texture); //ID

	glBindTexture(GL_TEXTURE_2D, texture); //bind


	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);





	//load texture
	int width, height, nrChannels;
	std::string path = "C:/assets/wall.jpg";

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);


	// 
	if (data)
	{
		//generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}


	//free memory after
	stbi_image_free(data);










	// those 2 unbinding of VBO and VAO allowed, (from tutorial code)
	// 
	// 
	// 
	// ---------------------------------------------------------
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);//you can safely unbind the VBO because the VAO has stored everything it needs.


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0); //Some developers like to unbind to prevent accidental modification, others just leave it bound.
	// ---------------------------------------------------------





	//--------------RENDER LOOP-----------------------
	//render loop ( keep drawing images and handling user input until the program has been explicitly told to stop.)
	//it's like update in unity
	while (!glfwWindowShouldClose(window))
	{
		// input 
		processInput(window);


		// rendering commands here
		//....
		//for example 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //state-setting function
		glClear(GL_COLOR_BUFFER_BIT); //state-using function

		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture);


		//use shader program
		ourShader.use();

		//drawing triangle
		glBindVertexArray(VAO); //each time you're about the draw you need to tell opengl which vao to use. opengl uses global state and only one vao can be activated at a time.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);




		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);  //this is last because presents the already-rendered frame to the screen.
		glfwPollEvents();


	}

	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)  //3 callback function on the window that gets called each time the window is resized.
{
	glViewport(0, 0, width, height);
}
