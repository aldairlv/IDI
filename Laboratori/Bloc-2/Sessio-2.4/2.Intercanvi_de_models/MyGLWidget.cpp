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
    xClick = yClick = 0;
    pintarLego = false;
    DoingInteractive = NONE;
    glEnable(GL_DEPTH_TEST);
    ortogonal = false;
    nuevaAltura = 1.0f;
    
    creaBuffersTerra();
    //==================
    creaBuffersModelLego();
    capsaModel(); 
    centreModelBaseLego = centreModelBase;
    escalaModelLego = escalaModel;
    //==================
    creaBuffersModelPatricio();
    capsaModel(); 
    centreModelBasePatricio = centreModelBase;
    escalaModelPatricio = escalaModel;
    
    puntmin = glm::vec3 (-2.5f,0.0f,-2.5f);
    puntmax = glm::vec3 (2.5f,4,2.5f);
    centreRadi(puntmin,puntmax);
    ini_camera();
    
    
    left = -radiEsfera;
    right = radiEsfera;
    bottom = -radiEsfera;
    top = radiEsfera;
    
    projectTransform();
    viewTransform();
    angle = 0.0f;
    angleX = 0.0f;
    angleY = 0.0f;
    angleZ = 0.0f;
    angle_patricio1 = 0.0f;
    angle_patricio2 = float(M_PI)/2.0f;
    angle_patricio3 = float(M_PI);
    

}

void MyGLWidget::carregaShaders(){
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(),"proj");
    viewLoc = glGetUniformLocation(program->programId(),"view");
}

