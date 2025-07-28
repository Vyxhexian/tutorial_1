#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// resolution settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


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









	//first build and compile shader program
	//vertex shader


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //ID

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //attach
	glCompileShader(vertexShader); //compile


	//checking for compile-time error ---------------
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}





	//fragment shader

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //ID

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //attach
	glCompileShader(fragmentShader); //compile



	// check for shader compile errors ---------------
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	//Shader program
	//link shaders

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); //ID

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors  ---------------
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//delete shaders.Shaders are no longer needed as separate objects, they're already part of the program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);






	//Input vertex data
	float vertices[] = { 0.0f,0.0f, 0.0f,
	1.0f,0.0f, 0.0f,
	0.5f, 0.8f, 0.0f,
	
	0.0f, 0.0f, 0.0f,
	-1.0f,0.0f, 0.0f,
	-0.5f, 0.8f, 0.0f
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







	//Linking Vertex Attributes
	//what part of input data goes to which vertex attribute in vertex shader

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);






	// those 2 unbinding of VBO and VAO allowed, (from tutorial code)
	// 
	// 
	// 
	// ---------------------------------------------------------
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);//you can safely unbind the VBO because the VAO has stored everything it needs.


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0); //Some developers like to unbind to prevent accidental modification, others just leave it bound.
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




		//drawing triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); //each time you're about the draw you need to tell opengl which vao to use. opengl uses global state and only one vao can be activated at a time.
		glDrawArrays(GL_TRIANGLES, 0, 6);




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
