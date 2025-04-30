#version 330 core
in vec3 WorldPos;
in vec3 Normal;
out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main() {
    vec3 I = normalize(WorldPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = texture(skybox, R);
}