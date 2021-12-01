#include "Object.h"

Object::~Object()
{	
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
}

void Object::initialize()
{
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

	transform = glm::mat4(1);
	texID = -1;
	shader = NULL;
	position = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
	angle = 0.0;
}

void Object::update()
{
	shader->Use();

	setTranslation(position, true);
	setRotation(angle, glm::vec3(0.0, 0.0, 1.0));
	setScale(scale);

	shader->setMat4("model", glm::value_ptr(transform));
}

void Object::draw()
{
	shader->Use();

	/// FILTROS //
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, texID);

	///// ICONES FILTROS  /////
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, texID);

	///// STICKERS /////
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE17);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE18);
	glBindTexture(GL_TEXTURE_2D, texID);

	// icones stickers //
	glActiveTexture(GL_TEXTURE19);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE20);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE21);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE22);
	glBindTexture(GL_TEXTURE_2D, texID);

	glActiveTexture(GL_TEXTURE23);
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Object::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset)
	{
		transform = glm::mat4(1);
	}
	transform = glm::rotate(transform, angle, axis);
}

void Object::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset)
	{
		transform = glm::mat4(1);
	}
	transform = glm::translate(transform, displacements);
}

void Object::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset)
	{
		transform = glm::mat4(1);
	}
	transform = glm::scale(transform, scaleFactors);
}
