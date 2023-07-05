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
  //ExamGLWidget::initializeGL();

  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();


  // ------------------------
  //   VARIABLES DE Control
  //-------------------------
  camPlanta = false;
  angleCoin = (2.0f*float(M_PI))/8.0f;
  angleMortyY = 0.0f;
  posMorty = glm::vec3(2,0,0);

  posFx = 7.0f;
  posFy = 0.0f;
  posFz = 0.0f;
  posFantasma = glm::vec3(posFx,posFy,posFz);

  move = 0.2f;

  xval = 0.0;
  yval = 0.0;
  zval = 0.0;

  xval2 = 0.0;
  yval2 = 0.0;
  zval2 = 0.0;

  menjat = false;
  posMushroom = glm::vec3(14.0f,0.0f,0.0f);


  rotacioCoin = 0.0f;

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()),this, SLOT(MyTimerSlot()));

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
  iniCamera ();


}

void MyGLWidget::iniEscena ()
{
  //ExamGLWidget::iniEscena();

  creaBuffersBrick();
  creaBuffersGreenPipe ();
  creaBuffersFantasma ();
  creaBuffersGroundBlocks3x3 ();
  creaBuffersLuckyBox ();
  creaBuffersMorty ();
  creaBuffersCoin ();
  creaBuffersMushroom ();

  // Paràmetres de l'escena -  ben calculats
  puntmin = glm::vec3(-1.5,-2.0,-4.5);
  puntmax = glm::vec3(16.5,4.0,4.5);

  radiEsc = (distance(puntmax,puntmin)/2.0);
  centreEsc = (puntmax + puntmin);
  centreEsc = glm::vec3(centreEsc[0]/2.0,centreEsc[1]/2.0,centreEsc[2]/2.0);


  timer->start(100);

}

void MyGLWidget::enviaColFocus()
{
  ExamGLWidget::enviaColFocus();
  glUniform3fv (colFocusCamaraLoc, 1, &colFocOn[0]);
  glUniform3fv (colFocusEscena1Loc, 1, &colFocOff[0]);
  glUniform3fv (colFocusEscena2Loc, 1, &colFocOff[0]);

  glUniform3fv (posFocusCamaraLoc, 1, &posFocusCam[0]);
  glUniform3fv (posFocusEscena1Loc, 1, &posFocusEsc1[0]);
  glUniform3fv (posFocusEscena2Loc, 1, &posFocusEsc2[0]);


}

