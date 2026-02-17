#version 330 core

layout(location = 0) in vec3 a_VertexPos;

uniform mat4 u_Proj;

out vec4 VertexColor;

void main() {
    gl_Position = u_Proj * vec4(a_VertexPos, 1.0);
    VertexColor = vec4(a_VertexPos.x / 200.0, a_VertexPos.y / 200.0, 1.0, 1.0);
}
