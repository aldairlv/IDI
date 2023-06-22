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
  // create a timer
  timer = new QTimer(this);
  efectoDiscoActiu = false;

  // setup signal and slot
  connect(timer, SIGNAL(timeout()),this, SLOT(MyTimerSlot()));
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  focusEscenaActiva = true;
  
  angleDiscoBall = 0.0f;
  angleMorty = 0.0f;
  angleFantasma = 0.0f;
  
 
  
  carregaShaders();
  
  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersDiscoBall();
  creaBuffersTerraIParets();

  iniEscena();
  iniCamera();
  
  posFocuse = glm::vec3(5.0,10.0,5.0);
  colorFocuse = glm::vec3(0.4,0.4,0.4);
  glUniform3fv(posFocusLocE, 1, &posFocuse[0]);
  glUniform3fv(colorFocusLocE, 1, &colorFocuse[0]);
  
  posFocusID0 = glm::vec3(1.2,0.0,0.0);
  colorFocusID0 = glm::vec3(0.4,0.0,0.0);
  glUniform3fv(posFocusID0Loc, 1, &posFocusID0[0]);
  glUniform3fv(colorFocusID0Loc, 1, &colorFocusID0[0]);
  
  posFocusID1 = glm::vec3(-1.2,0.0,0.0);
  colorFocusID1 = glm::vec3(0.0,0.4,0.0);
  glUniform3fv(posFocusID1Loc, 1, &posFocusID1[0]);
  glUniform3fv(colorFocusID1Loc, 1, &colorFocusID1[0]);
  
  posFocusID2 = glm::vec3(0.0,0.0,1.2);
  colorFocusID2 = glm::vec3(0.0,0.0,0.4);
  glUniform3fv(posFocusID2Loc, 1, &posFocusID2[0]);
  glUniform3fv(colorFocusID2Loc, 1, &colorFocusID2[0]);
  
  posFocusID3 = glm::vec3(0.0,0.0,-1.2);
  colorFocusID3 = glm::vec3(0.4,0.4,0.0);
  glUniform3fv(posFocusID3Loc, 1, &posFocusID3[0]);
  glUniform3fv(colorFocusID3Loc, 1, &colorFocusID3[0]);
  numFocus = 0;
}
void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  modelTransformFocus();
  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA 1
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  
  // FANTASMA 2
  modelTransformFantasma (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  
  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      angleMorty -= float(M_PI)/4.0f;
      angleFantasma -= float(M_PI)/4.0f;
    break;
	}
  case Qt::Key_D: {
      angleMorty += float(M_PI)/4.0f;
      angleFantasma += float(M_PI)/4.0f;
    break;
	}
  case Qt::Key_E: {
      focusEscenaActiva = !focusEscenaActiva;
      if (focusEscenaActiva) {
          colorFocuse = glm::vec3(0.4,0.4,0.4);
          glUniform3fv(colorFocusLocE, 1, &colorFocuse[0]);
      }
      else {
          colorFocuse = glm::vec3(0.0,0.0,0.0);
          glUniform3fv(colorFocusLocE, 1, &colorFocuse[0]);
      }
    break;
	}
  case Qt::Key_B: {
      // 0 totes les llums encendidas
      numFocus++;
      numFocus = numFocus%8;
     
      if (numFocus == 1 or numFocus == 5) {
          if (colorFocusID1 != glm::vec3(0.0)) colorFocusID1 = glm::vec3(0.0,0.0,0.0);
          else colorFocusID1 = glm::vec3(0.0,0.4,0.0);
          glUniform3fv(colorFocusID1Loc, 1, &colorFocusID1[0]);
      }
      else if (numFocus == 2 or numFocus == 6) {
          if (colorFocusID2 != glm::vec3(0.0)) colorFocusID2 = glm::vec3(0.0,0.0,0.0);
          else colorFocusID2 = glm::vec3(0.0,0.0,0.4);
          glUniform3fv(colorFocusID2Loc, 1, &colorFocusID2[0]);
      }
      else if (numFocus == 3 or numFocus == 7) {
          if (colorFocusID3 != glm::vec3(0.0)) colorFocusID3 = glm::vec3(0.0,0.0,0.0);
          else colorFocusID3 = glm::vec3(0.4,0.4,0.0);
          glUniform3fv(colorFocusID3Loc, 1, &colorFocusID3[0]);
      }
      else if (numFocus == 4) {
          colorFocusID0 = glm::vec3(0.0,0.0,0.0);
          glUniform3fv(colorFocusID0Loc, 1, &colorFocusID0[0]);
      }
      else if (numFocus == 0) {
          colorFocusID0 = glm::vec3(0.4,0.0,0.0);
          glUniform3fv(colorFocusID0Loc, 1, &colorFocusID0[0]);
      }
    break;
	}	
  case Qt::Key_R: {
        angleDiscoBall += (float(M_PI)*5.0f)/180.0f;
    break;
	}
  case Qt::Key_S: {
      efectoDiscoActiu = !efectoDiscoActiu;
      if(efectoDiscoActiu) timer->start(100);
      else timer->stop();
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}



