#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Shader.h"

#include "Object.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipo da função de callback do mouse
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void cursor_enter_callback(GLFWwindow* window, int entered);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Protótipos das funções
int setupGeometry();
int loadTexture(string path);
GLuint createSprite();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;
const int nPoints = 100 + 1 + 1; //+centro +cópia do primeiro
const float pi = 3.14159;

// carregar imagem //
string imagem;

int x = 0;
int y = 0;

int s = 0;
int i = 0;
int c = 0;

int stk = 0;

bool filtro1 = false;
bool filtro2 = false;
bool filtro3 = false;
bool filtro4 = false;
bool filtro5 = false;
bool filtro6 = false;
bool filtro7 = false;

bool sticker1 = false;
bool sticker2 = false;
bool sticker3 = false;
bool sticker4 = false;
bool sticker5 = false;

bool movesticker[5] = { false,false,false,false,false };
bool keepsticker[5] = { false,false,false,false,false };

// posição do sticker //
double sXpos;
double sYpos;

enum cores
{
	R, G, B
};

// Função MAIN
int main()
{

#pragma region GLFW

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Filtros - Erick", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// MOUSE
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

#pragma endregion

#pragma region Load de Shaders e texturas



	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader* shaderimagem = new Shader("./shaders/fimagem.vs", "./shaders/fimagem.fs");
	Shader* shader = new Shader("./shaders/f1.vs", "./shaders/f1.fs");
	Shader* shader2 = new Shader("./shaders/f2.vs", "./shaders/f2.fs");
	Shader* shader3 = new Shader("./shaders/f3.vs", "./shaders/f3.fs");
	Shader* shader4 = new Shader("./shaders/f4.vs", "./shaders/f4.fs");
	Shader* shader5 = new Shader("./shaders/f5.vs", "./shaders/f5.fs");
	Shader* shader6 = new Shader("./shaders/f6.vs", "./shaders/f6.fs");
	Shader* shader7 = new Shader("./shaders/f7.vs", "./shaders/f7.fs");

	// icones dos filtros //
	Shader* shadern1 = new Shader("./shaders/sn1.vs", "./shaders/sn1.fs");
	Shader* shadern2 = new Shader("./shaders/sn2.vs", "./shaders/sn2.fs");
	Shader* shadern3 = new Shader("./shaders/sn3.vs", "./shaders/sn3.fs");
	Shader* shadern4 = new Shader("./shaders/sn4.vs", "./shaders/sn4.fs");
	Shader* shadern5 = new Shader("./shaders/sn5.vs", "./shaders/sn5.fs");
	Shader* shadern6 = new Shader("./shaders/sn6.vs", "./shaders/sn6.fs");
	Shader* shadern7 = new Shader("./shaders/sn7.vs", "./shaders/sn7.fs");

	// stickers //
	Shader* shaderstks[5];
	for (int i = 0; i < 5; i++)
	{
		string str1 = "./shaders/stk" + to_string(i+1) + ".vs";
		string str2 = "./shaders/stk" + to_string(i+1) + ".fs";
		shaderstks[i] = new Shader(str1.c_str(), str2.c_str());
	}

	Shader* shaderstk1 = new Shader("./shaders/stk1.vs", "./shaders/stk1.fs");
	Shader* shaderstk2 = new Shader("./shaders/stk2.vs", "./shaders/stk2.fs");
	Shader* shaderstk3 = new Shader("./shaders/stk3.vs", "./shaders/stk3.fs");
	Shader* shaderstk4 = new Shader("./shaders/stk4.vs", "./shaders/stk4.fs");
	Shader* shaderstk5 = new Shader("./shaders/stk5.vs", "./shaders/stk5.fs");

	// icones stickers //
	Shader* shaderistk1 = new Shader("./shaders/istk1.vs", "./shaders/istk1.fs");
	Shader* shaderistk2 = new Shader("./shaders/istk2.vs", "./shaders/istk2.fs");
	Shader* shaderistk3 = new Shader("./shaders/istk3.vs", "./shaders/istk3.fs");
	Shader* shaderistk4 = new Shader("./shaders/istk4.vs", "./shaders/istk4.fs");
	Shader* shaderistk5 = new Shader("./shaders/istk5.vs", "./shaders/istk5.fs");

	// Carregando texturas
	cout << "caminho da imagem: " << endl;
	cin >> imagem;
	if (imagem.length() > 3) {
		imagem = "./textures/" + imagem;
	}
	else
	{
		imagem = "./textures/8.jpg";
		cout << "imagem inválida, carregando imagem padrao.\n";
	}

	GLuint texID = loadTexture(imagem);

	// filtros //
	GLuint texID1 = loadTexture("./textures/1.png");
	GLuint texID2 = loadTexture("./textures/2.png");
	GLuint texID3 = loadTexture("./textures/3.png");
	GLuint texID4 = loadTexture("./textures/4.png");
	GLuint texID5 = loadTexture("./textures/5.png");
	GLuint texID6 = loadTexture("./textures/6.png");
	GLuint texID7 = loadTexture("./textures/7.png");

	/// sticker ///
	//GLuint* texIDstk = 

	GLuint texIDstk1 = loadTexture("./textures/stk1.png");
	//texIDstk[0] = texIDstk1;

	GLuint texIDstk2 = loadTexture("./textures/stk2.png");
	//texIDstk[1] = texIDstk2;

	GLuint texIDstk3 = loadTexture("./textures/stk3.png");
	//texIDstk[2] = texIDstk3;

	GLuint texIDstk4 = loadTexture("./textures/stk4.png");
	//texIDstk[3] = texIDstk4;

	GLuint texIDstk5 = loadTexture("./textures/stk5.png");
	//texIDstk[4] = texIDstk5;

#pragma endregion

#pragma region Setup Objetos

	///// OBJECTS /////
	Object image;
	image.initialize();
	image.setTexture(texID);
	image.setShader(shaderimagem);
	image.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	image.setPosition(glm::vec3(400.0, 340.0, 0.0));

	Object f1;
	f1.initialize();
	f1.setTexture(texID);
	f1.setShader(shader);
	f1.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	f1.setPosition(glm::vec3(400.0, 340.0, 0.0));

	Object f2;
	f2.initialize();
	f2.setTexture(texID);
	f2.setShader(shader2);
	f2.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	f2.setPosition(glm::vec3(400.0, 340.0, 0.0));

	Object f3;
	f3.initialize();
	f3.setTexture(texID);
	f3.setShader(shader3);
	f3.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	f3.setPosition(glm::vec3(400.0, 340.0, 0.0));

	Object f4;
	f4.initialize();
	f4.setTexture(texID);
	f4.setShader(shader4);
	f4.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	f4.setPosition(glm::vec3(400.0, 340.0, 0.0));

	Object f5;
	f5.initialize();
	f5.setTexture(texID);
	f5.setShader(shader5);
	f5.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	f5.setPosition(glm::vec3(400.0, 340.0, 0.0));

	Object f6;
	f6.initialize();
	f6.setTexture(texID);
	f6.setShader(shader6);
	f6.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	f6.setPosition(glm::vec3(400.0, 340.0, 0.0));

	Object f7;
	f7.initialize();
	f7.setTexture(texID);
	f7.setShader(shader7);
	f7.setDimensions(glm::vec3(300.0, 300.0, 1.0));
	f7.setPosition(glm::vec3(400.0, 340.0, 0.0));

	///// icones ///
	Object n1;
	n1.initialize();
	n1.setTexture(texID1);
	n1.setShader(shadern1);
	n1.setDimensions(glm::vec3(100.0, 100.0, 1.0));
	n1.setPosition(glm::vec3(70.0, 100.0, 0.0));

	Object n2;
	n2.initialize();
	n2.setTexture(texID2);
	n2.setShader(shadern2);
	n2.setDimensions(glm::vec3(100.0, 100.0, 1.0));
	n2.setPosition(glm::vec3(180.0, 100.0, 0.0));

	Object n3;
	n3.initialize();
	n3.setTexture(texID3);
	n3.setShader(shadern3);
	n3.setDimensions(glm::vec3(100.0, 100.0, 1.0));
	n3.setPosition(glm::vec3(290.0, 100.0, 0.0));

	Object n4;
	n4.initialize();
	n4.setTexture(texID4);
	n4.setShader(shadern4);
	n4.setDimensions(glm::vec3(100.0, 100.0, 1.0));
	n4.setPosition(glm::vec3(400.0, 100.0, 0.0));

	Object n5;
	n5.initialize();
	n5.setTexture(texID5);
	n5.setShader(shadern5);
	n5.setDimensions(glm::vec3(100.0, 100.0, 1.0));
	n5.setPosition(glm::vec3(510.0, 100.0, 0.0));

	Object n6;
	n6.initialize();
	n6.setTexture(texID6);
	n6.setShader(shadern6);
	n6.setDimensions(glm::vec3(100.0, 100.0, 1.0));
	n6.setPosition(glm::vec3(620.0, 100.0, 0.0));

	Object n7;
	n7.initialize();
	n7.setTexture(texID7);
	n7.setShader(shadern7);
	n7.setDimensions(glm::vec3(100.0, 100.0, 1.0));
	n7.setPosition(glm::vec3(730.0, 100.0, 0.0));

	///// stickers /////
	Object sticker1;
	sticker1.initialize();
	sticker1.setTexture(texIDstk1);
	sticker1.setShader(shaderstk1);
	sticker1.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	//sticker1.setPosition(glm::vec3(400.0, 340.0, 0.0));
	sticker1.setPosition(glm::vec3(sXpos, sYpos, 0.0));

	Object sticker2;
	sticker2.initialize();
	sticker2.setTexture(texIDstk2);
	sticker2.setShader(shaderstk2);
	sticker2.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	sticker2.setPosition(glm::vec3(sXpos, sYpos, 0.0));

	Object sticker3;
	sticker3.initialize();
	sticker3.setTexture(texIDstk3);
	sticker3.setShader(shaderstk3);
	sticker3.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	sticker3.setPosition(glm::vec3(sXpos, sYpos, 0.0));

	Object sticker4;
	sticker4.initialize();
	sticker4.setTexture(texIDstk4);
	sticker4.setShader(shaderstk4);
	sticker4.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	sticker4.setPosition(glm::vec3(sXpos, sYpos, 0.0));

	Object sticker5;
	sticker5.initialize();
	sticker5.setTexture(texIDstk5);
	sticker5.setShader(shaderstk5);
	sticker5.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	sticker5.setPosition(glm::vec3(sXpos, sYpos, 0.0));

	/// ICONES STICKERS ///
	Object istk1;
	istk1.initialize();
	istk1.setTexture(texIDstk1);
	istk1.setShader(shaderistk1);
	istk1.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	istk1.setPosition(glm::vec3(70.0, 550.0, 0.0));

	Object istk2;
	istk2.initialize();
	istk2.setTexture(texIDstk2);
	istk2.setShader(shaderistk2);
	istk2.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	istk2.setPosition(glm::vec3(180.0, 550.0, 0.0));

	Object istk3;
	istk3.initialize();
	istk3.setTexture(texIDstk3);
	istk3.setShader(shaderistk3);
	istk3.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	istk3.setPosition(glm::vec3(290.0, 550.0, 0.0));

	Object istk4;
	istk4.initialize();
	istk4.setTexture(texIDstk4);
	istk4.setShader(shaderistk4);
	istk4.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	istk4.setPosition(glm::vec3(400.0, 550.0, 0.0));

	Object istk5;
	istk5.initialize();
	istk5.setTexture(texIDstk5);
	istk5.setShader(shaderistk5);
	istk5.setDimensions(glm::vec3(50.0, 50.0, 1.0));
	istk5.setPosition(glm::vec3(510.0, 550.0, 0.0));

#pragma endregion

#pragma region Setup Shaders


	/// STICKERS ///
	shaderstk1->Use();
	shaderstk1->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocstk1 = glGetUniformLocation(shaderstk1->Program, "projection");
	assert(projLocstk1 > -1);
	glUniform1i(glGetUniformLocation(shaderstk1->Program, "texstk1"), 0);

	shaderstk2->Use();
	shaderstk2->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocstk2 = glGetUniformLocation(shaderstk2->Program, "projection");
	assert(projLocstk2 > -1);
	glUniform1i(glGetUniformLocation(shaderstk2->Program, "texstk2"), 0);

	shaderstk3->Use();
	shaderstk3->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocstk3 = glGetUniformLocation(shaderstk3->Program, "projection");
	assert(projLocstk3 > -1);
	glUniform1i(glGetUniformLocation(shaderstk3->Program, "texstk3"), 0);

	shaderstk4->Use();
	shaderstk4->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocstk4 = glGetUniformLocation(shaderstk4->Program, "projection");
	assert(projLocstk4 > -1);
	glUniform1i(glGetUniformLocation(shaderstk4->Program, "texstk4"), 0);

	shaderstk5->Use();
	shaderstk5->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocstk5 = glGetUniformLocation(shaderstk5->Program, "projection");
	assert(projLocstk5 > -1);
	glUniform1i(glGetUniformLocation(shaderstk5->Program, "texstk5"), 0);

	// STICKER ICONES //
	shaderistk1->Use();
	shaderistk1->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocistk1 = glGetUniformLocation(shaderistk1->Program, "projection");
	assert(projLocistk1 > -1);
	glUniform1i(glGetUniformLocation(shaderistk1->Program, "texistk1"), 0);

	shaderistk2->Use();
	shaderistk2->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocistk2 = glGetUniformLocation(shaderistk2->Program, "projection");
	assert(projLocistk2 > -1);
	glUniform1i(glGetUniformLocation(shaderistk2->Program, "texistk2"), 0);

	shaderistk3->Use();
	shaderistk3->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocistk3 = glGetUniformLocation(shaderistk3->Program, "projection");
	assert(projLocistk3 > -1);
	glUniform1i(glGetUniformLocation(shaderistk3->Program, "texistk3"), 0);

	shaderistk4->Use();
	shaderistk4->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocistk4 = glGetUniformLocation(shaderistk4->Program, "projection");
	assert(projLocistk4 > -1);
	glUniform1i(glGetUniformLocation(shaderistk4->Program, "texistk4"), 0);

	shaderistk5->Use();
	shaderistk5->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocistk5 = glGetUniformLocation(shaderistk5->Program, "projection");
	assert(projLocistk5 > -1);
	glUniform1i(glGetUniformLocation(shaderistk5->Program, "texistk5"), 0);

	///// SHADERS /////
	shaderimagem->Use();
	shaderimagem->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocn = glGetUniformLocation(shaderimagem->Program, "projection");
	assert(projLocn > -1);
	glUniform1i(glGetUniformLocation(shaderimagem->Program, "teximagem"), 0);

	shadern1->Use();
	shadern1->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocsn1 = glGetUniformLocation(shadern1->Program, "projection");
	assert(projLocsn1 > -1);
	glUniform1i(glGetUniformLocation(shadern1->Program, "texsn1"), 0);

	shadern2->Use();
	shadern2->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocsn2 = glGetUniformLocation(shadern2->Program, "projection");
	assert(projLocsn2 > -1);
	glUniform1i(glGetUniformLocation(shadern2->Program, "texsn2"), 0);

	shadern3->Use();
	shadern3->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocsn3 = glGetUniformLocation(shadern3->Program, "projection");
	assert(projLocsn3 > -1);
	glUniform1i(glGetUniformLocation(shadern3->Program, "texsn3"), 0);

	shadern4->Use();
	shadern4->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocsn4 = glGetUniformLocation(shadern4->Program, "projection");
	assert(projLocsn4 > -1);
	glUniform1i(glGetUniformLocation(shadern4->Program, "texsn4"), 0);

	shadern5->Use();
	shadern5->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocsn5 = glGetUniformLocation(shadern5->Program, "projection");
	assert(projLocsn5 > -1);
	glUniform1i(glGetUniformLocation(shadern5->Program, "texsn5"), 0);

	shadern6->Use();
	shadern6->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocsn6 = glGetUniformLocation(shadern6->Program, "projection");
	assert(projLocsn6 > -1);
	glUniform1i(glGetUniformLocation(shadern6->Program, "texsn6"), 0);

	shadern7->Use();
	shadern7->setVec4("corColarizadora", 0.5, 0.5, 0.5, 1.0);
	GLint projLocsn7 = glGetUniformLocation(shadern7->Program, "projection");
	assert(projLocsn7 > -1);
	glUniform1i(glGetUniformLocation(shadern7->Program, "texsn7"), 0);

	// shader 1 //
	shader->Use();
	shader->setInt("idCanal", R); // vermelho
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	assert(projLoc > -1);
	glUniform1i(glGetUniformLocation(shader->Program, "tex1"), 0);

	// shader 2 //
	shader2->Use();
	shader2->setInt("idCanal", G); // verde
	GLint projLoc2 = glGetUniformLocation(shader2->Program, "projection");
	assert(projLoc2 > -1);
	glUniform1i(glGetUniformLocation(shader2->Program, "tex2"), 0);

	// shader 3 //
	shader3->Use();
	shader3->setInt("idCanal", B); // azul
	GLint projLoc3 = glGetUniformLocation(shader3->Program, "projection");
	assert(projLoc3 > -1);
	glUniform1i(glGetUniformLocation(shader3->Program, "tex3"), 0);

	// shader 4 //
	shader4->Use();
	shader4->setVec4("corColarizadora", 1.0, 0.0, 0.0, 1.0); // greyscale
	GLint projLoc4 = glGetUniformLocation(shader4->Program, "projection");
	assert(projLoc4 > -1);
	glUniform1i(glGetUniformLocation(shader4->Program, "tex4"), 0);

	// shader 5 //
	shader5->Use();
	shader5->setVec4("corColarizadora", 0.6, 0.4, 0.8, 1.0); // colorização
	GLint projLoc5 = glGetUniformLocation(shader5->Program, "projection");
	assert(projLoc5 > -1);
	glUniform1i(glGetUniformLocation(shader5->Program, "tex5"), 0);

	// shader 6 //
	shader6->Use();
	shader6->setVec4("corColarizadora", 1.0, 0.0, 0.0, 1.0); // inversão
	GLint projLoc6 = glGetUniformLocation(shader6->Program, "projection");
	assert(projLoc6 > -1);
	glUniform1i(glGetUniformLocation(shader6->Program, "tex6"), 0);

	// shader 7 //
	shader7->Use();
	shader7->setVec4("corColarizadora", 1.0, 0.0, 0.0, 1.0); // binarização
	GLint projLoc7 = glGetUniformLocation(shader7->Program, "projection");
	assert(projLoc7 > -1);
	glUniform1i(glGetUniformLocation(shader7->Program, "tex7"), 0);


#pragma endregion

#pragma region Window setup

	glm::mat4 ortho = glm::mat4(1); //inicializa com a matriz identidade

	// dimensões do mundo
	double xmin = 0.0, xmax = 800.0, ymin = 0.0, ymax = 600.0;

	// Habilitar profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	// Habilar transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#pragma endregion


	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region Icones Filtros Update&Draw

		// ICONES FILTROS //
		shadern1->Use();
		glUniformMatrix4fv(projLocsn1, 1, GL_FALSE, glm::value_ptr(ortho));
		n1.update();
		n1.draw();

		shadern2->Use();
		glUniformMatrix4fv(projLocsn2, 1, GL_FALSE, glm::value_ptr(ortho));
		n2.update();
		n2.draw();

		shadern3->Use();
		glUniformMatrix4fv(projLocsn3, 1, GL_FALSE, glm::value_ptr(ortho));
		n3.update();
		n3.draw();

		shadern4->Use();
		glUniformMatrix4fv(projLocsn4, 1, GL_FALSE, glm::value_ptr(ortho));
		n4.update();
		n4.draw();

		shadern5->Use();
		glUniformMatrix4fv(projLocsn5, 1, GL_FALSE, glm::value_ptr(ortho));
		n5.update();
		n5.draw();

		shadern6->Use();
		glUniformMatrix4fv(projLocsn6, 1, GL_FALSE, glm::value_ptr(ortho));
		n6.update();
		n6.draw();

		shadern7->Use();
		glUniformMatrix4fv(projLocsn7, 1, GL_FALSE, glm::value_ptr(ortho));
		n7.update();
		n7.draw();

#pragma endregion

#pragma region Icones Stickers Update&Draw

		// ICONE STICKERS //
		shaderistk1->Use();
		glUniformMatrix4fv(projLocistk1, 1, GL_FALSE, glm::value_ptr(ortho));
		istk1.update();
		istk1.draw();

		shaderistk2->Use();
		glUniformMatrix4fv(projLocistk2, 1, GL_FALSE, glm::value_ptr(ortho));
		istk2.update();
		istk2.draw();

		shaderistk3->Use();
		glUniformMatrix4fv(projLocistk3, 1, GL_FALSE, glm::value_ptr(ortho));
		istk3.update();
		istk3.draw();

		shaderistk4->Use();
		glUniformMatrix4fv(projLocistk4, 1, GL_FALSE, glm::value_ptr(ortho));
		istk4.update();
		istk4.draw();

		shaderistk5->Use();
		glUniformMatrix4fv(projLocistk5, 1, GL_FALSE, glm::value_ptr(ortho));
		istk5.update();
		istk5.draw();

#pragma endregion

		// IMAGEM ORIGINAL //
		shaderimagem->Use();
		glUniformMatrix4fv(projLocn, 1, GL_FALSE, glm::value_ptr(ortho));
		image.update();
		image.draw();

		// Atualização da matrix de projeção
		ortho = glm::ortho(xmin, xmax, ymin, ymax, -1.0, 1.0);


		// FILTROS //
		switch (s)
		{
		case 1:
			shader->Use();
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));
			f1.update();
			f1.draw();
			break;

		case 2:
			shader2->Use();
			glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(ortho));
			f2.update();
			f2.draw();
			break;

		case 3:
			shader3->Use();
			glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(ortho));
			f3.update();
			f3.draw();
			break;

		case 4:
			shader4->Use();
			glUniformMatrix4fv(projLoc4, 1, GL_FALSE, glm::value_ptr(ortho));
			f4.update();
			f4.draw();
			break;

		case 5:
			shader5->Use();
			glUniformMatrix4fv(projLoc5, 1, GL_FALSE, glm::value_ptr(ortho));
			f5.update();
			f5.draw();
			break;

		case 6:
			shader6->Use();
			glUniformMatrix4fv(projLoc6, 1, GL_FALSE, glm::value_ptr(ortho));
			f6.update();
			f6.draw();
			break;

		case 7:
			shader7->Use();
			glUniformMatrix4fv(projLoc7, 1, GL_FALSE, glm::value_ptr(ortho));
			f7.update();
			f7.draw();
			break;
		}

		// STICKERS //

		if (keepsticker[0]) {
			shaderstk1->Use();
			glUniformMatrix4fv(projLocstk1, 1, GL_FALSE, glm::value_ptr(ortho));

			if (movesticker[0])
				sticker1.setPosition(glm::vec3(sXpos, sYpos, 0.0));

			sticker1.update();
			sticker1.draw();
		}

		if (keepsticker[1]) {
			shaderstk2->Use();
			glUniformMatrix4fv(projLocstk2, 1, GL_FALSE, glm::value_ptr(ortho));

			if (movesticker[1])
				sticker2.setPosition(glm::vec3(sXpos, sYpos, 0.0));

			sticker2.update();
			sticker2.draw();
		}

		if (keepsticker[2]) {
			shaderstk3->Use();
			glUniformMatrix4fv(projLocstk3, 1, GL_FALSE, glm::value_ptr(ortho));

			if (movesticker[2])
				sticker3.setPosition(glm::vec3(sXpos, sYpos, 0.0));

			sticker3.update();
			sticker3.draw();
		}

		if (keepsticker[3]) {
			shaderstk4->Use();
			glUniformMatrix4fv(projLocstk4, 1, GL_FALSE, glm::value_ptr(ortho));

			if (movesticker[3])
				sticker4.setPosition(glm::vec3(sXpos, sYpos, 0.0));

			sticker4.update();
			sticker4.draw();
		}

		if (keepsticker[4]) {
			shaderstk5->Use();
			glUniformMatrix4fv(projLocstk5, 1, GL_FALSE, glm::value_ptr(ortho));

			if (movesticker[4])
				sticker5.setPosition(glm::vec3(sXpos, sYpos, 0.0));

			sticker5.update();
			sticker5.draw();
		}


		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;

