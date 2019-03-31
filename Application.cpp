#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexLayout.h"

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

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Vertex Array Object

	VertexArray va[2];
	va[0].Bind();

	float rect[] = {
		-0.4f, -0.4f,
		-0.4f,  0.4f,
		 0.2f,  0.4f,
		 0.2f, -0.4f
	};

	//Below are the necessary steps to be followed to actually render a triangle as OpenGL is a state machine

	VertexBuffer vbo1(rect, sizeof(rect));

	//Layout creation

	VertexLayout layout;
	layout.Push<float>(2, 0);
	va[0].AddLayout(layout);

	//Index buffers code

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	IndexBuffer ibo(indices, sizeof(indices));
	vbo1.UnBind();

	//Binding VAO for different geometry

	va[1].Bind();

	float tri[] = {
		//positions			
		0.35f, -0.5f, 0.0f,
		0.9f, -0.5f, 0.0f,
		0.45f, 0.5f, 0.0f
	};

	VertexBuffer vbo2(tri, sizeof(tri));

	VertexLayout trilayout;
	trilayout.Push<float>(3, 0);
	va[1].AddLayout(trilayout);

	vbo2.UnBind();
	va[1].UnBind();

	//Shader Program Creation
	Shader shaderProgram("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
	//Using/Binding the shader program
	shaderProgram.Use();

	shaderProgram.SetUniformF("u_xShift", -0.5f);

	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.SetUniformF("u_Color", r, 0.3f, 0.8f, 1.0f);

		va[0].Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		va[1].Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);	//Render from the currently bound buffer

		if (r < 0.0f)
			increment = 0.05f;
		else if (r > 1.0f)
			increment = -0.05f;
		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
