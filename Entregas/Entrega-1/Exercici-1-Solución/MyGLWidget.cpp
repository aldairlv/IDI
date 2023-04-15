#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus); // per rebre events de teclat
}

MyGLWidget::~MyGLWidget()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersQuadrat();
  creaBuffersCavall();
  menjat = false;
  scl = 1.0f;
  angle = 0.0f;
}

void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio)
{
  glm::mat4 TG(1.0f);
   TG = glm::rotate(TG,angle,glm::vec3(0.0f,0.0f,1.0f));
  TG = glm::scale(TG,glm::vec3(scl));
  TG = glm::translate(TG, posicio);
  TG = glm::scale(TG,glm::vec3(0.25,0.25,0.25)); 
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCavall(glm::vec3 posicio)
{
  glm::mat4 TG(1.0f);
   TG = glm::rotate(TG,angle,glm::vec3(0.0f,0.0f,1.0f));
  TG = glm::scale(TG,glm::vec3(scl));
  TG = glm::translate(TG, posicio);
  TG = glm::translate(TG,glm::vec3(0.06,-0.1,0.0));  
  TG = glm::scale(TG,glm::vec3(0.08,0.08,0.08));  
  TG = glm::rotate(TG,float(M_PI)/2,glm::vec3(0.0f,0.0f,1.0f));
  TG = glm::translate(TG,glm::vec3(-0.1,-0.1,0.0));  
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL()
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);

  glClear(GL_COLOR_BUFFER_BIT); // Esborrem el frame-buffer
  // Pintem tauler d'escacs
  glBindVertexArray(VAOQuadrat);
  int k = 0;
  for (float j = -0.875; j < 1.0; j += 0.25) {
      int s = 0;
      for (float i = -0.875; i < 1.0; i += 0.25) {
          if ((k+s)%2 == 0) glUniform3fv(colorLoc,1,&negreTauler[0]);
          else glUniform3fv(colorLoc,1,&blancTauler[0]);
          ++s;
          modelTransformQuadrat(glm::vec3(i,j,0.0));
          glDrawArrays(GL_TRIANGLES, 0, 6);
      }               
      ++k;
  }
  k = 0;
  glBindVertexArray(0);

 

  if (!menjat) glUniform3fv(colorLoc,1,&negre[0]);
  else glUniform3fv(colorLoc,1,&blanc[0]);
  modelTransformCavall(glm::vec3(0.125,0.125,0.0));
  glBindVertexArray(VAOCavall);
  glDrawArrays(GL_TRIANGLES, 0, 45);  
  glBindVertexArray(0);
  
  // Pintem un cavall blanc
  if (!menjat) {  
      glUniform3fv(colorLoc,1,&blanc[0]);
      modelTransformCavall(glm::vec3(-0.125,-0.375,0.0));
      glBindVertexArray(VAOCavall);
      glDrawArrays(GL_TRIANGLES, 0, 45);  
      glBindVertexArray(0);
  }

  // Desactivem el VAO
  glBindVertexArray(0);

}

void MyGLWidget::resizeGL(int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_S:
    if (scl < 2.0f) scl += 0.1f;
    break;
  case Qt::Key_D: 
    if (scl > 0.1f) scl -= 0.1f;
    break;
  case Qt::Key_E:
    angle += float(M_PI)/180;
    break;
  case Qt::Key_R: 
    angle -= float(M_PI)/180;
    break;
  case Qt::Key_C:
    menjat = !menjat;
    break;
  default:
    event->ignore();
    break;
  }
  update();
}

