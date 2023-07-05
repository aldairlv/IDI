#include "ExamGLWidget.h"

#include <iostream>

ExamGLWidget::ExamGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  DoingInteractive = NONE;
}

ExamGLWidget::~ExamGLWidget ()
{
  if (program != NULL)
    delete program;
}

void ExamGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  carregaShaders();
  iniEscena ();
  iniCamera ();

  //timer = new QTimer(this);
  //connect(timer, SIGNAL(timeout()),this, SLOT(MyTimerSlot()));
}

void ExamGLWidget::iniEscena ()
{
  creaBuffersBrick();
  creaBuffersGreenPipe ();
  creaBuffersFantasma ();
  creaBuffersGroundBlocks3x3 ();
  creaBuffersLuckyBox ();
  creaBuffersMorty ();
  creaBuffersCoin ();
  creaBuffersMushroom ();

  colFoc = glm::vec3(1.0f);
  enviaColFocus();
}

void ExamGLWidget::enviaColFocus()
{
  glUniform3fv (colfocusLoc, 1, &colFoc[0]);
}

void ExamGLWidget::iniCamera ()
{
  camPlanta = false;

  obs = glm::vec3(0, 3, 18);
  vrp = glm::vec3(0, 0, 0);
  up = glm::vec3(0, 1, 0);
  fov = float(M_PI)/4.0f;
  zn = 1;
  zf = 20;
  ra = 1.0f;
  centreEsc = glm::vec3(2.5,2,0);

  projectTransform ();
  viewTransform ();
}

void ExamGLWidget::paintGL ()
{
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

  // Pintem Coin
  glBindVertexArray(VAO_Coin);
  modelTransformCoin();
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
  glBindVertexArray(VAO_Mushroom);
  modelTransformMushroom();
  glDrawArrays(GL_TRIANGLES,0,Mushroom.faces().size()*3);  
  
  glBindVertexArray(0);
}

void ExamGLWidget::resizeGL (int w, int h) 
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
void ExamGLWidget::modelTransformBrick ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(2,3,0));
  TG = glm::scale(TG, glm::vec3 (escalaBrick));
  TG = glm::translate(TG, -centreBaseBrick);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformGreenPipe ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(0,0,-1));
  TG = glm::scale(TG, glm::vec3 (escalaGreenPipe));
  TG = glm::translate(TG, -centreBaseGreenPipe);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformFantasma ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(1,0,0));
  TG = glm::scale(TG, glm::vec3 (escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformGroundBlocks3x3 (glm::vec3 posicio)
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, posicio);
  TG = glm::translate(TG, glm::vec3(0,-2,-3));
  TG = glm::scale(TG, glm::vec3 (escalaGroundBlocks3x3, escalaGroundBlocks3x3, escalaGroundBlocks3x3));
  TG = glm::translate(TG, -centreBaseGroundBlocks3x3);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformLuckyBox ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(3,3,0));
  TG = glm::scale(TG, glm::vec3 (escalaLuckyBox));
  TG = glm::translate(TG, -centreBaseLuckyBox);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformMorty ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(3,0,0));
  TG = glm::scale(TG, glm::vec3 (escalaMorty));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformCoin (float angleCoin)
{
  TG = glm::mat4(1.f);

  TG = glm::translate(TG, glm::vec3(0,3,0));
  TG = glm::scale(TG, glm::vec3 (escalaCoin));
  TG = glm::translate(TG, -centreBaseCoin);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformCoin ()
{
  TG = glm::mat4(1.f);

  TG = glm::translate(TG, glm::vec3(0,3,0));
  TG = glm::scale(TG, glm::vec3 (escalaCoin));
  TG = glm::translate(TG, -centreBaseCoin);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void ExamGLWidget::modelTransformMushroom ()
{
  TG = glm::mat4(1.f);
  TG = glm::scale(TG, glm::vec3 (escalaMushroom));
  TG = glm::translate(TG, -centreBaseMushroom);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


//--------------------------------------------
//        PROJECT AND VIEW TRANSFORMS
//--------------------------------------------
void ExamGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(fov, ra, zn, zf);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void ExamGLWidget::viewTransform ()
{
  View = glm::mat4(1.f);
  View = glm::translate(View, glm::vec3(0, 0, -10));
  View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


//-------------------------------------------------
//             MOUSE OR KEY EVENTS
//-------------------------------------------------
void ExamGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  // Add code ...

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void ExamGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void ExamGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camPlanta)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    // Add code ...

    viewTransform ();
  }

  xClick = e->x();
  // Add code ...

  update ();
}

void ExamGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    default: event->ignore(); break;
  }
  update();
}


