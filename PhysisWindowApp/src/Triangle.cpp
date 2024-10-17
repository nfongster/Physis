#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
    glDeleteProgram(m_shader_id);
}

void Triangle::Initialize(std::shared_ptr<SystemState>& system_state)
{
    unsigned int indices[] = {
        0, 1, 2
    };

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_TRIANGLE_CORNERS * sizeof(int), indices, GL_STATIC_DRAW);

    // get initial position
    float l = 0.03;  // side length of equilateral triangle
    float c = (l / 2.0f) * (std::sqrt(3) / 3.0f);  // vertical distance to base

    for (const auto& pair : system_state->GetCurrent()->GetParticles())
    {
        std::shared_ptr<Particle> p = pair.second;
        float pos[2 * 3];  // 2 coords * 3 vertices
        // todo: initalize separately
        // todo: create normalizer class
        float center_x = p->GetPosition().X;
        float center_y = p->GetPosition().Y;
        // vertex 1
        pos[0] = center_x;
        pos[1] = center_y + c;
        // vertex 2
        pos[2] = center_x - (l / 2);
        pos[3] = center_y - c;
        // vertex 3
        pos[4] = center_x + (l / 2);
        pos[5] = center_y - c;

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * POS_COORDS * sizeof(float), pos, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, POS_COORDS, GL_FLOAT, GL_FALSE, sizeof(float) * POS_COORDS, 0);

        m_vao_map.emplace(vao, p);
    }

    auto builder = ShaderBuilder("C:\\Code\\Physis\\PhysisWindowApp\\resource\\shaders\\");
    m_shader_id = builder.Build();
    glUseProgram(m_shader_id);
    glGetUniformLocation(m_u_position_id, "u_Position");
}

void Triangle::Render()
{
    for (const auto& p : m_vao_map)
    {
        Vec2 pos = p.second->GetPosition();
        glUniform2f(m_u_position_id, pos.X, pos.Y);
        glBindVertexArray(p.first);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glDrawElements(GL_TRIANGLES, NUM_TRIANGLE_CORNERS, GL_UNSIGNED_INT, nullptr);
    }
}
