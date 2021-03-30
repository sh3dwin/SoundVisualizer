#pragma once

const char* fragmentShaderSource =
"version #330 core\n"
"uniform vec4 color;\n"
"out vec4 FragmentColor;\n"
"int main() {\n"
"	FragmentColor = color;\n"
"}\n\0";

const char* vertexShaderSource =
"version #330 core\n"
"layout (location = 0) in vec3 position"
"int main(){\n"
" gl_Position = vec4(position, 1.0)\n"
"}\n\0";