#pragma endregion

}

#pragma region Input Callback


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// filtro 8
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		s = 8;
	}
}

// POSIÇÃO DO MOUSE NA JANELA
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	cout << xpos << " : " << ypos << endl;
	if (xpos >= 21 && xpos <= 115 && ypos >= 452 && ypos <= 545)
	{
		filtro1 = true;
	}

	else if (xpos >= 134 && xpos <= 225 && ypos >= 452 && ypos <= 545)
	{
		filtro2 = true;
	}

	else if (xpos >= 244 && xpos <= 335 && ypos >= 452 && ypos <= 545)
	{
		filtro3 = true;
	}

	else if (xpos >= 354 && xpos <= 445 && ypos >= 452 && ypos <= 545)
	{
		filtro4 = true;
	}

	else if (xpos >= 463 && xpos <= 555 && ypos >= 452 && ypos <= 545)
	{
		filtro5 = true;
	}

	else if (xpos >= 573 && xpos <= 666 && ypos >= 452 && ypos <= 545)
	{
		filtro6 = true;
	}

	else if (xpos >= 684 && xpos <= 775 && ypos >= 452 && ypos <= 545)
	{
		filtro7 = true;
	}

	/// ICONE STICKERS ///
	else if (xpos >= 54 && xpos <= 87 && ypos >= 35 && ypos <= 68)
	{
		sticker1 = true;
	}

	else if (xpos >= 160 && xpos <= 196 && ypos >= 35 && ypos <= 68)
	{
		sticker2 = true;
	}

	else if (xpos >= 264 && xpos <= 307 && ypos >= 35 && ypos <= 68)
	{
		sticker3 = true;
	}

	else if (xpos >= 376 && xpos <= 415 && ypos >= 35 && ypos <= 68)
	{
		sticker4 = true;
	}

	else if (xpos >= 480 && xpos <= 535 && ypos >= 35 && ypos <= 68)
	{
		sticker5 = true;
	}

	if (sticker1 || sticker2 || sticker3 || sticker4 || sticker5)
	{
		sXpos = xpos;
		sYpos = 600 - ypos;
	}

}

