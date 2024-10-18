#include "Box.h"

Box::Box()
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

	auto polygon = system_state->GetCurrent()->GetBoundaryPoints();
	std::vector<float> vertices;
	vertices.push_back(polygon[0]);
	vertices.push_back(polygon[1]);
	for (int i = 2; i < polygon.size(); i += 2)
	{
		vertices.push_back(polygon[i]);
		vertices.push_back(polygon[i + 1]);
		vertices.push_back(polygon[i]);
		vertices.push_back(polygon[i + 1]);
	}
	vertices.push_back(polygon[0]);
	vertices.push_back(polygon[1]);
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, NUM_CORNERS * 2 * POS_COORDS * sizeof(float), &vertices[0], GL_STATIC_DRAW);
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
