/*
  CS 475: Assignment 1
  Made by: Charmi Dedhia(130070007) and Siddharth Bulia(130050012)
  Reference : Tutorial 3 by Parag Chaudari
*/


#include "assignment1.hpp"
using namespace std;

GLuint shaderProgram;
GLuint vbo, vao;

glm::mat4 rotation_matrix(1.0f), translation_matrix;
glm::mat4 initial_rotation_matrix(1.0f);
GLuint uRotationMatrix;

char mode='I';
int counter=0;
int initial_size;
//-----------------------------------------------------------------

//6 faces, 2 triangles/face, 3 vertices/triangle
int num_vertices;
std::vector<glm::vec4> v_positions;
std::vector<glm::vec4> v_colors;

void centroid(GLdouble c[]){
  GLdouble x=0.0,y=0.0,z=0.0;
  vector<glm::vec4> pos_nodup; 
  for(int i=0;i<v_positions.size();i++){
    bool duplicate=false;
    for(int j=0;j<pos_nodup.size();j++){
      if (v_positions[i]==pos_nodup[j]){
        duplicate=true;
        break;
      }
    }
    if(duplicate==false)pos_nodup.push_back(v_positions[i]);
  }
  for(int i=0;i<pos_nodup.size();i++){
    x=x+ pos_nodup[i][0];
    y=y+pos_nodup[i][1];
    z=z+pos_nodup[i][2];
  }
  c[0] = x/pos_nodup.size();
  c[1] = y/pos_nodup.size();
  c[2] = z/pos_nodup.size();
}

void remove_redundant_vertices(){
  v_positions.resize((v_positions.size()/3)*3);
  v_colors.resize((v_colors.size()/3)*3);
}

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

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((v_positions.size()*sizeof(glm::vec4))) );

  uRotationMatrix = glGetUniformLocation( shaderProgram, "uRotationMatrix");
}

void rotate(GLfloat x, GLfloat y, GLfloat z){
  GLdouble c[3];
  centroid(c);
  rotation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(c[0],c[1],c[2]));
  rotation_matrix = glm::rotate(rotation_matrix, x, glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, y, glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, z, glm::vec3(0.0f,0.0f,1.0f));
  rotation_matrix = glm::translate(rotation_matrix, glm::vec3(-c[0],-c[1],-c[2]));
  initial_rotation_matrix=rotation_matrix*initial_rotation_matrix;
}

void reinitialize(){
  initial_rotation_matrix=glm::mat4(1.0f);
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(xtrans,ytrans,ztrans));
  glUniformMatrix4fv(uRotationMatrix, 1, GL_FALSE, glm::value_ptr(translation_matrix*initial_rotation_matrix));
  glDrawArrays(GL_TRIANGLES, 0,v_positions.size());
}

void parseline(std::string line,glm::vec4 vect[]){
  std::string strvec[6];
  int i=0;
  int start=0;
  while(line.find(';')!=-1){
    int end= line.find(';');
    strvec[i]=line.substr(1,end-2);
    line = line.substr(end+1,line.length()-end-1);
    float f[4];
    int j=0; 
    while(strvec[i].find(',')!=-1){
      int end1=strvec[i].find(',');
      f[j]=::atof(strvec[i].substr(0,end1).c_str());
      strvec[i] = strvec[i].substr(end1+1,strvec[i].length()-end1-1);
      j++;
    }
    vect[i]=glm::vec4(f[0],f[1],f[2],f[3]);
    i++;
  }
 

}

void load_file(std::string filename){
  std::string line;
  std::ifstream myfile (filename.c_str());
  v_colors.clear();
  v_positions.clear();
  if (myfile.is_open())
  { int i=0;
    int j=0;
    while ( getline (myfile,line) )
    { glm::vec4 v[2];
      parseline(line,v);
      v_positions.push_back(v[0]);
      v_colors.push_back(v[1]);

    }
    myfile.close();
  }
  initBuffersGL();

}

void add_vertex(GLdouble xpos, GLdouble ypos, GLdouble zpos){
    if(counter==0){
      initial_size=v_positions.size();
    }
    counter++;
    if(counter<=3){
      glm::vec4 posvec=glm::vec4(xpos,ypos,zpos,1.0);
      v_positions.push_back(posvec);
      GLdouble c1=1.0f;
      GLdouble c2=1.0f;
      GLdouble c3=1.0f;
      glm::vec4 colorvec=glm::vec4(c1,c2,c3,1.0);
      v_colors.push_back(colorvec);
      initBuffersGL();
   }
   else{
    v_positions.push_back(v_positions[initial_size]);
    v_positions.push_back(v_positions[v_positions.size()-2]);
    v_colors.push_back(v_colors[initial_size]);
    v_colors.push_back(v_colors[v_colors.size()-2]);
    glm::vec4 posvec=glm::vec4(xpos,ypos,zpos,1.0);
      v_positions.push_back(posvec);
      GLdouble c1=1.0f;
      GLdouble c2=1.0f;
      GLdouble c3=1.0f;
      glm::vec4 colorvec=glm::vec4(c1,c2,c3,1.0);
      v_colors.push_back(colorvec);
     
      initBuffersGL();
   }
}

void remove_vertex(){
  if(v_positions.size()==0){
    return;
  }
  if(counter>0 && counter<=3){
  std::cout<<glm::to_string(v_positions[v_colors.size()-1])<<std::endl;
  v_positions.pop_back();
  v_colors.pop_back();
  std::cout<<glm::to_string(v_positions[v_colors.size()-1])<<std::endl;
  initBuffersGL();
  if(counter>0)
  counter--;
  std::cout<<counter<<"\n";
  }
  else{
  v_positions.pop_back();
  v_colors.pop_back();
  v_positions.pop_back();
  v_colors.pop_back();
  v_positions.pop_back();
  v_colors.pop_back();
  initBuffersGL();
  if(counter>0)
  counter--;
  }

}

void save_file(std::string filename){
  std::ofstream myfile;
  myfile.open(filename.c_str());
  v_positions.resize((v_positions.size()/3)*3);
  v_colors.resize((v_colors.size()/3)*3);
  for(int i=0; i<v_positions.size();i++){
     myfile<<"("<<v_positions[i][0]<<','<<v_positions[i][1]<<','<<v_positions[i][2]<<','<<v_positions[i][3]<<",);";
     myfile<<"("<<v_colors[i][0]<<','<<v_colors[i][1]<<','<<v_colors[i][2]<<','<<v_colors[i][3]<<",);\n";
   }
   myfile.close();
}

void clear_screen(){
  v_positions.clear();
  v_colors.clear();
}
//-----------------------------------------------------------------


int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(512, 512, "CS475 Assignment 1", NULL, NULL);
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

  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  glfwSetMouseButtonCallback(window, csX75::mouse_button_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  std::cout<<"L: Load file\nS: Save file\nM: Modelling Mode\nI: Inspection Mode\nC: Clear Screen\n";
  std::cout<<"When in Inspection mode: \n Up, Down, Left, Right, PgUp, PgDn: rotation about X, Y and Z axis respectively\n R: move origin to centroid \n A,D,W,S,Z,X: move along X, Y and Z axes respectively\n";
  std::cout<<"When in modelling mode:\n Left click: add point with z=0\n Ctrl + Left Click: Add point with custom Z value\n Shift + Left Click: Remove Point\n";

  std::cout<<"Currently in Inspection Mode\n";
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
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

