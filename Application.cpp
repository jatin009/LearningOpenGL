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

	float positions[] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int buffer;

	//Below are the necessary steps to be followed to actually render a triangle as OpenGL is a state machine

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
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

		glDrawArrays(GL_TRIANGLES, 0, 3);	//Render from the currently bound buffer

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
