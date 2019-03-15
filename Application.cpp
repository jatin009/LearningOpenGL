#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static std::stringstream ParseShader(const std::string& filepath)
{
	std::ifstream file;
	std::stringstream ss;

	file.open(filepath);

	if (file.is_open())
	{
		std::string line;
		while (getline(file, line))
		{
			ss << line << '\n';
		}
	}
	return ss;
}

static unsigned int CreateShader(unsigned int type, const std::string& source)
{
	unsigned int shaderid = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(shaderid, 1, &src, nullptr);

	glCompileShader(shaderid);
	int compile;
	glGetShaderiv(shaderid, GL_COMPILE_STATUS, &compile);

	if (compile != GL_TRUE)
	{
		char message[1024];
		int log_length;
		glGetShaderInfoLog(shaderid, 1024, &log_length, message);

		std::cout << "Shader Compilation Error: " << message << std::endl;
		return 0;
	}

	return shaderid;
}

int main(void)
{
	GLFWwindow* window;
	
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Vertex Array Object

	unsigned int vao[2];
	glGenVertexArrays(2, vao);
	glBindVertexArray(vao[0]);

	float rect[] = {
		-0.5f, -0.5f,
		-0.5f,  0.5f,
		 0.5f,  0.5f,
		 0.5f, -0.5f
	};
	
	//Below are the necessary steps to be followed to actually render a triangle as OpenGL is a state machine

	unsigned int buffer[2];
	glGenBuffers(2, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), rect, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//Index buffers code

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int ibo;

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(vao[1]);

	float tri[] = {
		0.0f, 1.0f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), tri, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	
	//Vertex Shader Creation
	std::stringstream source = ParseShader("res/shaders/Vertex.shader");
	unsigned int vShader = CreateShader(GL_VERTEX_SHADER, source.str());

	//Fragment Shader Creation
	source = ParseShader("res/shaders/Fragment.shader");
	unsigned int fShader = CreateShader(GL_FRAGMENT_SHADER, source.str());
	
	//Program creation to attach shader objects
	unsigned int program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glLinkProgram(program);

	int link;
	glGetProgramiv(program, GL_LINK_STATUS, &link);

	if (link != GL_TRUE)
	{
		char message[1024];
		int log_length;
		glGetProgramInfoLog(fShader, 1024, &log_length, message);

		std::cout << "Program Linking Error: " << message << std::endl;
	}

	glUseProgram(program);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Uncomment below 2 lines when you've only one vao and comment glBindVertexArray */
		//glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glBindVertexArray(vao[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Uncomment below 2 lines when you've only one vao and comment glBindVertexArray */
		//glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);	//Render from the currently bound buffer

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);	// 2nd param for elements in indices, 3rd param for data type in indices, 4th param should be nullptr if the ibo is already bound using glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo) or else should be indices

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
