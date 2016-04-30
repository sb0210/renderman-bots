#include "meshing.h"
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include <unistd.h>
GLuint shaderProgram;
GLuint vbo, vao;

int num_vertices;
std::vector<glm::vec4> v_positions;
std::vector<glm::vec4> v_colors;


float radius = 30.0;
float center_x = 0;
float center_y = 0;
float partitions = 6;
vector<Node*> nodes;
vector<Triangle*> triangles;
Mesh* mesh;
float pi=3.1415;


void initiateMesh(){

  Node* n =  new Node(0,0);
  nodes.push_back(n);
  nodes[0]->id = 0;
  float angle = 2*3.1415/partitions;
  for(int i=0;i<partitions;i++){
    Node* n1 = new Node(center_x + (radius/2)*cos(i*angle),center_y + (radius/2)*sin(i*angle));
    nodes.push_back(n1);
    nodes[2*i+1]->id = 2*i+1;
    n1 = new Node(center_x + (radius)*cos(i*angle),center_y + (radius)*sin(i*angle));
    nodes.push_back(n1);
    nodes[2*i+2]->id = 2*i+2;
    nodes[2*i+2]->is_boundary = true;
  }
  for(int i=0;i<partitions;i++){
    Triangle* t;
    if(i==partitions-1)
      t  = new Triangle(nodes[0],nodes[(2*i+1)%nodes.size()],nodes[(1)%nodes.size()]);
    else
      t = new Triangle(nodes[0],nodes[(2*i+1)%nodes.size()],nodes[(2*i+3)%nodes.size()]);
    triangles.push_back(t);
    nodes[0]->addTriangle(triangles[3*i]);
    nodes[(2*i+1)%nodes.size()]->addTriangle(triangles[3*i]);
    nodes[(2*i+3)%nodes.size()]->addTriangle(triangles[3*i]);
    triangles[3*i]->id =3*i;
    if (i==partitions-1)
      t = new Triangle(nodes[(2*i+1)%nodes.size()],nodes[(2*i+2)%nodes.size()],nodes[(1)%nodes.size()]);
    else
      t = new Triangle(nodes[(2*i+1)%nodes.size()],nodes[(2*i+2)%nodes.size()],nodes[(2*i+3)%nodes.size()]);
    triangles.push_back(t);
    nodes[(2*i+1)%nodes.size()]->addTriangle(triangles[3*i+1]);
    nodes[(2*i+2)%nodes.size()]->addTriangle(triangles[3*i+1]);
    nodes[(2*i+3)%nodes.size()]->addTriangle(triangles[3*i+1]);
    triangles[3*i+1]->id =3*i+1;
    if(i==partitions-1)
      t = new Triangle(nodes[(2*i+2)%nodes.size()],nodes[(1)%nodes.size()],nodes[(2)%nodes.size()]);
    else
      t = new Triangle(nodes[(2*i+2)%nodes.size()],nodes[(2*i+3)%nodes.size()],nodes[(2*i+4)%nodes.size()]);
    triangles.push_back(t);
    nodes[(2*i+2)%nodes.size()]->addTriangle(triangles[3*i+2]);
    nodes[(2*i+3)%nodes.size()]->addTriangle(triangles[3*i+2]);
    nodes[(2*i+4)%nodes.size()]->addTriangle(triangles[3*i+2]);
    triangles[3*i+2]->id =3*i+2;
  } 


  // Node * n = new Node(radius*cos(pi/2),radius*sin(pi/2));
  // Node* n2 = new Node(radius*cos(pi/2 + 2*pi/3),radius*sin(pi/2 + 2*pi/3));
  // Node* n3 = new Node(radius*cos(pi/2 + 4*pi/3),radius*sin(pi/2 + 4*pi/3));
  // Triangle* t = new Triangle(n,n2,n3);
  // n->addTriangle(t);
  // n2->addTriangle(t);
  // n3->addTriangle(t);
  // triangles.push_back(t);
  // nodes.push_back(n);
  // nodes.push_back(n2);
  // nodes.push_back(n3);


}



//-----------------------------------------------------------------


void initBuffersGL(void)
{
  // colorcube();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);

  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer
  //std::cout<<v_positions.size()<<" v_positions.size "<<v_colors.size()<<endl;
  glBufferData (GL_ARRAY_BUFFER, (v_positions.size()+v_colors.size())* sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, v_positions.size()* sizeof(glm::vec4), &v_positions.front() );
  glBufferSubData( GL_ARRAY_BUFFER, v_positions.size()* sizeof(glm::vec4),v_colors.size()* sizeof(glm::vec4), &v_colors.front() );

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("vshader.glsl");
  std::string fragment_shader_file("fshader.glsl");
  std::string g_shader_file("gshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_GEOMETRY_SHADER, g_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((v_positions.size()*sizeof(glm::vec4))) );

  }

void update(){
  mesh->getPositions(v_positions,v_colors);
  // mesh->print();
  mesh->updateMesh();
  initBuffersGL();

}

void renderGL(void)
{
  update();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0,v_positions.size());
}

int main(int argc, char** argv)
{

	initiateMesh();
	mesh = new Mesh(nodes,triangles);
	mesh->print();


  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  cout<<"hiii"<<endl;
  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(512, 512, "CS 775 Project", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  

  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
      sleep(1);
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

