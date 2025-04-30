#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform float time;
uniform sampler2D sunTexture;

void main()
{
    // Flame effect using a sine wave and noise
    float flame = sin(TexCoord.y * 10.0 + time * 5.0) * 0.5 + 0.5;
    vec4 sunColor = texture(sunTexture, TexCoord);
    vec4 flameColor = vec4(1.0, flame, 0.0, 1.0); // Orange flame

    FragColor = mix(sunColor, flameColor, flame);
}