//---------------------------------------------------
//                  OTHER FUNCTIONS
//---------------------------------------------------
void ExamGLWidget::calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  escala = 1.0f/(maxy-miny);
  //std::cout << "Escala: " << escala << std::endl;
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
  //std::cout << "centreBase: (" << centreBase[0] << "," << centreBase[1] << "," << centreBase[2] << ")" << std::endl;
  //std::cout << "puntMin: (" << minx << "," << miny << "," << minz << ")" << std::endl;
  //std::cout << "puntMax: (" << maxx << "," << maxy << "," << maxz << ")" << std::endl;
}

void ExamGLWidget::carregaShaders() 
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

  // Demanem identificadors per als uniforms dels shaders
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  colfocusLoc = glGetUniformLocation (program->programId(), "colFocus");
  

}


//---------------------------------------------------
//                      BUFFERS
//---------------------------------------------------
void ExamGLWidget::creaBuffersBrick ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  Brick.load("./models/Bricks.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (Brick, escalaBrick, centreBaseBrick);
  
  // Creació del Vertex Array Object del Brick
  glGenVertexArrays(1, &VAO_Brick);
  glBindVertexArray(VAO_Brick);

  // Creació dels buffers del model brick
  GLuint VBO_Brick[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_Brick);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Brick[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Brick.faces().size()*3*3, Brick.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Brick[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Brick.faces().size()*3*3, Brick.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Brick[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Brick.faces().size()*3*3, Brick.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Brick[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Brick.faces().size()*3*3, Brick.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Brick[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Brick.faces().size()*3*3, Brick.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Brick[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Brick.faces().size()*3, Brick.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);

}

void ExamGLWidget::creaBuffersGreenPipe ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  GreenPipe.load("./models/GreenPipe.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (GreenPipe, escalaGreenPipe, centreBaseGreenPipe);
  
  // Creació del Vertex Array Object del Brick
  glGenVertexArrays(1, &VAO_GreenPipe);
  glBindVertexArray(VAO_GreenPipe);

  // Creació dels buffers del model brick
  GLuint VBO_GreenPipe[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_GreenPipe);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GreenPipe[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GreenPipe.faces().size()*3*3, GreenPipe.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GreenPipe[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GreenPipe.faces().size()*3*3, GreenPipe.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GreenPipe[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GreenPipe.faces().size()*3*3, GreenPipe.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GreenPipe[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GreenPipe.faces().size()*3*3, GreenPipe.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);


  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GreenPipe[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GreenPipe.faces().size()*3*3, GreenPipe.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GreenPipe[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GreenPipe.faces().size()*3, GreenPipe.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void ExamGLWidget::creaBuffersFantasma ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  Fantasma.load("./models/Fantasma.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (Fantasma, escalaFantasma, centreBaseFantasma);
  
  // Creació del Vertex Array Object del Brick
  glGenVertexArrays(1, &VAO_Fantasma);
  glBindVertexArray(VAO_Fantasma);

  // Creació dels buffers del model brick
  GLuint VBO_Fantasma[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_Fantasma);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Fantasma.faces().size()*3*3, Fantasma.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Fantasma.faces().size()*3*3, Fantasma.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Fantasma.faces().size()*3*3, Fantasma.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Fantasma.faces().size()*3*3, Fantasma.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);


  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Fantasma.faces().size()*3*3, Fantasma.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Fantasma[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Fantasma.faces().size()*3, Fantasma.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void ExamGLWidget::creaBuffersGroundBlocks3x3 ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  GroundBlocks3x3.load("./models/GroundBlocks3x3.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (GroundBlocks3x3, escalaGroundBlocks3x3, centreBaseGroundBlocks3x3);
  
  // Creació del Vertex Array Object del GroundBlocks3x3
  glGenVertexArrays(1, &VAO_GroundBlocks3x3);
  glBindVertexArray(VAO_GroundBlocks3x3);

  // Creació dels buffers del model GroundBlocks3x3
  GLuint VBO_GroundBlocks3x3[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_GroundBlocks3x3);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GroundBlocks3x3[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GroundBlocks3x3.faces().size()*3*3, GroundBlocks3x3.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GroundBlocks3x3[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GroundBlocks3x3.faces().size()*3*3, GroundBlocks3x3.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GroundBlocks3x3[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GroundBlocks3x3.faces().size()*3*3, GroundBlocks3x3.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GroundBlocks3x3[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GroundBlocks3x3.faces().size()*3*3, GroundBlocks3x3.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GroundBlocks3x3[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GroundBlocks3x3.faces().size()*3*3, GroundBlocks3x3.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_GroundBlocks3x3[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*GroundBlocks3x3.faces().size()*3, GroundBlocks3x3.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void ExamGLWidget::creaBuffersLuckyBox ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  LuckyBox.load("./models/LuckyBox.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (LuckyBox, escalaLuckyBox, centreBaseLuckyBox);
  
  // Creació del Vertex Array Object del LuckyBox
  glGenVertexArrays(1, &VAO_LuckyBox);
  glBindVertexArray(VAO_LuckyBox);

  // Creació dels buffers del model LuckyBox
  GLuint VBO_LuckyBox[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_LuckyBox);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_LuckyBox[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*LuckyBox.faces().size()*3*3, LuckyBox.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_LuckyBox[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*LuckyBox.faces().size()*3*3, LuckyBox.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_LuckyBox[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*LuckyBox.faces().size()*3*3, LuckyBox.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_LuckyBox[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*LuckyBox.faces().size()*3*3, LuckyBox.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_LuckyBox[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*LuckyBox.faces().size()*3*3, LuckyBox.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_LuckyBox[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*LuckyBox.faces().size()*3, LuckyBox.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void ExamGLWidget::creaBuffersMorty ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  Morty.load("./models/Morty.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (Morty, escalaMorty, centreBaseMorty);
  
  // Creació del Vertex Array Object del Morty
  glGenVertexArrays(1, &VAO_Morty);
  glBindVertexArray(VAO_Morty);

  // Creació dels buffers del model Morty
  GLuint VBO_Morty[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_Morty);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Morty.faces().size()*3*3, Morty.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Morty.faces().size()*3*3, Morty.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Morty.faces().size()*3*3, Morty.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Morty.faces().size()*3*3, Morty.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Morty.faces().size()*3*3, Morty.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Morty[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Morty.faces().size()*3, Morty.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void ExamGLWidget::creaBuffersCoin ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  Coin.load("./models/Coin.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (Coin, escalaCoin, centreBaseCoin);
  
  // Creació del Vertex Array Object del Coin
  glGenVertexArrays(1, &VAO_Coin);
  glBindVertexArray(VAO_Coin);

  // Creació dels buffers del model Coin
  GLuint VBO_Coin[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_Coin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Coin[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Coin.faces().size()*3*3, Coin.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Coin[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Coin.faces().size()*3*3, Coin.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Coin[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Coin.faces().size()*3*3, Coin.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Coin[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Coin.faces().size()*3*3, Coin.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Coin[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Coin.faces().size()*3*3, Coin.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Coin[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Coin.faces().size()*3, Coin.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void ExamGLWidget::creaBuffersMushroom ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  Mushroom.load("./models/Mushroom.obj");
  
  // Calculem la capsa contenidora del model
  calculaCapsaModel (Mushroom, escalaMushroom, centreBaseMushroom);
  
  // Creació del Vertex Array Object del Mushroom
  glGenVertexArrays(1, &VAO_Mushroom);
  glBindVertexArray(VAO_Mushroom);

  // Creació dels buffers del model Mushroom
  GLuint VBO_Mushroom[6];
  
  // Buffer de posicions
  glGenBuffers(6, VBO_Mushroom);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Mushroom[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Mushroom.faces().size()*3*3, Mushroom.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Mushroom[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Mushroom.faces().size()*3*3, Mushroom.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Mushroom[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Mushroom.faces().size()*3*3, Mushroom.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Mushroom[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Mushroom.faces().size()*3*3, Mushroom.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Mushroom[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Mushroom.faces().size()*3*3, Mushroom.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Mushroom[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*Mushroom.faces().size()*3, Mushroom.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}


//-------------------------------------------
//                 TIMER
//-------------------------------------------
/*
void ExamGLWidget::MyTimerSlot()
{
    //makeCurrent();
    // Add code ...
    //update();
}

void ExamGLWidget::engega()
{
    //makeCurrent();
    // Add code ...
    //update();
}

void ExamGLWidget::atura()
{
    //makeCurrent();
    // Add code ...
    //update();
}
*/
