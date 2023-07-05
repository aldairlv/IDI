#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class ExamGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    ExamGLWidget (QWidget *parent=0);
    ~ExamGLWidget ();
    
    QTimer *timer;

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    
    
    virtual void modelTransformBrick ();
    virtual void modelTransformGreenPipe ();
    virtual void modelTransformFantasma ();
    virtual void modelTransformGroundBlocks3x3 (glm::vec3 posicio);
    virtual void modelTransformLuckyBox ();
    virtual void modelTransformMorty ();
    virtual void modelTransformCoin (float angleCoin);
    virtual void modelTransformCoin ();
    virtual void modelTransformMushroom ();
    
    
    virtual void enviaColFocus ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    void creaBuffersBrick ();
    void creaBuffersGreenPipe ();
    void creaBuffersFantasma ();
    
    void creaBuffersGroundBlocks3x3 ();
    void creaBuffersLuckyBox ();
    void creaBuffersMorty ();
    void creaBuffersCoin ();
    void creaBuffersMushroom ();
    void carregaShaders ();
    void calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase);

    void engega();
    void atura();

 
    
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc, colfocusLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // Matriu de posició i orientació
    glm::mat4 View;
    // Matriu de transformació del model
    glm::mat4 TG;



    float escala;
   

    glm::vec3 centreEsc;
    float radiEsc, ra, fov, zn, zf;

    // Definim els paràmetres del material del terra
    glm::vec3 amb, diff, spec;
    float shin;

    glm::vec3 colFoc;
    float angleX, angleY;
    bool camPlanta;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    

    
    //------------------------
    //        BRICK
    //------------------------
    // VAO names
    GLuint VAO_Brick;
    // model
    Model Brick;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaBrick;
    glm::vec3 centreBaseBrick;
        

    //------------------------
    //        GREEN PIPE
    //------------------------
    // VAO names
    GLuint VAO_GreenPipe;
    // model
    Model GreenPipe;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaGreenPipe;
    glm::vec3 centreBaseGreenPipe;
    
    //------------------------
    //        FANTASMA
    //------------------------
    // VAO names
    GLuint VAO_Fantasma;
    // model
    Model Fantasma;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaFantasma;
    glm::vec3 centreBaseFantasma;
    
    //------------------------
    //        GROUND BLOCK 3X3
    //------------------------
    // VAO names
    GLuint VAO_GroundBlocks3x3;
    // model
    Model GroundBlocks3x3;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaGroundBlocks3x3;
    glm::vec3 centreBaseGroundBlocks3x3;
    
    //------------------------
    //        LUCKY BOX
    //------------------------
    // VAO names
    GLuint VAO_LuckyBox;
    // model
    Model LuckyBox;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaLuckyBox;
    glm::vec3 centreBaseLuckyBox;
    
    //------------------------
    //        Morty
    //------------------------
    // VAO names
    GLuint VAO_Morty;
    // model
    Model Morty;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaMorty;
    glm::vec3 centreBaseMorty;
    
    //------------------------
    //        Coin
    //------------------------
    // VAO names
    GLuint VAO_Coin;
    // model
    Model Coin;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaCoin;
    glm::vec3 centreBaseCoin;
    
    //------------------------
    //        Mushroom
    //------------------------
    // VAO names
    GLuint VAO_Mushroom;
    // model
    Model Mushroom;
    // paràmetres calculats a partir de la capsa contenidora del model
    float escalaMushroom;
    glm::vec3 centreBaseMushroom;
   
    glm::vec3 vrp;
    glm::vec3 obs;
    glm::vec3 up;
    
    bool ortogonal;  
    float l,r,b,t;
    float rotacioCoin;  
    
    bool efectoCoin;




    
    float xval;
    float yval;
    float zval;    
    //public slots:
    //void MyTimerSlot();
    //void atura();
    //void engega();
};

