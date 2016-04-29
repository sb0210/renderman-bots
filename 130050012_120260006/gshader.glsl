#version 330

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

out vec4 color;
void main () 
{
  gl_Position = gl_in[0].gl_Position; color = vec4(1,0,0,1); EmitVertex();
  gl_Position = gl_in[1].gl_Position; color = vec4(1,0,0,1);EmitVertex();
  gl_Position = gl_in[2].gl_Position; color = vec4(1,0,0,1);EmitVertex();
  gl_Position = gl_in[0].gl_Position; color = vec4(1,0,0,1);EmitVertex();
  EndPrimitive();
}
