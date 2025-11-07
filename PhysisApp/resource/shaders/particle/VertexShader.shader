#version 330 core

layout(location = 0) in vec2 vertex_Position;

uniform vec2 u_Position;

void main()
{
    vec2 updated_Position = vertex_Position + u_Position;
    gl_Position = vec4(updated_Position, 0.0, 1.0);
};