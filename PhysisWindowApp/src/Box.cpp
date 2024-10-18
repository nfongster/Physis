#include "Box.h"

Box::Box(float& height, float& width) : m_height(height), m_width(width)
{
}

Box::~Box()
{
}

void Box::Initialize(std::shared_ptr<SystemState>& system_state)
{
	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5, 6, 7
	};

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_CORNERS * 2 * sizeof(int), indices, GL_STATIC_DRAW);
	
	float half_width = m_width / 2;
	float half_height = m_height / 2;
	float vertices[] = {
		-half_width, -half_height, // Left Edge
		-half_width,  half_height,

		-half_width,  half_height, // Top Edge
		 half_width,  half_height,

		 half_width,  half_height, // Right Edge
		 half_width, -half_height,

		 half_width, -half_height, // Bottom Edge
		-half_width, -half_height
	};
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, NUM_CORNERS * 2 * POS_COORDS * sizeof(float), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, POS_COORDS, GL_FLOAT, GL_FALSE, sizeof(float) * POS_COORDS, 0);

	auto builder = ShaderBuilder("C:\\Code\\Physis\\PhysisWindowApp\\resource\\shaders\\box\\");
	m_shader = builder.Build();
}

void Box::Render()
{
	glUseProgram(m_shader);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_LINES, NUM_CORNERS * 2, GL_UNSIGNED_INT, nullptr);
}
