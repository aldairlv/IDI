// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL();
    virtual void initializeGL();
    virtual void carregaShaders();
    virtual void projectTransform();
    virtual void viewTransform();
    virtual void ini_camera();
    virtual void creaBuffersModel();
    virtual void creaBuffersTerra();
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void modelTransform ();
    virtual void modelTransformTerra ();
    virtual void centreRadi(glm::vec3 pmin, glm::vec3 pmax);
    GLuint projLoc;
    GLuint viewLoc;
    glm::vec3 obs;
    glm::vec3 vrp;
    glm::vec3 up;
    float fov;
    float ra;
    float znear;
    float zfar;
    Model m;
    GLuint VAO_model;
    GLuint VAO_terra;
    float angle;
    
    glm::vec3 puntmin;
    glm::vec3 puntmax;
    
    glm::vec3 centreEscena;
    float radiEsfera;
  private:
    int printOglError(const char file[], int line, const char func[]);
};
