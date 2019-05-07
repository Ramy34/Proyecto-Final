#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>
#include "SOIL2/SOIL2.h"
#include "camera.h"
#include "Model.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings-----------------------------------------------------------------------------------------------------------------------------------------
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f);
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);

void myData(void);
void display(Shader, Model, Model);
void display2(Shader);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

float	base = 0.0f,
		articulacion1 = 0.0f,
		articulacion2 = 0.0f,
		articulacionmon = 0.0f;

//Texture
unsigned int	t_unam,
				t_mesa,
				t_patas,
				t_pizarron,
				t_ladrillo,
				t_pared,
				t_pared_ventanas,
				t_concreto,
				t_base,
				t_monitor,
				t_cpu,
				t_teclado,
				t_piso_techo,
				t_pared_ven,
				t_texture;

//Para ciclos
int i, j, k;

//For model
bool animacion = false;
float movKit_z = 0.0f;
//--------------------------------------------------------------------------------------------------------------------------------------------------

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------
unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "No se pudo cargar las texturas :,(" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{
	t_unam = generateTextures("Texturas/escudo_unam.jpg", 0);
	t_patas = generateTextures("Texturas/patas.png", 1);
	t_mesa = generateTextures("Texturas/mesa.png", 1);
	t_pizarron = generateTextures("Texturas/Pizarron.png", 1);
	t_ladrillo = generateTextures("Texturas/Ladrillo.png", 1);
	t_pared = generateTextures("Texturas/pared.png", 1);
	t_pared_ventanas = generateTextures("Texturas/pared_puertas.png", 1);
	t_concreto = generateTextures("Texturas/Concreto.png", 1);
	t_base = generateTextures("Texturas/Base.png", 1);
	t_monitor = generateTextures("Texturas/Monitor.png", 1);
	t_cpu = generateTextures("Texturas/CPU.png", 1);
	t_teclado = generateTextures("Texturas/teclado.jpg", 0);
	t_piso_techo = generateTextures("Texturas/Piso_Techo.png", 1);
	t_pared_ven = generateTextures("Texturas/Pared_Ventana.png", 1);
	t_texture = generateTextures("Texturas/awesomeface.png", 1);
	

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_unam);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_patas);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_mesa);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, t_pizarron);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, t_ladrillo);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, t_pared);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, t_pared_ventanas);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, t_concreto);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, t_base);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, t_monitor);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, t_cpu);

	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, t_teclado);
	
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, t_piso_techo);

	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, t_pared_ven);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, t_texture);
}
//Termina la parte de las texturas -----------------------------------------------------------------------------------------------------------------

