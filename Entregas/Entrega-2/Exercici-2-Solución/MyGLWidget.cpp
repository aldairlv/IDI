#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
using namespace std;
#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersModels();
  creaBuffersTerra();
  iniEscena();
  iniCamera();
  angleRick = float(M_PI);
  angleCameraX = float(M_PI)/4.0f;
  angleCameraY = float(M_PI)/4.0f;
  angleMortyY = 0.0f;
  camera2 = false;
  invisible = false;
}

void MyGLWidget::iniEscena()
{
  glm::vec3 puntmin (-4,0,-4);
  glm::vec3 puntmax (4,3,4);
  
  radiEscena = distance(puntmin,puntmax)/2.0f;
  centreEscena = (puntmin + puntmax)/2.0f;
}

void MyGLWidget::iniCamera()
{
  vrp = centreEscena;
  obs = vrp + glm::vec3(0,0,1)*(2*radiEscena);
  up = glm::vec3(0, 1, 0);
  
  fovIni = 2*asin(radiEscena/(2*radiEscena));
  fov = fovIni;
  znear = radiEscena;
  zfar = 3*radiEscena;
  angleX = float(M_PI)/4.0f;
  angleY = 0.0f;
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);
   
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   viewTransform();
   projectTransform();
   glm::vec3 colGreen(1.0f, 1.0f, 1.0f);
   glUniform3fv (greenLoc, 1, &colGreen[0]);
  
  // Rick
   if (invisible) {
  	colGreen = glm::vec3(0.0f, 0.7f, 0.0f);
  	glUniform3fv (greenLoc, 1, &colGreen[0]);
  	glUniform1i(invisibleLoc,true);
  }
  glBindVertexArray (VAO_models[RICK]);
  RickTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);
  colGreen = glm::vec3(1.0f, 1.0f, 1.0f);
  glUniform3fv (greenLoc, 1, &colGreen[0]);
  glUniform1i(invisibleLoc,false);
  
  // Morty
  glBindVertexArray (VAO_models[MORTY]);
  MortyTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
  
  
  // VideoCamera
  glBindVertexArray (VAO_models[VIDEOCAMERA]);
  VideoCameraTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);  
  
  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  // Paret
  ParetTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  if (camera2) {
   	  glViewport(ample-(ample/4),alt-(alt/4),ample/4,alt/4);
   	  viewTransform2();
   	  projectTransform2();
   	  colGreen = glm::vec3(0.0f, 1.0f, 0.0f);
          glUniform3fv (greenLoc, 1, &colGreen[0]);
   	  // Rick
   	  if (invisible) {
  	  	colGreen = glm::vec3(0.0f, 0.7f, 0.0f);
  		glUniform3fv (greenLoc, 1, &colGreen[0]);
  		glUniform1i(invisibleLoc,true);
  	  }
	  glBindVertexArray (VAO_models[RICK]);
	  RickTransform();
	  glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);
	  colGreen = glm::vec3(0.0f, 1.0f, 0.0f);
  	  glUniform3fv (greenLoc, 1, &colGreen[0]);
  	  glUniform1i(invisibleLoc,false);

	  // Morty
	  glBindVertexArray (VAO_models[MORTY]);
	  MortyTransform();
	  glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
	  
	  // Terra
	  glBindVertexArray (VAO_Terra);
	  identTransform();
	  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	  // Paret
	  ParetTransform();
	  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   }
  glBindVertexArray (0);
}

void MyGLWidget::RickTransform ()
{
  glm::mat4 TG(1.0f);  
  TG = glm::translate(TG, glm::vec3(-2, 0, -2));
  TG = glm::rotate(TG,angleRick,glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
  TG = glm::translate(TG, -centreBaseModels[RICK]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posMorty);
  TG = glm::rotate(TG,angleMortyY,glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::VideoCameraTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-4, 3, -4));
  TG = glm::rotate(TG,angleCameraY,glm::vec3(0,1,0));
  TG = glm::rotate(TG,angleCameraX,glm::vec3(1,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[VIDEOCAMERA]));
  TG = glm::translate(TG, -centreBaseModels[VIDEOCAMERA]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  View = glm::translate(View, glm::vec3(0, 0, -2*radiEscena));
  View = glm::rotate(View,angleX,glm::vec3(1,0,0));
  View = glm::rotate(View,angleY,glm::vec3(0,1,0));
  View = glm::translate(View, -centreEscena);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::viewTransform2 ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  glm::vec3 obs2 = glm::vec3(-4, 3, -4);
  View = glm::lookAt (obs2, obs2+glm::vec3(sin(angleCameraY),tan(-angleCameraX),cos(angleCameraY)), up); 
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::projectTransform2 ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (float(M_PI)/3.0f, ra, 0.25f, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE))
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (yClick - e->y()) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: {     
    	posMorty = posMorty + glm::vec3(sin(angleMortyY),0.0,cos(angleMortyY))*glm::vec3(0.1);	
      break;
    }
    case Qt::Key_Down: {
	posMorty = posMorty -  glm::vec3(sin(angleMortyY),0.0,cos(angleMortyY))*glm::vec3(0.1);
      break;
    }
    case Qt::Key_Q: {
	if (angleCameraY < float(M_PI)/2.0f) angleCameraY += float(M_PI)/180.0f;
      break;
    }
    case Qt::Key_E: {
	if (angleCameraY > 0.0f) angleCameraY -= float(M_PI)/180.0f;
      break;
    }
     case Qt::Key_Left: { 
    	angleMortyY += float(M_PI)/4.0f;	
      break;
    }
     case Qt::Key_Right: { 
       angleMortyY -= float(M_PI)/4.0f;  	
      break;
    }
    case Qt::Key_C: { 
       camera2 = !camera2;	
      break;
    }
    case Qt::Key_G: { 
       invisible = !invisible;	
      break;
    }
    case Qt::Key_R: {
    	iniCamera();
  	angleRick = float(M_PI);
  	angleCameraX = float(M_PI)/4.0f;
  	angleCameraY = float(M_PI)/4.0f;
  	angleMortyY = 0.0f;
  	camera2 = false;
  	invisible = false;
  	posMorty = glm::vec3(1,0,0);
       break;
    }
    default: event->ignore(); break;
  }
  update();
}


void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
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

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");
  greenLoc  = glGetUniformLocation (program->programId(), "colGreen");
  invisibleLoc  = glGetUniformLocation (program->programId(), "invisible");
}
void MyGLWidget::resizeGL (int w, int h) 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  ra = float(ample)/float(alt);
  factorAngleY = M_PI / ample;
  factorAngleX = M_PI / alt;
  projectTransform();
  
  fov = fovIni;
  if (ra < 1.0f) fov = 2.0f*atan(tan(fovIni/2.0f)/ra);
}
