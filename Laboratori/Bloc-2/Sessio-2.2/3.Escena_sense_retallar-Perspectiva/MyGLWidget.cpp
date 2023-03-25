// MyGLWidget.cpp
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

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL(){
    BL2GLWidget::initializeGL();
    glEnable(GL_DEPTH_TEST);
    creaBuffersModel();
    creaBuffersTerra(); 
    puntmin = glm::vec3 (-2,-1,-2);
    puntmax = glm::vec3 (2,1,2);
    centreRadi(puntmin,puntmax);
    ini_camera();
    projectTransform();
    viewTransform();
    angle = 0.0f;
   
}
void MyGLWidget::carregaShaders(){
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(),"proj");
    viewLoc = glGetUniformLocation(program->programId(),"view");
}

void MyGLWidget::projectTransform(){
    glm::mat4 Proj = glm::perspective (fov,raw,znear,zfar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}
void MyGLWidget::viewTransform(){
    glm::mat4 View = glm::lookAt(obs,vrp,up);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&View[0][0]);
}

void MyGLWidget::ini_camera(){
    fovIni = 2*asin(radiEsfera/(2*radiEsfera));
    fov = fovIni;
    raw = 1.0f;
    znear = radiEsfera;
    zfar = 3.0f*radiEsfera;
    obs = glm::vec3(0,0,2*radiEsfera);
    vrp = glm::vec3(0,0,0);
     up = glm::vec3(0,1,0);
}

void MyGLWidget::creaBuffersModel(){
    m.load("../../models/HomerProves.obj");
    glGenVertexArrays(1,&VAO_model);
    glBindVertexArray(VAO_model);

    GLuint VBO_model[2];
    glGenBuffers(2,VBO_model);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_model[1]);
     glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::paintGL () {
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
   
   
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform ();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_model);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  
  modelTransformTerra();
  glBindVertexArray(VAO_terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      escala += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      escala -= 0.05;
      break;
    }
    case Qt::Key_R: { 
      angle += float(M_PI)/4.0f;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform,angle,glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(escala));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(escala));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::creaBuffersTerra () 
{
  // Dades del terra
  // Dos VBOs, un amb posició i l'altre amb color
  glm::vec3 posicio[6] = {
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, 2.0)
  }; 
  glm::vec3 color[6] = {
	glm::vec3(1,0,0),
	glm::vec3(0,0,1),
	glm::vec3(0,1,0),
	
	glm::vec3(1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,0,1),
  };

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_terra);
  glBindVertexArray(VAO_terra);

  GLuint VBO_terra[2];
  glGenBuffers(2, VBO_terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}


void MyGLWidget::centreRadi(glm::vec3 pmin, glm::vec3 pmax){
	centreEscena[0] = (pmax.x+pmin.x)/2.0f;
	centreEscena[1] = (pmax.y+pmin.y)/2.0f;
	centreEscena[2] = (pmax.z+pmin.z)/2.0f;

        radiEsfera = (distance(pmin,pmax))/2.0f;
        //cout << "centreEscena = (" << centreEscena[0] << "," << centreEscena[1] << "," << centreEscena[2] << ")" << endl;
        //cout << "radiEsfera = " << radiEsfera << endl;
}

void MyGLWidget::resizeGL (int w, int h) {
    BL2GLWidget::resizeGL(w,h) ;
    rav = float(w)/float(h);
    fov = fovIni;
    raw = rav;
    if (rav < 1.0f) fov = 2.0f*atan(tan(fovIni/2.0f)/rav);
    projectTransform();
}

