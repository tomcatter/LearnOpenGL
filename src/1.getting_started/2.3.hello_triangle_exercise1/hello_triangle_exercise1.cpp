#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 1) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


int main()
{

	// GLFW: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // GLFW window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile shader	program
	// --------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	// fragment shader
	// ---------------
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}


	// link shader
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	// set up vertex data (and buffers) and configure vertex attributes
	// ----------------------------------------------------------------
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
							 // second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int indices[] = {
		0, 1, 3, 
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffers, and
	// the configure vertex attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int aPosAttrib = glGetAttribLocation(shaderProgram, "aPos");
	glEnableVertexAttribArray(aPosAttrib);
	glVertexAttribPointer(aPosAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways 
	// so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// note that this is a allowed, the call to glVertexAttribPoint registered VBO
	// as the vertex attribute's bound vertex buffer object so afterwards we can safely
	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer
	// object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// uncomment this call to draw in wireframe polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);


		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw a first triangle
		glUseProgram(shaderProgram);
		
		//glEnableVertexAttribArray(aPosAttrib);

		//glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//glVertexAttribPointer(aPosAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glBindVertexArray(VAO); 

		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time 
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse move etc.)
		// ------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// glfw: terminate, clearing all previously allocated GLFW resource.
	// -----------------------------------------------------------------
	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}