void MyGLWidget::modelTransformDiscoBall ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::rotate(TG, angleDiscoBall, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::translate(TG, -centreBaseDiscoBall);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformMorty ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::rotate(TG, angleMorty, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float posX)
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::rotate(TG, angleFantasma, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  //glUniformMatrix4fv (transFocusLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFocus() {
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(5.0, 5.75, 5.0));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::rotate(TG, angleDiscoBall, glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv (transFocusLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("./shaders/basicLlumShader.frag");
  vs.compileSourceFile("./shaders/basicLlumShader.vert");
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
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "Proj");
  viewLoc = glGetUniformLocation (program->programId(), "View");
  colorFocusLocE = glGetUniformLocation(program->programId(), "colorFocusE");
  posFocusLocE = glGetUniformLocation(program->programId(), "posFocusE");
  
  colorFocusID0Loc = glGetUniformLocation(program->programId(), "colorFocusID0");
  posFocusID0Loc = glGetUniformLocation(program->programId(), "posFocusID0");
  
  colorFocusID1Loc = glGetUniformLocation(program->programId(), "colorFocusID1");
  posFocusID1Loc = glGetUniformLocation(program->programId(), "posFocusID1");
  
  colorFocusID2Loc = glGetUniformLocation(program->programId(), "colorFocusID2");
  posFocusID2Loc = glGetUniformLocation(program->programId(), "posFocusID2");
  
  colorFocusID3Loc = glGetUniformLocation(program->programId(), "colorFocusID3");
  posFocusID3Loc = glGetUniformLocation(program->programId(), "posFocusID3");
  
  transFocusLoc = glGetUniformLocation (program->programId(), "TGfocus");
}

void MyGLWidget::MyTimerSlot()
{
    makeCurrent();
    angleDiscoBall += (float(M_PI)*5.0f)/180.0f;
    numFocus++;
    numFocus = numFocus%8;
    if (numFocus == 1 or numFocus == 5) {
        if (colorFocusID1 != glm::vec3(0.0)) colorFocusID1 = glm::vec3(0.0,0.0,0.0);
        else colorFocusID1 = glm::vec3(0.0,0.4,0.0);
        glUniform3fv(colorFocusID1Loc, 1, &colorFocusID1[0]);
    }
    else if (numFocus == 2 or numFocus == 6) {
        if (colorFocusID2 != glm::vec3(0.0)) colorFocusID2 = glm::vec3(0.0,0.0,0.0);
        else colorFocusID2 = glm::vec3(0.0,0.0,0.4);
        glUniform3fv(colorFocusID2Loc, 1, &colorFocusID2[0]);
    }
    else if (numFocus == 3 or numFocus == 7) {
        if (colorFocusID3 != glm::vec3(0.0)) colorFocusID3 = glm::vec3(0.0,0.0,0.0);
        else colorFocusID3 = glm::vec3(0.4,0.4,0.0);
        glUniform3fv(colorFocusID3Loc, 1, &colorFocusID3[0]);
    }
    else if (numFocus == 4) {
        colorFocusID0 = glm::vec3(0.0,0.0,0.0);
        glUniform3fv(colorFocusID0Loc, 1, &colorFocusID0[0]);
    }
    else if (numFocus == 0) {
        colorFocusID0 = glm::vec3(0.4,0.0,0.0);
        glUniform3fv(colorFocusID0Loc, 1, &colorFocusID0[0]);
    }
    update();
}