void MyGLWidget::iniCamera ()
{
  //ExamGLWidget::iniCamera();

  //camPlanta = false;

  angleY = 0.0f;
  angleX = 0.0f;

  ra = 1.0f;

  fovIni = 2.0f*asin(radiEsc/(2.0f*radiEsc));
  fov = fovIni;
  zn = radiEsc;
  zf = 3*radiEsc;
  vrp = centreEsc;
  obs = centreEsc + 2*radiEsc*glm::vec3(0,1,0);

  up = glm::vec3(0,0,-1);
  l = radiEsc;
  r = radiEsc;
  b = radiEsc;
  t = radiEsc;

  projectTransform ();
  viewTransform ();

  colFocOn = glm::vec3(1.0f);
  colFocOff = glm::vec3(0.0f);

  posFocusCam = glm::vec3(0.0f,0.0f,0.0f);
  posFocusEsc1 = glm::vec3(7.5f,3.5f,0.0f);
  posFocusEsc2 = glm::vec3(7.5f,6.0f,0.0f);

  enviaColFocus();

}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  //ExamGLWidget::paintGL();
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);,i9

  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem un GroundBlock3x3
  glBindVertexArray(VAO_GroundBlocks3x3);
  for (int k = 0; k < 2; ++k) {
      for (int i = 0; i < 6; ++i) {
          for (int j = 0; j < 3; ++j) {
              modelTransformGroundBlocks3x3(glm::vec3(3*i,k,3*j));
              glDrawArrays(GL_TRIANGLES,0,GroundBlocks3x3.faces().size()*3);
          }
      }
  }

  // Pintem 8 Coins
  glBindVertexArray(VAO_Coin);
  modelTransformCoin(0*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  modelTransformCoin(1*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  modelTransformCoin(2*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  modelTransformCoin(3*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  modelTransformCoin(4*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  modelTransformCoin(5*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  modelTransformCoin(6*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  modelTransformCoin(7*angleCoin);
  glDrawArrays(GL_TRIANGLES,0,Coin.faces().size()*3);

  // Pintem GreenPipe
  glBindVertexArray(VAO_GreenPipe);
  modelTransformGreenPipe();
  glDrawArrays(GL_TRIANGLES,0,GreenPipe.faces().size()*3);

  // Pintem Brick
  glBindVertexArray(VAO_Brick);
  modelTransformBrick();
  glDrawArrays(GL_TRIANGLES,0,Brick.faces().size()*3);

  // Pintem Fantasma
  glBindVertexArray(VAO_Fantasma);
  modelTransformFantasma();
  glDrawArrays(GL_TRIANGLES,0,Fantasma.faces().size()*3);

  // Pintem LuckyBox
  glBindVertexArray(VAO_LuckyBox);
  modelTransformLuckyBox();
  glDrawArrays(GL_TRIANGLES,0,LuckyBox.faces().size()*3);

  // Pintem Morty
  glBindVertexArray(VAO_Morty);
  modelTransformMorty();
  glDrawArrays(GL_TRIANGLES,0,Morty.faces().size()*3);

  // Pintem Mushroom
  if (not menjat)
  {
      glBindVertexArray(VAO_Mushroom);
      modelTransformMushroom();
      glDrawArrays(GL_TRIANGLES,0,Mushroom.faces().size()*3);
  }
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
#ifdef __APPLE__
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif

  ra = float(ample)/float(alt);
  projectTransform();
}



//---------------------------------------------------
//                   MODEL TRANSFORMS
//---------------------------------------------------
void MyGLWidget::modelTransformBrick ()
{
  //ExamGLWidget::modelTransformBrick();

  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(5,3,0));
  TG = glm::scale(TG, glm::vec3 (escalaBrick));
  TG = glm::translate(TG, -centreBaseBrick);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformGreenPipe ()
{
  //ExamGLWidget::modelTransformGreenPipe();

  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(0,0,-3));
  TG = glm::scale(TG, glm::vec3 (2*escalaGreenPipe));
  TG = glm::translate(TG, -centreBaseGreenPipe);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformFantasma ()
{
  //ExamGLWidget::modelTransformFantasma();

  TG = glm::mat4(1.f);
  TG = glm::translate(TG, posFantasma);
  TG = glm::scale(TG, glm::vec3 (escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformLuckyBox ()
{
  //ExamGLWidget::modelTransformLuckyBox();

  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(6,3,0));
  TG = glm::scale(TG, glm::vec3 (escalaLuckyBox));
  TG = glm::translate(TG, -centreBaseLuckyBox);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformMorty ()
{
  //ExamGLWidget::modelTransformMorty();

  TG = glm::mat4(1.f);
  TG = glm::translate(TG, posMorty);
  TG = glm::rotate(TG,angleMortyY,glm::vec3(0,1,0));
  TG = glm::rotate(TG,float(M_PI)/2.0f,glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (escalaMorty));
  TG = glm::translate(TG, -centreBaseMorty);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformCoin (float angleCoin)
{
  //ExamGLWidget::modelTransformCoin (angleCoin);

  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(14,3,0));
  TG = glm::rotate(TG,angleCoin,glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(2,0,0));
  TG = glm::rotate(TG,-angleCoin,glm::vec3(0,1,0));
  TG = glm::rotate(TG,rotacioCoin,glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (0.5*escalaCoin));
  TG = glm::translate(TG, -centreBaseCoin);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformMushroom ()
{
  //ExamGLWidget::modelTransformMushroom();

  TG = glm::mat4(1.f);
  TG = glm::translate(TG, posMushroom);
  TG = glm::scale(TG, glm::vec3 (escalaMushroom));
  TG = glm::translate(TG, -centreBaseMushroom);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}



//--------------------------------------------
//        PROJECT AND VIEW TRANSFORMS
//--------------------------------------------
void MyGLWidget::projectTransform ()// OPTICA DE LA CAMARA
{
  // ExamGLWidget::projectTransform();
  ra = float(ample)/float(alt);
  if (!camPlanta) {
      fov = fovIni;
      if (ra < 1.0f) fov = 2.0f*atan(tan(fovIni/2.0f)/ra);
      glm::mat4 Proj;  // Matriu de projecció
      Proj = glm::perspective(fov, ra, zn, zf);
      glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  } else {
      // Codi per a la projectMatrix de la Càmera-2
       l = r = b = t = 9;
       if (ra > 1.0f) {
            l = r = 9*ra;
        }
        else if (ra < 1.0f) {
            b = t = 9/ra;
        }
      glm::mat4 Proj;  // Matriu de projecció
      Proj = glm::ortho(-l,r,-b,t,radiEsc,3*radiEsc);
      glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::viewTransform ()// POSICION DE LA CAMARA
{
  // ExamGLWidget::viewTransform();
  if (!camPlanta) {
      View = glm::mat4(1.f);
      View = glm::translate(View, glm::vec3(0, 0, -2.0f*radiEsc));
      View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
      View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
      View = glm::translate(View, -centreEsc);
      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);

  } else {
      View = lookAt(obs,vrp,up);
      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}



//-------------------------------------------------
//             MOUSE OR KEY EVENTS
//-------------------------------------------------
void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camPlanta)
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
  // ExamGLWidget::keyPressEvent(event);
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_R: { // RESET
      camPlanta = false;
      iniCamera();
      angleMortyY = 0.0f;
      posMorty = glm::vec3(2,0,0);
      posFx = 7.0f;
      posFy = 0.0f;
      posFz = 0.0f;

      posFantasma = glm::vec3(posFx,posFy,posFz);
      menjat = false;
      emit per();
      emit eng();

      break;
    }
    case Qt::Key_C: { // CHANGE CAM
      camPlanta = !camPlanta;
      if(camPlanta) {
          glUniform3fv (colFocusCamaraLoc, 1, &colFocOff[0]);
          glUniform3fv (colFocusEscena1Loc, 1, &colFocOn[0]);
          glUniform3fv (colFocusEscena2Loc, 1, &colFocOn[0]);
          emit ort();
      }
      else {
          glUniform3fv (colFocusCamaraLoc, 1, &colFocOn[0]);
          glUniform3fv (colFocusEscena1Loc, 1, &colFocOff[0]);
          glUniform3fv (colFocusEscena2Loc, 1, &colFocOff[0]);
          emit per();
      }
      projectTransform();
      viewTransform();
      break;
    }
    case Qt::Key_Up: {
      angleMortyY = -(float(M_PI) + float(M_PI)/2.0f);
      posMorty = posMorty + glm::vec3(0.0,0.0,-move);

      glm::vec3 dist = posMorty - posFantasma;
      float total = abs(dist[0]) + abs(dist[2]);
      float pesoX = 0.0f;
      float pesoZ = 0.0f;


      if(total != 0.0f) pesoX = (abs(dist[0])/total);
      pesoZ = 1.0f - pesoX;

      if (posMorty[0] > posFantasma[0]) posFx = posFx + 0.13f*pesoX;
      else if (posMorty[0] < posFantasma[0]) posFx = posFx - 0.13f*pesoX;

      if (posMorty[2] > posFantasma[2]) posFz = posFz + 0.13f*pesoZ;
      else if (posMorty[2] < posFantasma[2]) posFz = posFz - 0.13f*pesoZ;

      if(distance(posFantasma,posMorty)<= 0.5f){
            angleMortyY = 0.0f;
            posMorty = glm::vec3(2,0,0);

            posFx = 7.0f;
            posFy = 0.0f;
            posFz = 0.0f;

            posFantasma = glm::vec3(posFx,posFy,posFz);
            menjat = false;

      } else {
          posFantasma = glm::vec3(posFx,posFy,posFz);
      }
      if(distance(posMushroom,posMorty)<= 0.5f) menjat = true;

      break;
    }
    case Qt::Key_Down: {
      angleMortyY = -float(M_PI)/2.0f;
	  posMorty = posMorty + glm::vec3(0.0,0.0,move);

      glm::vec3 dist = posMorty - posFantasma;
      float total = abs(dist[0]) + abs(dist[2]);
      float pesoX = 0.0f;
      float pesoZ = 0.0f;

      if(total != 0.0f) pesoX = (abs(dist[0])/total);
      pesoZ = 1.0f - pesoX;

      if (posMorty[0] > posFantasma[0]) posFx = posFx + 0.13f*pesoX;
      else if (posMorty[0] < posFantasma[0]) posFx = posFx - 0.13f*pesoX;

      if (posMorty[2] > posFantasma[2]) posFz = posFz + 0.13f*pesoZ;
      else if (posMorty[2] < posFantasma[2]) posFz = posFz - 0.13f*pesoZ;

      if(distance(posFantasma,posMorty)<= 0.5f){
            angleMortyY = 0.0f;
            posMorty = glm::vec3(2,0,0);

            posFx = 7.0f;
            posFy = 0.0f;
            posFz = 0.0f;

            posFantasma = glm::vec3(posFx,posFy,posFz);
            menjat = false;
      } else {
          posFantasma = glm::vec3(posFx,posFy,posFz);
      }
      if(distance(posMushroom,posMorty)<= 0.5f) menjat = true;

      break;
    }
    case Qt::Key_Left: {
      angleMortyY = -float(M_PI);
      posMorty = posMorty + glm::vec3(-move,0.0,0.0);

      glm::vec3 dist = posMorty - posFantasma;
      float total = abs(dist[0]) + abs(dist[2]);
      float pesoX = 0.0f;
      float pesoZ = 0.0f;

      if(total != 0.0f) pesoX = (abs(dist[0])/total);
      pesoZ = 1.0f - pesoX;

      if (posMorty[0] > posFantasma[0]) posFx = posFx + 0.13f*pesoX;
      else if (posMorty[0] < posFantasma[0]) posFx = posFx - 0.13f*pesoX;

      if (posMorty[2] > posFantasma[2]) posFz = posFz + 0.13f*pesoZ;
      else if (posMorty[2] < posFantasma[2]) posFz = posFz - 0.13f*pesoZ;

      if(distance(posFantasma,posMorty)<= 0.5f){
            angleMortyY = 0.0f;
            posMorty = glm::vec3(2,0,0);

            posFx = 7.0f;
            posFy = 0.0f;
            posFz = 0.0f;

            posFantasma = glm::vec3(posFx,posFy,posFz);
            menjat = false;
      } else {
          posFantasma = glm::vec3(posFx,posFy,posFz);
      }
      if(distance(posMushroom,posMorty)<= 0.5f) menjat = true;

      break;
    }
    case Qt::Key_Right: {
      angleMortyY = 0.0f;
      posMorty = posMorty + glm::vec3(move,0.0,0.0);


      glm::vec3 dist = posMorty - posFantasma;
      float total = abs(dist[0]) + abs(dist[2]);
      float pesoX = 0.0f;
      float pesoZ = 0.0f;

      if(total != 0.0f) pesoX = (abs(dist[0])/total);
      pesoZ = 1.0f - pesoX;

      if (posMorty[0] > posFantasma[0]) posFx = posFx + 0.13f*pesoX;
      else if (posMorty[0] < posFantasma[0]) posFx = posFx - 0.13f*pesoX;

      if (posMorty[2] > posFantasma[2]) posFz = posFz + 0.13f*pesoZ;
      else if (posMorty[2] < posFantasma[2]) posFz = posFz - 0.13f*pesoZ;

      if(distance(posFantasma,posMorty)<= 0.5f){
            angleMortyY = 0.0f;
            posMorty = glm::vec3(2,0,0);

            posFx = 7.0f;
            posFy = 0.0f;
            posFz = 0.0f;

            posFantasma = glm::vec3(posFx,posFy,posFz);
            menjat = false;
      } else {
          posFantasma = glm::vec3(posFx,posFy,posFz);
      }

      if(distance(posMushroom,posMorty)<= 0.5f) menjat = true;

      break;
    }
    /*
   // CONTROL FOCUS DE CÀMERA
    case Qt::Key_1: { // Apaga el focus de càmera
      glUniform3fv (colFocusCamaraLoc, 1, &colFocOff[0]);
      break;
    }
    case Qt::Key_2: { // encén el focus de càmera
      glUniform3fv (colFocusCamaraLoc, 1, &colFocOn[0]);
      break;
    }

    // CONTROL FOCUS D'ESCENA 1 - LLUM ESPECULAR
    case Qt::Key_P: {
      posFocusEsc1 = posFocusEsc1 - glm::vec3(0.0,0.0,0.1);
      glUniform3fv(posFocusEscena1Loc, 1, &posFocusEsc1[0]);
      break;
    }
    case Qt::Key_O: {
      posFocusEsc1 = posFocusEsc1 + glm::vec3(0.0,0.0,0.1);
      glUniform3fv(posFocusEscena1Loc, 1, &posFocusEsc1[0]);
      break;
    }
     case Qt::Key_I: {
      posFocusEsc1 = posFocusEsc1 - glm::vec3(0.1,0.0,0.0);
      glUniform3fv(posFocusEscena1Loc, 1, &posFocusEsc1[0]);
      break;
    }
    case Qt::Key_U: {
      posFocusEsc1 = posFocusEsc1 + glm::vec3(0.1,0.0,0.0);
      glUniform3fv(posFocusEscena1Loc, 1, &posFocusEsc1[0]);
      break;
    }
    case Qt::Key_Y: {
      posFocusEsc1 = posFocusEsc1 - glm::vec3(0.0,0.1,0.0);
      glUniform3fv(posFocusEscena1Loc, 1, &posFocusEsc1[0]);
      break;
    }
    case Qt::Key_T: {
      posFocusEsc1 = posFocusEsc1 + glm::vec3(0.0,0.1,0.0);
      glUniform3fv(posFocusEscena1Loc, 1, &posFocusEsc1[0]);
      break;
    }
    case Qt::Key_3: {
      glUniform3fv (colFocusEscena1Loc, 1, &colFocOff[0]);
      break;
    }
    case Qt::Key_4: {
      glUniform3fv (colFocusEscena1Loc, 1, &colFocOn[0]);
      break;
    }

    // CONTROL FOCUS D'ESCENA 2 - LLUM DIFUSA
    case Qt::Key_M: {
      posFocusEsc2 = posFocusEsc2 - glm::vec3(0.0,0.0,0.1);
      glUniform3fv(posFocusEscena2Loc, 1, &posFocusEsc2[0]);
      break;
    }
    case Qt::Key_N: {
      posFocusEsc2 = posFocusEsc2 + glm::vec3(0.0,0.0,0.1);
      glUniform3fv(posFocusEscena2Loc, 1, &posFocusEsc2[0]);
      break;
    }
     case Qt::Key_B: {
      posFocusEsc2 = posFocusEsc2 - glm::vec3(0.1,0.0,0.0);
      glUniform3fv(posFocusEscena2Loc, 1, &posFocusEsc2[0]);
      break;
    }
    case Qt::Key_V: {
      posFocusEsc2 = posFocusEsc2 + glm::vec3(0.1,0.0,0.0);
      glUniform3fv(posFocusEscena2Loc, 1, &posFocusEsc2[0]);
      break;
    }
    case Qt::Key_X: {
      posFocusEsc2 = posFocusEsc2 - glm::vec3(0.0,0.1,0.0);
      glUniform3fv(posFocusEscena2Loc, 1, &posFocusEsc2[0]);
      break;
    }
    case Qt::Key_Z: {
      posFocusEsc2 = posFocusEsc2 + glm::vec3(0.0,0.1,0.0);
      glUniform3fv(posFocusEscena2Loc, 1, &posFocusEsc2[0]);
      break;
    }
    case Qt::Key_5: {
      glUniform3fv (colFocusEscena2Loc, 1, &colFocOff[0]);
      break;
    }
    case Qt::Key_6: {
      glUniform3fv (colFocusEscena2Loc, 1, &colFocOn[0]);
      break;
    }
    */
    default: event->ignore(); break;
  }
  update();
}


//---------------------------------------------------
//                  OTHER FUNCTIONS
//---------------------------------------------------
void MyGLWidget::carregaShaders()
{
  ExamGLWidget::carregaShaders();

  colFocusCamaraLoc = glGetUniformLocation (program->programId(), "colFocusCam");
  posFocusCamaraLoc = glGetUniformLocation(program->programId(), "posFocusCam");

  colFocusEscena1Loc = glGetUniformLocation (program->programId(), "colFocusEsc1");
  posFocusEscena1Loc = glGetUniformLocation(program->programId(), "posFocusEsc1");

  colFocusEscena2Loc = glGetUniformLocation (program->programId(), "colFocusEsc2");
  posFocusEscena2Loc = glGetUniformLocation(program->programId(), "posFocusEsc2");
}



//-------------------------------------------
//                 TIMER
//-------------------------------------------
void MyGLWidget::MyTimerSlot()
{
    makeCurrent();
    rotacioCoin += (float(M_PI)/180.0f)*10.0f;
    if (rotacioCoin == 2.0f*float(M_PI)) rotacioCoin = 0.0f;
    update();
}

void MyGLWidget::engega()
{
    makeCurrent();
    timer->start(100);
    update();
}

void MyGLWidget::atura()
{
    makeCurrent();
    timer->stop();
    update();
}


void MyGLWidget::setPerspectiva()
{
    makeCurrent();
    camPlanta = false;
    glUniform3fv (colFocusCamaraLoc, 1, &colFocOn[0]);
    glUniform3fv (colFocusEscena1Loc, 1, &colFocOff[0]);
    glUniform3fv (colFocusEscena2Loc, 1, &colFocOff[0]);

    projectTransform();
    viewTransform();
    update();
}

void MyGLWidget::setOrtogonal()
{
    makeCurrent();
    camPlanta = true;

    glUniform3fv (colFocusCamaraLoc, 1, &colFocOff[0]);
    glUniform3fv (colFocusEscena1Loc, 1, &colFocOn[0]);
    glUniform3fv (colFocusEscena2Loc, 1, &colFocOn[0]);

    projectTransform();
    viewTransform();
    update();
}
