#include "stdafx.h"
#include "Cube.h"

#include "stdafx.h"

// количество вершин куба
static const GLuint cubeVerticesCount = 24;

// описание геометрии куба для всех его сторон
// координаты вершин куба
const float s = 1.0f; // половина размера куба
const float cubePositions[cubeVerticesCount][3] = {
        {-s, s, s}, { s, s, s}, { s,-s, s}, {-s,-s, s}, // front
        { s, s,-s}, {-s, s,-s}, {-s,-s,-s}, { s,-s,-s}, // back
        {-s, s,-s}, { s, s,-s}, { s, s, s}, {-s, s, s}, // top
        { s,-s,-s}, {-s,-s,-s}, {-s,-s, s}, { s,-s, s}, // bottom
        {-s, s,-s}, {-s, s, s}, {-s,-s, s}, {-s,-s,-s}, // left
        { s, s, s}, { s, s,-s}, { s,-s,-s}, { s,-s, s}  // right
};

// количество индексов куба
const GLuint cubeIndicesCount = 36;

// индексы вершин куба в порядке поротив часовой стрелки
const GLuint cubeIndices[cubeIndicesCount] = {
         0, 3, 1,  1, 3, 2, // front
         4, 7, 5,  5, 7, 6, // back
         8,11, 9,  9,11,10, // top
        12,15,13, 13,15,14, // bottom
        16,19,17, 17,19,18, // left
        20,23,21, 21,23,22  // right
};

void Cube::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	// переменные для хранения VAO и VBO связанных с кубом
	GLuint cubeVBO[3];

	// переменные для хранения индексов вершинных атрибутов
	GLint positionLocation = -1, texcoordLocation = -1;

	// создаем VAO
	glGenVertexArrays(1, &cubeVAO);

	// сделаем VAO активным
	glBindVertexArray(cubeVAO);

	// создадим 3 VBO для данных куба - координаты вершин, текстурные координат и индексный буфер
	glGenBuffers(3, cubeVBO);

	// начинаем работу с индексным буфером
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVBO[2]);
	// поместим в буфер индексы вершин куба
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesCount * sizeof(GLuint),
			cubeIndices, GL_STATIC_DRAW);
}

void Cube::renderImpl()
{
	glEnableVertexAttribArray(0);
	// сделаем VAO куба активным
	glBindVertexArray(cubeVAO);

	// вывдоим куб на экран
	glDrawElements(GL_TRIANGLES, cubeIndicesCount, GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(0);
}
