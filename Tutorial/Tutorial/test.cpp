#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "shader.h"

#include <iostream>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




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






	unsigned int texture1;
	glGenTextures(1, &texture1); //ID


	glBindTexture(GL_TEXTURE_2D, texture1); //bind


	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);





	//load texture
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true); //without this texture flipped. tell stb_image.h to flip loaded texture's on the y-axis.

	 

	unsigned char* data = stbi_load("C:/assets/wall.jpg", &width, &height, &nrChannels, 0);
 
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


	//free data
	stbi_image_free(data);

	   




	unsigned int texture2;
	glGenTextures(1, &texture2); //ID

 

	glBindTexture(GL_TEXTURE_2D, texture2); //bind


	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




	 


	data = stbi_load("C:/assets/awesomeface.png", &width, &height,	&nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	 
	//free memory after
	stbi_image_free(data);
	  



















	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
// -------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	//2 ways to set the texture unit;
	// 
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);


	float mixVal = 0;




	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate trans around z-axis 90 degrees
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f)); //scale to make it half



	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform"); //get the location of the uniform variable in the shader program
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)); //set the uniform variable in the shader program with the value(trans)


















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





		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{

			if (mixVal <= 0.99f )
			{
				mixVal = mixVal + 0.01f;
				std::cout << "Holding UP key\n";
				std::cout << mixVal;
			}

		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{


			if (mixVal >= 0.01f)
			{
				mixVal = mixVal - 0.01f;

				std::cout << "Holding DOWN key\n";
				std::cout << mixVal;
			}

		}







		// rendering commands here
		//....
		//for example 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //state-setting function
		glClear(GL_COLOR_BUFFER_BIT); //state-using function

		//activate the texture number you want and bind 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);





		// set the texture mix value in the shader  
		ourShader.setFloat("mixVal", mixVal);





		//render container
		//------------------------------------------------------
		//use shader program
		ourShader.use();
		//drawing triangle
		glBindVertexArray(VAO); //each time you're about the draw you need to tell opengl which vao to use. opengl uses global state and only one vao can be activated at a time.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//------------------------------------------------------



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
