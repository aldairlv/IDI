// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();
  
  public slots: 
    void zoom(int z);
    void pintarModel(bool pm);
    void cambiarOptica();
    void cambiarEscala(int e);
    void cambiarAngleY(int y);
    void cambiarAngleX(int x);
    void guardarRed(int r);
    void guardarGreen(int g);
    void guardarBlue(int b);
    //void cambiarColorDeFons(int r, int g, int b);
  signals: 
    void setEscala(int s);
    void enviarRed(int r);
    void enviarGreen(int g);
    void enviarBlue(int b);
  protected:
    virtual void setColorDeFons();
    virtual void paintGL();
    virtual void initializeGL();
    virtual void carregaShaders();
    virtual void projectTransform();
    virtual void viewTransform();
    virtual void ini_camera();
    virtual void creaBuffersModelPatricio();
    virtual void creaBuffersModelLego();
    virtual void creaBuffersTerra();
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void modelTransform_Patricio1();
    virtual void modelTransform_Patricio2();
    virtual void modelTransform_Patricio3();
    virtual void modelTransformTerra ();
    virtual void centreRadi(glm::vec3 pmin, glm::vec3 pmax);
    virtual void resizeGL (int width, int height);
    virtual void capsaModel();
    virtual void mousePressEvent (QMouseEvent *e);
    virtual void mouseReleaseEvent( QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *e);
    
    GLuint projLoc;
    GLuint viewLoc;
    glm::vec3 obs;
    glm::vec3 vrp;
    glm::vec3 up;
    float fov;
    float raw;
    float znear;
    float zfar;
    Model m;
    
    GLuint VAO_modelPatricio;
    GLuint VAO_modelLego;
    
    GLuint VAO_terra;
    float angle;
    
    glm::vec3 puntmin;
    glm::vec3 puntmax;
    
    glm::vec3 centreEscena;
    float radiEsfera;
    float rav;
    float fovIni;
    
    glm::vec3 centreModel;
    glm::vec3 centreModelBase;
     
    float altura;
    float nuevaAltura;
    float escalaModel;
     
    bool ortogonal;
    float left,right,bottom,top;
    float angleX, angleY, angleZ;
     
    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    
    float angle_patricio1;
    float angle_patricio2;
    float angle_patricio3;
    
    bool pintarLego;
    glm::vec3 centreModelBasePatricio;
    glm::vec3 centreModelBaseLego;
    
    float escalaModelPatricio;
    float escalaModelLego;
    
    int spinEscala;
    
    float vermell, verd, blau;
  
  private:
    int printOglError(const char file[], int line, const char func[]);
};
