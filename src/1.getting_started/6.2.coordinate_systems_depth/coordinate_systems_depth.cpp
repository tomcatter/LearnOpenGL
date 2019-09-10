#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_s.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// store how much we're seeing or either texture
float mixValue = 0.2f;


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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "textures_combination", NULL, NULL);
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

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shader	program
	// --------------------------------
	Shader ourShader("6.2.coordinate_systems.vs", "6.2.coordinate_systems.fs");

	// set up vertex data (and buffers) and configure vertex attributes
	// ----------------------------------------------------------------
	float vertices[] = {
		// position           // texture coords
		// Vertex data for face 0
		-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  // v0
		0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // v1
		-0.5,  0.5f, -0.5f,    0.0f, 1.0f,  // v2
		0.5f,  0.5f, -0.5f,    1.0f, 1.0f, // v3

		// Vertex data for face 1
		0.5f, -0.5f,  0.5f,   0.0f, 1.0f, // v4
		0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // v5
		0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  // v6
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // v7

		// Vertex data for face 2
		0.5f, -0.5f, -0.5f,  0.66f, 0.5f, // v8
		-0.5f, -0.5f, -0.5f, 1.0f, 0.5f,  // v9
		0.5f,  0.5f, -0.5f,  0.66f, 1.0f, // v10
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // v11

		// Vertex data for face 3
		-0.5f, -0.5f, -0.5f, 0.66f, 0.0f, // v12
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // v13
		-0.5f,  0.5f, -0.5f, 0.66f, 0.5f, // v14
		-0.5f,  0.5f,  0.5f, 1.0f, 0.5f,  // v15

		// Vertex data for face 4
		-0.5f, -0.5f, -0.5f,  0.33f, 0.0f, // v16
		0.5f, -0.5f, -0.5f,   0.66f, 0.0f, // v17
		-0.5f, -0.5f,  0.5f,  0.33f, 0.5f, // v18
		0.5f, -0.5f,  0.5f,   0.66f, 0.5f, // v19

		// Vertex data for face 5
		-0.5f,  0.5f,  0.5f,  0.33f, 0.5f, // v20
		0.5f,  0.5f,  0.5f,   0.66f, 0.5f, // v21
		-0.5f,  0.5f, -0.5f,  0.33f, 1.0f, // v22
		0.5f,  0.5f, -0.5f,   0.66f, 1.0f  // v23
	};

	unsigned int indices[] = {
		0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
		4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
		8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
		12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
		16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
		20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	// bind the Vertex Array Object first, then bind and set vertex buffers, and
	// the configure vertex attributes

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways 
	// so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	// note that this is a allowed, the call to glVertexAttribPoint registered VBO
	// as the vertex attribute's bound vertex buffer object so afterwards we can safely
	// unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// uncomment this call to draw in wireframe polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// load and create a texture
	// -------------------------
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// tell stb_image.h to filp loaded texture's on the y-axis
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



	// tell opengl for each sampler to which texture unit it belong to (only has to be done once)
	// ------------------------------------------------------------------------------------------
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
					 // either set it manually like so
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// or set it via the texture class
	ourShader.setInt("texture2", 1);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// create transformations
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// get matrix's uniform location and set matrix
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");

		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, false, &view[0][0]);
		glUniformMatrix4fv(projectionLoc, 1, false, &projection[0][0]);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_INT, 0);

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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f;
		if (mixValue >= 1.0f)
		{
			mixValue = 1.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f;
		if (mixValue <= 0.0f)
		{
			mixValue = 0.0f;
		}
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