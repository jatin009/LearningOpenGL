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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

	Texture texture_img2("res/images/davidface.jpg", GL_REPEAT);

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

	va[1].Bind();

	float rect_2[] = {
		//positions			//texture coords		
		-0.5f, -0.5f,		0.0f, 0.0f,
		-0.5f,  0.5f,		0.0f, 1.0f,
		 0.5f,  0.5f,		1.0f, 1.0f,
		 0.5f, -0.5f,		1.0f, 0.0f
	};

	VertexBuffer vbo2(rect_2, sizeof(rect_2));

	VertexLayout rect_2_layout;
	rect_2_layout.Push<float>(2, 0);
	rect_2_layout.Push<float>(2, (void*) (2 * sizeof(float)));
	rect_2_layout.SetStride(4 * sizeof(float));

	va[1].AddLayout(rect_2_layout);
	//Index buffers code

	unsigned int indices2[] = {
		0, 1, 2,
		2, 3, 0
	};

	IndexBuffer ibo2(indices2, sizeof(indices2));

	vbo2.UnBind();
	va[1].UnBind();

	//Shader Program Creation
	Shader shaderProgram("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");

	//Using/Binding the shader program
	shaderProgram.Use();

	shaderProgram.SetUniform<int>("u_Texture1", 0);
	shaderProgram.SetUniform<int>("u_Texture2", 1);
	shaderProgram.SetUniform<float>("u_xShift", -0.15f);

	//Using second shader program
	Shader shaderProgram_2("res/shaders/Vertex_2.shader", "res/shaders/Fragment_2.shader");
	shaderProgram_2.Use();
	
	float r = 0.0f;
	float increment = 0.05f;
	float smileyFac = 0.2f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Use();

		glm::mat4 trans1(1.0f);
		trans1 = glm::scale(trans1, glm::vec3( sin(glfwGetTime() ), sin(glfwGetTime()), sin (glfwGetTime()) ));
		trans1 = glm::translate(trans1, glm::vec3(-0.2f, 0.8f, 0.0f));

		shaderProgram.SetUniformF("u_Color", r, 0.3f, 0.8f, 1.0f);
		shaderProgram.SetUniform<float>("u_MixVal", smileyFac);
		shaderProgram.SetUniform<glm::mat4>("u_Trans", trans1);

		texture1.Bind(0);
		texture2.Bind(1);
		va[0].Bind();
		
//		vbo1.ShiftXY(0.005f, 0.0f );
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		shaderProgram_2.Use();
		//OpenGL Maths Starts
		glm::mat4 trans(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//OpenGL Maths Ends

		shaderProgram_2.SetUniform<glm::mat4>("u_Transform", trans);

		texture_img2.Bind(0);
		va[1].Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
