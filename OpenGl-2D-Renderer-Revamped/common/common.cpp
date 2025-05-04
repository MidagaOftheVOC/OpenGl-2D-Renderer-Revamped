
#include "common.h"

template <typename t>
vec2<t> vec2<t>::operator+(const vec2<t>& other) {
	return { x + other.x, y + other.y };
}


template <typename t>
vec2<t> vec2<t>::operator-(const vec2<t>& other) {
	return { x - other.x, y - other.y };
}


template <typename t>
float vec2<t>::GetLength() {
	return static_cast<float>(sqrt(x * x + y * y));
}

template <typename t>
void vec2<t>::SetAngle(float radian) {
	float vecLength = GetLength();

	t xNew, yNew;
	float length = GetLength();

	xNew = x * cos(radian) * length;
	yNew = y * sin(radian) * length;

	x = xNew;
	y = yNew;
}


bool fEqual(
	float _val1,
	float _val2) 
{
	return 
		( abs(_val1 - _val2) < FLOAT_COMPARE_TOLERANCE ) ? true : false;
}




void StandardQuad::Unbind() {
	glBindVertexArray(0);
}

void StandardQuad::Init() {

	unsigned int  g_stdVertexCoordArray[] = {
		0,		0,
		0,		100,
		100,	100,
		100,	0
	};

	float g_stdTexCoordArray[] = {
		0.f,	0.f,
		0.f,	1.f,
		1.f,	1.f,
		1.f,	0.f
	};

	unsigned short g_stdIndexArray[] = {
		0,	1,	2,
		0,	2,	3
	};


	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VextexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VextexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_stdVertexCoordArray), g_stdVertexCoordArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_TexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_stdTexCoordArray), g_stdTexCoordArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_stdIndexArray), g_stdIndexArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void StandardQuad::Bind() {
	glBindVertexArray(m_VAO);
}