void MyGLWidget::projectTransform(){
//  The projection matrix deals with the lens of the camera. 
    glm::mat4 Proj (1.0f);
    if (ortogonal) Proj = glm::ortho(left,right,bottom,top,znear,zfar);
    else Proj = glm::perspective (fov,raw,znear,zfar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::viewTransform(){
//  Describe the position and orientation of a camera.
    glm::mat4 View(1.0);
//  glm::mat4 View = glm::lookAt(obs,vrp,up);
    View = glm::translate(View,glm::vec3(0.0,0.0,-2.0f*radiEsfera)); 
    View = glm::rotate(View,angleZ,glm::vec3(0.0,0.0,1.0));
    View = glm::rotate(View,angleX,glm::vec3(1.0,0.0,0.0));
    View = glm::rotate(View,angleY,glm::vec3(0.0,1.0,0.0));
    View = glm::translate(View,-centreEscena);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&View[0][0]);
}

void MyGLWidget::ini_camera(){
    fovIni = 2.0f*asin(radiEsfera/(2.0f*radiEsfera));
    fov = fovIni;
    raw = 1.0f;
    znear = radiEsfera;
    zfar = 3.0f*radiEsfera;

    vrp = glm::vec3(centreEscena);
    obs = vrp + (2.0f*radiEsfera)*glm::vec3(0.0f,0.0f,1.0f);
     up = glm::vec3(0.0f,1.0f,0.0f);
}

void MyGLWidget::creaBuffersModelPatricio(){
    m.load("../../models/Patricio.obj");
    glGenVertexArrays(1,&VAO_modelPatricio);
    glBindVertexArray(VAO_modelPatricio);

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

void MyGLWidget::creaBuffersModelLego(){
    m.load("../../models/legoman.obj");
    glGenVertexArrays(1,&VAO_modelLego);
    glBindVertexArray(VAO_modelLego);

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
    /*
    //=========================================================
    //               Control del redimensionament
    //=========================================================
    rav = float(ample)/float(alt);
    if (not ortogonal){
        fov = fovIni;
        raw = rav;
        
        if (rav < 1.0f) fov = 2.0f*atan(tan(fovIni/2.0f)/rav);
    }
    else {
        left = -radiEsfera;
        right = radiEsfera;
        bottom = -radiEsfera;
        top = radiEsfera;
        if (rav > 1.0) {
            left  = -radiEsfera*rav;
            right =  radiEsfera*rav;
        } 
        else {
            bottom = -radiEsfera/rav;
            top    =  radiEsfera/rav;
        }
    }
    projectTransform();
    viewTransform();
    //=========================================================
    */
    
    // Activem el VAO per a pintar el model y1
    if(pintarLego)  {
        glBindVertexArray (VAO_modelLego);
        centreModelBase = centreModelBaseLego;
        escalaModel = escalaModelLego; 
        
    }
    else {
        glBindVertexArray (VAO_modelPatricio);
        centreModelBase = centreModelBasePatricio;
        escalaModel = escalaModelPatricio; 
    }
    // Carreguem la transformació de model per al primer Patricio.
    modelTransform_Patricio1 ();
    // pintem
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
    
    modelTransform_Patricio2 ();
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  
    modelTransform_Patricio3 ();
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
    case Qt::Key_O: {
      ortogonal = !ortogonal;
      resizeGL(ample,alt);
      break;
    }
    case Qt::Key_Z: {
      if(not ortogonal) fov = (fov/2.0f - float(M_PI)/180.0f)*2.0f;
      else {
         left *= 0.9f;
         right *= 0.9f;
         bottom *= 0.9f;
         top *= 0.9f;
      }
      projectTransform();
      break;
    }
    case Qt::Key_X: {
      if (not ortogonal) fov = (fov/2.0f + float(M_PI)/180.0f)*2.0f;
      else {
         left *= 1.1f;
         right *= 1.1f;
         bottom *= 1.1f;
         top *= 1.1f;
      }
      projectTransform();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::modelTransform_Patricio1 () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform,glm::vec3(2.0,0.0,2.0));
  transform = glm::rotate(transform,angle_patricio1,glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(escalaModel));
  transform = glm::translate(transform,glm::vec3(-centreModelBase));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform_Patricio2 () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform,glm::vec3(0.0,0.0,0.0));
  transform = glm::rotate(transform,angle_patricio2,glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(escalaModel));
  transform = glm::translate(transform,glm::vec3(-centreModelBase));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform_Patricio3 () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform,glm::vec3(-2.0,0.0,-2.0));
  transform = glm::rotate(transform,angle_patricio3,glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(escalaModel));
  transform = glm::translate(transform,glm::vec3(-centreModelBase));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra () 
{
  // Matriu de transformació del terra
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(escala,0.0f,escala));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::creaBuffersTerra () 
{
  // Dades del terra
  // Dos VBOs, un amb posició i l'altre amb color
  glm::vec3 posicio[6] = {
	glm::vec3(2.50, 0.0, 2.50),
	glm::vec3(2.50, 0.0, -2.50),
	glm::vec3(-2.50, 0.0, -2.50),
	
	glm::vec3(2.50, 0.0, 2.50),
	glm::vec3(-2.50, 0.0, -2.50),
	glm::vec3(-2.50, 0.0, 2.50)
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
        cout << "centreEscena = (" << centreEscena[0] << "," << centreEscena[1] << "," << centreEscena[2] << ")" << endl;
        cout << "radiEsfera = " << radiEsfera << endl;
}

void MyGLWidget::resizeGL (int w, int h) {
    BL2GLWidget::resizeGL(w,h) ;
    
    rav = float(w)/float(h);
    if (not ortogonal){
        fov = fovIni;
        raw = rav;
        if (rav < 1.0f) fov = 2.0f*atan(tan(fovIni/2.0f)/rav);
    }
    else {
        left = -radiEsfera;
        right = radiEsfera;
        bottom = -radiEsfera;
        top = radiEsfera;
        if (rav > 1.0) {
            left  = -radiEsfera*rav;
            right =  radiEsfera*rav;
	} 
	else {
	    bottom = -radiEsfera/rav;
	    top    =  radiEsfera/rav;
	}
    }
    projectTransform();
    viewTransform();
}

void MyGLWidget::capsaModel(){
    glm::vec3 min;
    glm::vec3 max;
    min[0] = m.vertices()[0];
    min[1] = m.vertices()[1];
    min[2] = m.vertices()[2];
    max = min;
    
    for (unsigned int i = 0; i < m.vertices().size(); i+=3) {
    // Escric per pantalla les coordenades del vèrtexs
    //std::cout << "(x, y, z) = (" << m.vertices()[i] << ", " << m.vertices()[i+1] << ", " << m.vertices()[i+2] << ")" << std::endl;
	if(m.vertices()[i] < min[0]) min[0] = m.vertices()[i];
	if(m.vertices()[i+1] < min[1]) min[1] = m.vertices()[i+1];
	if(m.vertices()[i+2] < min[2]) min[2] = m.vertices()[i+2];
	
	if(m.vertices()[i] > max[0]) max[0] = m.vertices()[i];
	if(m.vertices()[i+1] > max[1]) max[1] = m.vertices()[i+1];
	if(m.vertices()[i+2] > max[2]) max[2] = m.vertices()[i+2];
   }

   //std::cout << "MIN(x, y, z) = (" << min[0] << "," << min[1] << "," << min[2] << ")" << std::endl;
   //std::cout << "MAX(x, y, z) = (" << max[0] << "," << max[1] << "," << max[2] << ")" << std::endl;
  	
        centreModel[0] = (max.x+min.x)/2.0f;
	centreModel[1] = (max.y+min.y)/2.0f;
	centreModel[2] = (max.z+min.z)/2.0f;
	
	centreModelBase[0] = centreModel.x;
	centreModelBase[1] = min.y;
        centreModelBase[2] = centreModel.z;
  
  //std::cout << "CentreModel(x, y, z) = (" << centreModel[0] << "," << centreModel[1] << "," << centreModel[2] << ")" << std::endl;
  //std::cout << "CentreModelBase(x, y, z) = (" << centreModelBase[0] << "," << centreModelBase[1] << "," << centreModelBase[2] << ")" << std::endl;
   	
	altura = max.y - min.y;
	//std::cout << "altura: " << altura << std::endl;
	escalaModel = nuevaAltura/altura;
}


void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton && !(e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier))) 
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

void MyGLWidget::zoom(int z) {
    makeCurrent();
    float v = float(z)/10.0;
    if(not ortogonal) fov = (fovIni/2.0f + (float(M_PI)/180.0f)*v)*2.0f;
    projectTransform();
    update ();
}

void MyGLWidget::pintarModel(bool pm) {
    makeCurrent();
    pintarLego = pm;
    update();
}
