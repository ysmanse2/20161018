#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <stdlib.h>		  
#include <math.h>





class GeometricObject
{
public:

	const int width = 640;
	const int height = 480;
	int i_center = 300, j_center = 400;
	int thickness = 50;

	int a_ = 123;

	float *pixels = new float[width * height * 3];

	void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
	{
		pixels[(i + width* j) * 3 + 0] = red;
		pixels[(i + width* j) * 3 + 1] = green;
		pixels[(i + width* j) * 3 + 2] = blue;
	}

	void(GeometricObject::*callback_)(void) = nullptr;

	void drawBackground()
	{
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				pixels[(i + width* j) * 3 + 0] = 1.0f;
				pixels[(i + width* j) * 3 + 1] = 1.0f;
				pixels[(i + width* j) * 3 + 2] = 1.0f;
			}
		}
	}

	void execute() 
	{
		(this->*callback_)();
	}

	void command(GeometricObject *go, void(GeometricObject::*input_fp)(void))
	{
		/*assert(go != nullptr);
		assert(input_fp != nullptr);*/
		
		(go->*input_fp)();
	}

	void drawCircle()
	{
		double radius = 0;

		for (int j = 1; j <= 60; j++)
		{
			radius = j;
			for (double i = 0.0; i < 360.0; i += 0.1)
			{
				double angle = i * 3.1415926535897932384626433832795 / 180;

				int x = (int)(300 + radius * cos(angle));
				int y = (int)(100 + radius * sin(angle));

				drawPixel(x, y, 1.0f, 0.0f, 0.0f);
			}
		}
	}

	void drawCircleForSurrounding()
	{
		double radius = 0;

		for (int j = 1; j <= 60; j++)
		{
			radius = j;
			for (double i = 0.0; i < 360.0; i += 0.1)
			{
				double angle = i * 3.1415926535897932384626433832795 / 180;

				int x = (int)((300/2) + radius * cos(angle));
				int y = (int)((100/2) + radius * sin(angle));

				drawPixel(x, y, 1.0f, 0.0f, 0.0f);
			}
		}
	}

	void drawBox()
	{
		int i_center = 500;
		int j_center = 400;

		for (int j = j_center - thickness; j < j_center + thickness; j++) {
			for (int i = i_center - thickness; i < i_center + thickness; i++) {
				drawPixel(i, j, 1.0f, 0.0f, 0.0f);
			}
		}
	}

	void drawBoxForSurrounding()
	{
		int i_center = 400;
		int j_center = 300;

		for (int j = j_center - thickness; j < j_center + thickness; j++) {
			for (int i = i_center - thickness; i < i_center + thickness; i++) {
				drawPixel(i, j, 1.0f, 0.0f, 0.0f);
			}
		}
	}


	static GeometricObject* getPoint(const std::string type_name)
	{

		auto new_ob = new GeometricObject;

		if (type_name == "Background") {
			new_ob->callback_ = &GeometricObject::drawBackground;
		}
		else if (type_name == "Box") {
			new_ob->callback_ = &GeometricObject::drawBox;
			new_ob->callback_ = &GeometricObject::drawBoxForSurrounding;
		}
		else if (type_name == "Circle") {
			new_ob->callback_ = &GeometricObject::drawCircle;
			new_ob->callback_ = &GeometricObject::drawCircleForSurrounding;
		}

		return new_ob;
	}
};




int main() {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		std::list<GeometricObject *> go_list;
		go_list.push_back(GeometricObject::getPoint("Background"));
		go_list.push_back(GeometricObject::getPoint("Box"));
		go_list.push_back(GeometricObject::getPoint("Circle"));

		for (auto itr : go_list)
		{
			itr->execute();
		}

		for (auto itr : go_list)
		{
			delete itr;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	delete[] pixels;
	// free(pixels);

	return 0;
}