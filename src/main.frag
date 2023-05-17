#version 330 core
layout(location = 0) out vec4 o_color;

in vec4 v_color;
uniform float u_random_number;

void main() {
    o_color = vec4(v_color.r + u_random_number, v_color.g, v_color.b, v_color.a);
}