#version 330

in vec4 vPosition;
in vec4 vColor;
out vec4 color2;

void main () 
{
  gl_Position = vPosition;
  color2 = vColor;
}