// VERIFICA SE O MOUSE ESTÁ DENTRO/FORA DA JANELA
void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		cout << "mouse dentro da janela" << endl;
	}
	else
	{
		cout << "mouse fora da janela" << endl;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		cout << "botão esquerdo pressionado" << endl;
		//clickmouse = true;

		if (filtro1 == true)
		{
			s = 1;
		}

		else if (filtro2 == true)
		{
			s = 2;
		}

		else if (filtro3 == true)
		{
			s = 3;
		}

		else if (filtro4 == true)
		{
			s = 4;
		}

		else if (filtro5 == true)
		{
			s = 5;
		}

		else if (filtro6 == true)
		{
			s = 6;
		}

		else if (filtro7 == true)
		{
			s = 7;
		}

		/// ICONE STICKERS ///
		else if (sticker1 == true)
		{
			movesticker[0] = true;
			keepsticker[0] = !keepsticker[0];
		}

		else if (sticker2 == true)
		{
			movesticker[1] = true;
			keepsticker[1] = !keepsticker[1];
		}

		else if (sticker3 == true)
		{
			movesticker[2] = true;
			keepsticker[2] = !keepsticker[2];
		}

		else if (sticker4 == true)
		{
			movesticker[3] = true;
			keepsticker[3] = !keepsticker[3];
		}

		else if (sticker5 == true)
		{
			movesticker[4] = true;
			keepsticker[4] = !keepsticker[4];
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		cout << "botão esquerdo solto" << endl;
		filtro1 = false;
		filtro2 = false;
		filtro3 = false;
		filtro4 = false;
		filtro5 = false;
		filtro6 = false;
		filtro7 = false;

		sticker1 = false;
		sticker2 = false;
		sticker3 = false;
		sticker4 = false;
		sticker5 = false;
		
		for (int i = 0; i < 5; i++)
		{
			movesticker[i] = false;
		}
		



	}
}

#pragma endregion

int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat* vertices;

	vertices = new GLfloat[nPoints * 3];

	float angle = 0.0;
	float deltaAngle = 2 * pi / (float)(nPoints - 2);
	float radius = 0.5;

	//Adicionar o centro
	vertices[0] = 0.0; // x
	vertices[1] = 0.0; // y
	vertices[2] = 0.0; // z sempre zero 

	for (int i = 3; i < nPoints * 3; i += 3)
	{
		vertices[i] = radius * cos(angle);
		vertices[i + 1] = radius * sin(angle);
		vertices[i + 2] = 0.0;

		angle += deltaAngle;
	}

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, (nPoints * 3) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

GLuint createSprite()
{
	GLuint VAO;
	GLuint VBO, EBO;

	float vertices[] = {
		// positions          // colors          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};
	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}

