#version 330 core

layout(location = 0) in vec3 a_VertexPos;
layout(location = 1) in vec3 a_VertexNormal;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Normal;

out vec3 FragPos;
out vec3 VertexNormal;

void main() {
  gl_Position = u_Proj * u_View * u_Model * vec4(a_VertexPos, 1.0);

  FragPos = vec3(u_Model * vec4(a_VertexPos, 1.0));
  VertexNormal = mat3(u_Normal) * a_VertexNormal;
}
