// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

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
void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
  iniCamera ();
  anglePatricio = 0.0f;
  patricioActive = false;
  focusGrocActive = false;
  obs = glm::vec3(0,2*radiEsc,0);
  vrp = glm::vec3(0,0,0);
  up = glm::vec3(1,0,0);
  l = -radiEsc;
  r = radiEsc;
  b = -radiEsc;
  t = radiEsc;
  angleHorari = 0;

}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  //ExamGLWidget::paintGL();
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  if (patricioActive) {
      // Pintem el Patricio
      glBindVertexArray (VAO_Patr);
      modelTransformPatricio ();
      glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  else {
      // Pintem el cub 1
      glBindVertexArray(VAO_Cub);
      modelTransformCub (4.0, 0.0);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // Pintem el cub 2
      glBindVertexArray(VAO_Cub);
      modelTransformCub (5.0, 2*float(M_PI)/3.0f);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // Pintem el cub 3
      glBindVertexArray(VAO_Cub);
      modelTransformCub (6.0, 2*2*float(M_PI)/3.0f);
      glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  //ExamGLWidget::modelTransformCub (1.0, 0.0);
  // En aquest mètode has de substituir aquest codi per construir la 
  // transformació geomètrica (TG) del cub usant els paràmetres adientment
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG,angleHorari+angle,glm::vec3(0,1,0));
  TG = glm::translate(TG,glm::vec3(5,0,0));
  TG = glm::scale(TG,glm::vec3(escala));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  //ExamGLWidget::modelTransformPatricio ();
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG,angleHorari+anglePatricio,glm::vec3(0,1,0));
  TG = glm::translate(TG,glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3 (2*escala));
  TG = glm::rotate(TG,-float(M_PI)/2.0f,glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBasePat);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    // Codi per a la viewMatrix de la Càmera-2
      View = lookAt (obs,vrp,up);
      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    // Codi per a la projectMatrix de la Càmera-2
    glm::mat4 Proj = glm::ortho(l,r,b,t,radiEsc,3.0f*radiEsc);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      patricioActive = !patricioActive;
    break;
	}
  case Qt::Key_1: {
      anglePatricio = 0.0f;
    break;
	}
  case Qt::Key_2: {
      anglePatricio = 2.0f*float(M_PI)/3.0f;
    break;
	}
  case Qt::Key_3: {
      anglePatricio = 2.0f*2.0f*float(M_PI)/3.0f;
    break;
	}
  case Qt::Key_F: {
     focusGrocActive = !focusGrocActive;
     if (focusGrocActive) {
         colFoc = glm::vec3(1,1,0);
         enviaColFocus();
     }
     else {
         colFoc = glm::vec3(1,1,1);
         enviaColFocus();
     }
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      if (camPlanta) emit ort();
      else emit per();
      projectTransform ();
      viewTransform ();
    break;
	}
  case Qt::Key_Right: {
      angleHorari += 2.0f*float(M_PI)/3.0f;
    break;
	}
  case Qt::Key_Left: {
      angleHorari -= 2.0f*float(M_PI)/3.0f;
    break;
	}
  case Qt::Key_R: {
      patricioActive = false;
      anglePatricio = 0.0f;
      angleHorari = 0.0f;
      camPlanta = false;
      colFoc = glm::vec3(1,1,1);
      enviaColFocus();
      emit per();
      emit resetCub(1);
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::setCamaraOrto() {
    makeCurrent();
    camPlanta = true;
    projectTransform ();
    viewTransform ();
    update();
}
void MyGLWidget::setCamaraPers() {
    makeCurrent();
    camPlanta = false;
    projectTransform ();
    viewTransform ();
    update();
}
void MyGLWidget::setCubAssociat(int idCub) {
    makeCurrent();
    if (idCub == 1) {
        anglePatricio = 0.0f;
    }
    else if (idCub == 2) {
        anglePatricio = 2.0f*float(M_PI)/3.0f;
    }
    else if (idCub == 3) {
        anglePatricio = 2.0f*2.0f*float(M_PI)/3.0f;
    }
    update();
}

