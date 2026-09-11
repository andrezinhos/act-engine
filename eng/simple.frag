#version 330 core
out vec4 FragColor;

in vec4 ncolor;
in vec2 texCoord;
uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, texCoord) * vec4(ncolor);
}