void MyGLWidget::creaBuffersQuadrat()
{
  glm::vec3 Vertices[6]; // vèrtexs amb X, Y i Z

  // minX = -0.5
  // maxX = 0.5
  // minY = -0.5
  // maxY = 0.5

  Vertices[0] = glm::vec3(-0.5, -0.5, 0);
  Vertices[1] = glm::vec3(0.5, -0.5, 0);
  Vertices[2] = glm::vec3(0.5, 0.5, 0);
  Vertices[3] = glm::vec3(0.5, 0.5, 0);
  Vertices[4] = glm::vec3(-0.5, 0.5, 0);
  Vertices[5] = glm::vec3(-0.5, -0.5, 0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOQuadrat);
  glBindVertexArray(VAOQuadrat);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCavall()
{
  // minX = 0.1
  // maxX = 2.6
  // minY = 0.1
  // maxY = 1.6

  glm::vec3 Vertices[45]; // vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(0.1, 0.3, 0.0);
  Vertices[1] = glm::vec3(0.1, 1.6, 0.0);
  Vertices[2] = glm::vec3(0.3, 0.7, 0.0);
  Vertices[3] = glm::vec3(0.3, 0.7, 0.0);
  Vertices[4] = glm::vec3(0.1, 1.6, 0.0);
  Vertices[5] = glm::vec3(0.3, 1.2, 0.0);
  Vertices[6] = glm::vec3(0.8, 0.4, 0.0);
  Vertices[7] = glm::vec3(0.3, 0.7, 0.0);
  Vertices[8] = glm::vec3(0.8, 0.7, 0.0);
  Vertices[9] = glm::vec3(0.8, 0.7, 0.0);
  Vertices[10] = glm::vec3(0.3, 0.7, 0.0);
  Vertices[11] = glm::vec3(0.8, 1.2, 0.0);
  Vertices[12] = glm::vec3(0.8, 1.2, 0.0);
  Vertices[13] = glm::vec3(0.3, 0.7, 0.0);
  Vertices[14] = glm::vec3(0.3, 1.2, 0.0);
  Vertices[15] = glm::vec3(0.8, 1.2, 0.0);
  Vertices[16] = glm::vec3(0.3, 1.2, 0.0);
  Vertices[17] = glm::vec3(0.8, 1.5, 0.0);
  Vertices[18] = glm::vec3(1.2, 0.7, 0.0);
  Vertices[19] = glm::vec3(0.8, 0.4, 0.0);
  Vertices[20] = glm::vec3(1.3, 1.7, 0.0);
  Vertices[21] = glm::vec3(1.3, 1.7, 0.0);
  Vertices[22] = glm::vec3(0.8, 0.4, 0.0);
  Vertices[23] = glm::vec3(0.8, 1.5, 0.0);
  Vertices[24] = glm::vec3(1.2, 0.7, 0.0);
  Vertices[25] = glm::vec3(1.3, 1.7, 0.0);
  Vertices[26] = glm::vec3(1.6, 1.0, 0.0);
  Vertices[27] = glm::vec3(1.6, 1.0, 0.0);
  Vertices[28] = glm::vec3(1.3, 1.7, 0.0);
  Vertices[29] = glm::vec3(2.0, 1.7, 0.0);
  Vertices[30] = glm::vec3(1.6, 1.0, 0.0);
  Vertices[31] = glm::vec3(2.0, 1.7, 0.0);
  Vertices[32] = glm::vec3(2.2, 1.0, 0.0);
  Vertices[33] = glm::vec3(1.6, 1.0, 0.0);
  Vertices[34] = glm::vec3(2.2, 1.0, 0.0);
  Vertices[35] = glm::vec3(2.2, 0.7, 0.0);
  Vertices[36] = glm::vec3(2.2, 0.7, 0.0);
  Vertices[37] = glm::vec3(2.2, 1.0, 0.0);
  Vertices[38] = glm::vec3(2.6, 0.6, 0.0);
  Vertices[39] = glm::vec3(1.6, 1.0, 0.0);
  Vertices[40] = glm::vec3(2.2, 0.7, 0.0);
  Vertices[41] = glm::vec3(1.4, 0.1, 0.0);
  Vertices[42] = glm::vec3(1.4, 0.1, 0.0);
  Vertices[43] = glm::vec3(1.3, 0.4, 0.0);
  Vertices[44] = glm::vec3(1.6, 0.6, 0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar el cavall
  glGenVertexArrays(1, &VAOCavall);
  glBindVertexArray(VAOCavall);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs(QOpenGLShader::Fragment, this);
  QOpenGLShader vs(QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation(program->programId(), "vertex");

  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG");
  colorLoc = glGetUniformLocation(program->programId(), "color");
}
