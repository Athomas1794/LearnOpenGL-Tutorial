#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

//Vertex shader code in a string
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

//Vertex shader code in a string
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

//Callback function to adjust the viewport size when a window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Callback to proccess different inputs
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{

	//Initializing glfw window context settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //opengl version 3.3 minimum
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //only use core features without backwards compatability

	//Creating the window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Setting the rendering window
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //setting callback function for window resizing (GLFW fills  in parameters for us)

	//========================================================================
	//Start Shader code 
	//========================================================================

	//Triangle NDC (Normalized Device Coordinates)
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	//Vertex buffer object (input)
	//Store vertices in GPU memeory for faster access
	unsigned int VBO;
	glGenBuffers(1, &VBO); //generate buffer id
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind to array buffer type
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy vertex data to the buffer memory

	//Creating and compiling our vertex shader (vertex inputs)
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //Creating a vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //Attaching the shader code to its object
	glCompileShader(vertexShader); //Compiling shader

	//Checking shader compile
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		success = true;
	}

	//Fragment Shader (pixel color output)
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Checking fragment shader compile
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		success = true;
	}

	//Creating the Shader program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Attach and link the shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Checking shader link
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		success = true;
	}

	//Setting the program shader
	glUseProgram(shaderProgram);

	//Deleting shader objects we no longer need
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Linking Vertex Attributes
	//We have to specify how OpenGL should interpret the vertex data before rendering
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Setup VAO (stores vertex attribute calls)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 4. Draw the object in render loop

	//========================================================================
	//End Shader code 
	//========================================================================

	//Render loop to keep redrawing images
	while (!glfwWindowShouldClose(window)) //checks if GLFW has been instructed to close
	{
		//check and process input from user
		processInput(window);

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ..:: Drawing code (in render loop) :: ..
		// 4. draw the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap the 2d color buffer containing pixel values
		glfwSwapBuffers(window);	
		//check if any events were triggered (keyboard input, mouse movement, etc) updates the window state, and call the corresponding callback functions
		glfwPollEvents();
	}

	//terminate GLFW resources
	glfwTerminate();
	return 0;
}
