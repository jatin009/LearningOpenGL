#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexLayout.h"
#include "Texture.h"
#include "stb_image.h"

void inputKey(GLFWwindow* win, float &smileyFactor)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);

	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (smileyFactor < 1.0f)
			smileyFactor += 0.01f;
	}
	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if ( smileyFactor > 0.0f)
		smileyFactor -= 0.01f;
	}
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

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Texture texture1("res/images/container.jpg", GL_CLAMP_TO_BORDER);
	Texture texture2("res/images/awesomeface.png", GL_REPEAT);

	//Vertex Array Object

	VertexArray va[2];
	va[0].Bind();

	float rect[] = {
		//rect coords		//texture coords	//colors
		-0.8f, -0.8f,		0.0f, 0.0f,			0.2f, 0.3f, 0.8f,
		-0.8f,  0.3f,		0.0f, 2.0f,			0.6f, 0.2f, 0.1f,
		 0.1f,  0.3f,		2.0f, 2.0f,			0.1f, 0.7f, 0.2f,
		 0.1f, -0.8f,		2.0f, 0.0f,			0.3f, 0.4f, 0.5f
	};

	//Below are the necessary steps to be followed to actually render a triangle as OpenGL is a state machine

	VertexBuffer vbo1(rect, sizeof(rect));

	//Layout creation

	VertexLayout layout;
	layout.Push<float>(2, 0);
	layout.Push<float>(2, (void*)(2*sizeof(float)));
	layout.Push<float>(3, (void*)(4*sizeof(float)));
	layout.SetStride(7 * sizeof(float));

	va[0].AddLayout(layout);

	//Index buffers code

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	IndexBuffer ibo(indices, sizeof(indices));
	vbo1.UnBind();

	//Binding VAO for different geometry
#if 0
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
#endif
	//Shader Program Creation
	Shader shaderProgram("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
	//Using/Binding the shader program
	shaderProgram.Use();
	shaderProgram.SetUniform1<int>("u_Texture1", 0);
	shaderProgram.SetUniform1<int>("u_Texture2", 1);
	shaderProgram.SetUniform1<float>("u_xShift", -0.15f);

	texture1.Bind(0);
	texture2.Bind(1);

	float r = 0.0f;
	float increment = 0.05f;
	float smileyFac = 0.2f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.SetUniformF("u_Color", r, 0.3f, 0.8f, 1.0f);
		shaderProgram.SetUniform1<float>("u_MixVal", smileyFac);

		va[0].Bind();
		vbo1.ShiftXY(0.005f, 0.0f );
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//va[1].Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 3);	//Render from the currently bound buffer

		if (r < 0.0f)
			increment = 0.05f;
		else if (r > 1.0f)
			increment = -0.05f;
		r += increment;

		inputKey(window, smileyFac);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
