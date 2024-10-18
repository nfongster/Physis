#version 330 core

layout(location = 0) in
vec4 initial_position;

uniform vec2 u_Position;

void main()
{
    gl_Position = vec4(initial_position) + vec4(u_Position.x, u_Position.y, 0, 0);
};