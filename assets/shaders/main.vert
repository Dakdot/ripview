#version 330 core

layout(location = 0) in vec3 a_VertexPos;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec4 VertexColor;

void main() {
  gl_Position = u_Proj * u_View * u_Model * vec4(a_VertexPos, 1.0);
  VertexColor = vec4(a_VertexPos.x / 30.0, a_VertexPos.y / 30.0, a_VertexPos.z / 30.0, 1.0);
}
