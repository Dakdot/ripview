#version 330 core

in vec3 FragPos;
in vec3 VertexNormal;

uniform vec3 u_ObjColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform float u_Roughness;

out vec4 FragColor;

void main() {
  float ambientStrength = 0.1;
  float specularStrength = 1.0 - u_Roughness;

  vec3 norm = normalize(VertexNormal);
  vec3 lightDir = normalize(u_LightPos - FragPos);

  vec3 viewDir = normalize(u_ViewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  vec3 ambient = ambientStrength * u_LightColor;

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * u_LightColor;

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
  vec3 specular = specularStrength * spec * u_LightColor;

  vec3 result = (ambient + diffuse + specular) * u_ObjColor;
  FragColor = vec4(result, 1.0);
}
