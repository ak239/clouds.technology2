#include "stdafx.h"
#include "Cube.h"

#include "stdafx.h"

// ���������� ������ ����
static const GLuint cubeVerticesCount = 24;

// �������� ��������� ���� ��� ���� ��� ������
// ���������� ������ ����
const float s = 1.0f; // �������� ������� ����
const float cubePositions[cubeVerticesCount][3] = {
        {-s, s, s}, { s, s, s}, { s,-s, s}, {-s,-s, s}, // front
        { s, s,-s}, {-s, s,-s}, {-s,-s,-s}, { s,-s,-s}, // back
        {-s, s,-s}, { s, s,-s}, { s, s, s}, {-s, s, s}, // top
        { s,-s,-s}, {-s,-s,-s}, {-s,-s, s}, { s,-s, s}, // bottom
        {-s, s,-s}, {-s, s, s}, {-s,-s, s}, {-s,-s,-s}, // left
        { s, s, s}, { s, s,-s}, { s,-s,-s}, { s,-s, s}  // right
};

// ���������� �������� ����
const GLuint cubeIndicesCount = 36;

// ������� ������ ���� � ������� ������� ������� �������
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

	// ���������� ��� �������� VAO � VBO ��������� � �����
	GLuint cubeVBO[3];

	// ���������� ��� �������� �������� ��������� ���������
	GLint positionLocation = -1, texcoordLocation = -1;

	// ������� VAO
	glGenVertexArrays(1, &cubeVAO);

	// ������� VAO ��������
	glBindVertexArray(cubeVAO);

	// �������� 3 VBO ��� ������ ���� - ���������� ������, ���������� ��������� � ��������� �����
	glGenBuffers(3, cubeVBO);

	// �������� ������ � ��������� �������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVBO[2]);
	// �������� � ����� ������� ������ ����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesCount * sizeof(GLuint),
			cubeIndices, GL_STATIC_DRAW);
}

void Cube::renderImpl()
{
	glEnableVertexAttribArray(0);
	// ������� VAO ���� ��������
	glBindVertexArray(cubeVAO);

	// ������� ��� �� �����
	glDrawElements(GL_TRIANGLES, cubeIndicesCount, GL_UNSIGNED_INT, NULL);
	glDisableVertexAttribArray(0);
}