void myData()
{
	float vertices[] = {
		//Cubo para texturas únicas
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  //Cara Trasera 0
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  //Cara Frontal 4
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f, //Cara Izquierda 8
		-0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  //Cara Derecha 12
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  //Cara Inferior 16
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  //Cara Superior 20
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
		
		//Cubo para el pizarron
		-0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  //Cara Trasera 24
		 0.5f, -0.5f, -0.5f,  1.0f,  0.5f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.5f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  //Cara Frontal 28
		 0.5f, -0.5f,  0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.5f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.5f,

		-0.5f,  0.5f,  0.5f,  0.0f,  0.5f, //Cara Izquierda 32
		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  0.0f,  0.5f,  //Cara Derecha 36
		 0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f,  0.5f,  //Cara Inferior 40
		 0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  0.5f,  //Cara Superior 44
		 0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
		//Paredes de puertas
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f, //48
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.6666f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.6666f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, //52
		-0.5f,  0.5f,  0.5f,  1.0f,  1.0f, 
		-0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  0.3333f, //56
		-0.5f,  0.5f,  0.5f,  1.0f,  0.3333f,
		-0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

		//Monitor
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  //Cara Trasera 60
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.7f,
		-0.5f,  0.5f, -0.5f,  0.0,   0.7f,

		-0.5f,  0.5f,  0.5f,  0.0f,  0.5f,  //Cara Frontal 64
		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f, //Cara Izquierda 68
		-0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f,  0.7f,
		-0.5f, -0.5f,  0.5f,  0.0,   0.7f,

		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  //Cara Derecha 72
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.7f,
		 0.5f, -0.5f,  0.5f,  0.0,   0.7f,

		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  //Cara Inferior 76
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.7f,
		-0.5f, -0.5f,  0.5f,  0.0,   0.7f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  //Cara Superior 80
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.7f,
		-0.5f,  0.5f,  0.5f,  0.0,  0.7f,
//Cubo del CPU
		-0.5f, -0.5f, -0.5f, 0.5f, 0.5f,  //Cara Trasera 84
		0.5f, -0.5f, -0.5f,  1.0f, 0.5f,
		0.5f, 0.5f, -0.5f,   1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,  0.5f, 0.0f,

		-0.5f, 0.5f, 0.5f,   0.5f, 1.0f,  //Cara Frontal 88
		 0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,  1.0f, 0.5f,
		-0.5f, -0.5f, 0.5f,  0.5f, 0.5f,

		-0.5f, 0.5f, -0.5f,  0.0f, 1.0f, //Cara Izquierda 92
		-0.5f, 0.5f,  0.5f,  0.5f, 1.0f,
		-0.5f, -0.5f, 0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.5f,

		0.5f, 0.5f, 0.5f,    0.0f, 1.0f,  //Cara Derecha 96
		0.5f, 0.5f, -0.5f,   0.5f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,   0.0f, 0.5f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //Cara Inferior 100
		0.5f, -0.5f, -0.5f,  0.5f, 1.0f,
		0.5f, -0.5f, 0.5f,   0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.5f,

		-0.5f, 0.5f, -0.5f,  0.0f, 1.0f,  //Cara Superior 104
		0.5f, 0.5f, -0.5f,   0.5f, 1.0f,
		0.5f, 0.5f, 0.5f,    0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,   0.0f, 0.5f,
//Piso_Techo
		-0.5f, -0.5f, -0.5f, 0.0f, 0.5f,  //Cara Trasera 108
		 0.5f, -0.5f, -0.5f, 1.0f, 0.5f,
		 0.5f, 0.5f, -0.5f,  1.0f, 0.0f,
		 -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,

		 -0.5f, 0.5f, 0.5f,  0.0f, 0.5f,  //Cara Frontal 112
		 0.5f, 0.5f, 0.5f,   1.0f, 0.5f,
		 0.5f, -0.5f, 0.5f,  1.0f, 0.0f,
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		 -0.5f, 0.5f, -0.5f, 0.0f, 0.5f, //Cara Izquierda 116
		 -0.5f, 0.5f, 0.5f,  1.0f, 0.5f,
		 -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,0.0f, 0.0f,

		 0.5f, 0.5f, 0.5f,   0.0f, 0.5f,  //Cara Derecha 120
		 0.5f, 0.5f, -0.5f,  1.0f, 0.5f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

		 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //Cara Inferior 124
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,   1.0f, 0.5f,
		 -0.5f, -0.5f, 0.5f,  0.0f, 0.5f,

		 -0.5f, 0.5f, -0.5f,  0.0f, 0.5f,  //Cara Superior 128
		 0.5f, 0.5f, -0.5f,   1.0f, 0.5f,
		 0.5f, 0.5f, 0.5f,    1.0f, 0.0f,
		 -0.5f, 0.5f, 0.5f,   0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f,  0.0f, 1.0f, //Cara Trasera 132
		 0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  
		 
		 
		
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3, // second triangle

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
//Termina la definición de los vertices ------------------------------------------------------------------------------------------------------------

void animate(void)
{
	if (animacion)
	{
		movKit_z += 0.03f;
		if (movKit_z >= 15.0f)
			animacion = false;
	}
}
//Termina la parte de a animación ------------------------------------------------------------------------------------------------------------------

void display(Shader shader, Model modelo, Model llantas, Model piso)
{
	shader.use();

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.8f, -1.0f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	shader.setMat4("model", model);
	piso.Draw(shader);

	model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(15.0f, -1.75f, movKit_z));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	shader.setMat4("model", model);
	modelo.Draw(shader);

	model = glm::translate(tmp, glm::vec3(0.85f, 0.25f, 1.29f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Izq delantera

	model = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, 1.29f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Der delantera

	model = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, -1.45f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Der trasera

	model = glm::translate(tmp, glm::vec3(0.85f, 0.25f, -1.45f));
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model);
	llantas.Draw(shader);	//Izq trase
}
//Termina la parte del dibujo de modelos -----------------------------------------------------------------------------------------------------------

void display2(Shader projectionShader) {
	projectionShader.use();

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 tmp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	//-----------------------------------------------------------------Computadora--------------------------------------------------------------------------
	model = glm::mat4(1.0f); //Monitor Base
	model = glm::translate(model, glm::vec3(12.5f, 7.05f, -22.6f)); //Ubicación del salón

	model = glm::rotate(model, glm::radians(base), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.6f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setInt("texture1", t_base);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::translate(tmp, glm::vec3(0.0f, -0.05f, 0.05f)); //Tronco
	model = glm::rotate(model, glm::radians(articulacion1), glm::vec3(-1.0f, 0.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(0.0f, 0.35f, 0.0f));//moverme al centro de la primitiva
	model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setInt("texture1", t_base);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::translate(tmp, glm::vec3(0.0f, 0.15f, 0.1f));//Soporte
	model = glm::rotate(model, glm::radians(articulacion2), glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));//moverme al centro
	model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.4f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setInt("texture1", t_base);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.25f));//Pantalla
	model = glm::rotate(model, glm::radians(articulacionmon), glm::vec3(-1.0f, 0.0f, 0.0f));
	tmp = model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.1f));//moverme al centro
	model = glm::scale(model, glm::vec3(0.8f, 0.5f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setInt("texture1", t_monitor);
	glDrawArrays(GL_QUADS, 60, 24);

	//CPU
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(13.0f, 7.5f, -22.5f)); //Ubicación del salón
	model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setInt("texture1", t_cpu);
	glDrawArrays(GL_QUADS, 84, 24);

	//Teclado
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.5f, 7.0f, -22.1f)); //Ubicación del salón
	model = glm::scale(model, glm::vec3(0.99f, 0.005f, 0.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_teclado);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//Dibujo del salón de cómputo
	
	//---------------------------------------------------------------Edificio Derecho-----------------------------------------------------------------------
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(11.0f, 0.0f, 11.0f));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			model = glm::scale(model, glm::vec3(10.0f, 4.0f, 0.1f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_concreto);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, -16.0f, -44.0f));
	}

	//Ventanas
	model = glm::mat4(1.0f);//Primer Salón
	tmp = model = glm::translate(model, glm::vec3(15.94f, 0.0f, 6.5f));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			model = glm::scale(model, glm::vec3(0.11f, 4.0f, 9.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ventanas);
			glDrawArrays(GL_QUADS, 48, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, -16.0f, 0.0f));
	}

	model = glm::mat4(1.0f); //Segundo Salón
	tmp = model = glm::translate(model, glm::vec3(15.94f, 0.0f, -2.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(0.11f, 4.0f, 8.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ventanas);
		glDrawArrays(GL_QUADS, 48, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	}
	tmp = model = glm::translate(tmp, glm::vec3(-9.89f, -16.0f, 0.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(0.11f, 3.999f, 8.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ventanas);
		glDrawArrays(GL_QUADS, 56, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f); //Salón central
	tmp = model = glm::translate(model, glm::vec3(15.94f, 0.0f, -10.5f));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			model = glm::scale(model, glm::vec3(0.11f, 4.0f, 9.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ventanas);
			glDrawArrays(GL_QUADS, 48, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, -16.0f, 0.0f));
	}

	model = glm::mat4(1.0f); //Salón del laboratorio
	tmp = model = glm::translate(model, glm::vec3(15.94f, 0.0f, -20.0f));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			model = glm::scale(model, glm::vec3(0.11f, 4.0f, 10.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ventanas);
			glDrawArrays(GL_QUADS, 48, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, -16.0f, 0.0f));
	}

	model = glm::mat4(1.0f);//último salón
	tmp = model = glm::translate(model, glm::vec3(15.94f, 0.0f, -29.0f));
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			model = glm::scale(model, glm::vec3(0.11f, 4.0f, 8.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ventanas);
			glDrawArrays(GL_QUADS, 48, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, -16.0f, 0.0f));
	}

	//Pared que da a los pasillos
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(8.125f, 0.0f, -21.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(0.1f, 4.0f, 8.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared);
		glDrawArrays(GL_QUADS, 52, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(8.125f, 0.0f, -11.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(0.1f, 4.0f, 8.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared);
		glDrawArrays(GL_QUADS, 52, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f); //último salón
	tmp = model = glm::translate(model, glm::vec3(8.125f, 0.0f, -3.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(0.1f, 4.0f, 6.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared);
		glDrawArrays(GL_QUADS, 52, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	}

	//Paredes que dividen los salones
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(12.01f, 0.0f, -25.0f));

	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(7.95f, 3.99f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(7.95f, 3.99f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 9.0f));
		model = glm::scale(model, glm::vec3(7.95f, 3.99f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 8.0f));
		model = glm::scale(model, glm::vec3(7.95f, 3.99f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, -27.0f));
	}
	//Piso - Techo
	model = glm::mat4(1.0f);//último salón
	tmp = model = glm::translate(model, glm::vec3(7.04f, -2.0f, -29.0f));
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(2.0f, 0.05f, 8.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_piso_techo);
			glDrawArrays(GL_QUADS, 108, 24);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(7.04f, -2.0f, -23.0f));
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(2.0f, 0.05f, 4.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_piso_techo);
			glDrawArrays(GL_QUADS, 108, 24);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(7.04f, -2.0f, -17.0f));
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(2.0f, 0.05f, 8.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_piso_techo);
			glDrawArrays(GL_QUADS, 108, 24);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, 4.0f, 0.0f));
	}
	
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(7.04f, -2.0f, -10.0f));
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(2.0f, 0.05f, 6.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_piso_techo);
			glDrawArrays(GL_QUADS, 108, 24);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(7.04f, -2.0f, -4.0f));
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(2.0f, 0.05f, 6.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_piso_techo);
			glDrawArrays(GL_QUADS, 108, 24);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(7.04f, -2.0f, 2.0f));
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(2.0f, 0.05f, 6.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_piso_techo);
			glDrawArrays(GL_QUADS, 108, 24);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(7.04f, -2.0f, 8.0f));
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(2.0f, 0.05f, 6.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_piso_techo);
			glDrawArrays(GL_QUADS, 108, 24);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-10.0f, 4.0f, 0.0f));
	}

	//------------------------------------------------------------------Edificio Izquierdo------------------------------------------------------------------
	//Paredes que dividen los salones
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-12.0f, 0.0f, -3.0f));

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			model = glm::scale(model, glm::vec3(7.95f, 3.99f, 0.01f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 6.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, -24.0f));
	}
	//Pared que da a los pasillos
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-7.975f, 0.0f, -7.0f));
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			model = glm::scale(model, glm::vec3(0.1f, 4.0f, 4.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared);
			glDrawArrays(GL_QUADS, 52, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 6.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, -24.0f));
	}

	//Techo_Piso
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-7.0f, -2.0f, -6.0f));
	for (k = 0; k < 5; k++) {
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++) {
				model = glm::scale(model, glm::vec3(2.0f, 0.05f, 6.0f));
				projectionShader.setMat4("model", model);
				projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
				projectionShader.setInt("texture1", t_piso_techo);
				glDrawArrays(GL_QUADS, 108, 24);
				tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 6.0f));
			}
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, -30.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-2.0f, -20.0f, -0.0f));
	}
	//Ventanas
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-16.0f, -0.0f, -6.0f));
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 5; j++) {
			model = glm::scale(model, glm::vec3(0.01f, 4.0f, 6.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ventanas);
			glDrawArrays(GL_QUADS, 48, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 6.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, -30.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-6.0f, -0.0f, 18.0f));
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			model = glm::scale(model, glm::vec3(0.01f, 4.0f, 6.0f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ventanas);
			glDrawArrays(GL_QUADS, 48, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -6.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 18.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-6.0f, -0.0f, -6.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(0.01f, 4.0f, 6.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ventanas);
		glDrawArrays(GL_QUADS, 48, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	}
	
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-6.0f, -0.0f, 0.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(0.01f, 4.0f, 6.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ventanas);
		glDrawArrays(GL_QUADS, 56, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 0.0f));
	}

	//Cemento
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-11.0f, 0.0f, 21.0f));
	for (i = 0; i < 4; i++) {
		model = glm::scale(model, glm::vec3(10.0f, 4.0f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_concreto);
		glDrawArrays(GL_QUADS, 0, 4);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -30.0f));
		model = glm::scale(model, glm::vec3(10.0f, 4.0f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_concreto);
		glDrawArrays(GL_QUADS, 0, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 4.0f, 30.0f));
	}
	//------------------------------------------------------------Edificio Central--------------------------------------------------------------------------
	//Piso_Techo
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-5.0f, -2.0f, 3.0f));
	for (k = 0; k < 5; k++) {
		for (j = 0; j < 6; j++) {
			for (i = 0; i < 2; i++) {
				model = glm::scale(model, glm::vec3(2.0f, 0.05f, 6.0f));
				projectionShader.setMat4("model", model);
				projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
				projectionShader.setInt("texture1", t_piso_techo);
				glDrawArrays(GL_QUADS, 108, 24);
				tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -6.0f));
			}
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 12.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-12.0f, 4.0f, 0.0f));
	}

	//Paredes
	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, -6.0f));
	for (i = 0; i < 2; i++) {
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ven);
		glDrawArrays(GL_QUADS, 132, 4);
		tmp = model = glm::translate(tmp, glm::vec3(-2.0f, 0.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-5.0f, 4.0f, -6.0f));
	for (j = 0; j < 3; j++) {
		for (i = 0; i < 6; i++) {
			model = glm::scale(model, glm::vec3(2.0f, 4.0f, 0.01f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ven);
			glDrawArrays(GL_QUADS, 132, 4);
			tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 12.0f));
			model = glm::scale(model, glm::vec3(2.0f, 4.0f, 0.01f));
			projectionShader.setMat4("model", model);
			projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
			projectionShader.setInt("texture1", t_pared_ven);
			glDrawArrays(GL_QUADS, 132, 4);
			tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, -12.0f));
		}
		tmp = model = glm::translate(tmp, glm::vec3(-12.0f, 4.0f, 0.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -6.0f));
	for (i = 0; i < 2; i++) {
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ven);
		glDrawArrays(GL_QUADS, 132, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 12.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ven);
		glDrawArrays(GL_QUADS, 132, 4);
		tmp = model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, -12.0f));
	}

	model = glm::mat4(1.0f);
	tmp = model = glm::translate(model, glm::vec3(5.0f, 0.0f, -6.0f));
	for (i = 0; i < 2; i++) {
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ven);
		glDrawArrays(GL_QUADS, 132, 4);
		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 12.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 0.01f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_pared_ven);
		glDrawArrays(GL_QUADS, 132, 4);
		tmp = model = glm::translate(tmp, glm::vec3(-2.0f, 0.0f, -12.0f));
	}
	

	//----------------------------------------------------------------------Mesa----------------------------------------------------------------------------
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(14.0f, 6.99f, -22.5f)); //Ubicación del salón

	for (j = 0; j < 3; j++) {
		tmp = model;
		model = glm::scale(model, glm::vec3(3.98f, 0.01f, 1.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_mesa);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
	}
	model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -6.0f));

	for (j = 0; j < 3; j++) {
		tmp = model;
		model = glm::scale(model, glm::vec3(3.0f, 0.01f, 1.0f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_mesa);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
	}

	//--------------------------------------------------------------------Patas mesa------------------------------------------------------------------------
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.25f, 6.5f, -22.125f));
	for (j = 0; j < 3; j++) {
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.8f));
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(3.5f, 0.0f, 0.0f));
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		projectionShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.8f));
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(-3.5f, 0.0f, 2.0f));
	}

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(8.75f, 6.5f, -22.125f));
	for (j = 0; j < 3; j++) {
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.8f));
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		projectionShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.8f));
		tmp = model;
		model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
		projectionShader.setMat4("model", model);
		projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		projectionShader.setInt("texture1", t_patas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = tmp;

		model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 2.0f));
	}

	//--------------------------------------------------------------------------Escritorio------------------------------------------------------------------
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(14.125f, 6.5f, -23.6f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.8f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = tmp;

	model = glm::translate(model, glm::vec3(1.7f, 0.0f, 0.0f));
	tmp = model;
	model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	projectionShader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.8f));
	model = glm::scale(model, glm::vec3(0.125f, 0.98f, 0.125f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_patas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(15.0f, 6.99f, -24.0f));
	model = glm::scale(model, glm::vec3(1.98f, 0.01f, 1.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_mesa);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	//---------------------------------------------------------------Pizarrón-------------------------------------------------------------------------------
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(12.0f, 8.0f, -24.999f)); //Ubicación del salón
	//tmp = model = glm::translate(model, glm::vec3(-5.0f, 4.0f, -20.0f));
	model = glm::scale(model, glm::vec3(5.0f, 2.0f, 0.01f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setInt("texture1", t_pizarron);
	glDrawArrays(GL_QUADS, 24, 24);
	
	glBindVertexArray(0);

}
//Termina la parte del dibujado de primitivas ------------------------------------------------------------------------------------------------------

void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		animacion = true;
}
//Termina el uso de las teclas ---------------------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");
	Shader modelShader("Shaders/modelLoading.vs", "Shaders/modelLoading.fs");
	// Load models
	Model ourModel = ((char *)"Models/Lambo/carroseria.obj");
	Model llantasModel = ((char *)"Models/Lambo/Wheel.obj");
	Model pisoModel = ((char *)"Models/piso/piso.obj");


	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		my_input(window);
		animate();

		// render
		// Backgound color
		glClearColor(0.0f, 0.95f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel, pisoModel);
		display2(projectionShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}