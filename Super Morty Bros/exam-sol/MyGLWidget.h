#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

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

    virtual void iniEscena ();
    virtual void iniCamera ();



    virtual void projectTransform ();
    virtual void viewTransform ();

    virtual void enviaColFocus ();
    virtual void modelTransformBrick ();
    virtual void modelTransformGreenPipe ();
    virtual void modelTransformFantasma ();
    virtual void modelTransformLuckyBox ();
    virtual void modelTransformMorty ();
    virtual void modelTransformCoin (float angleCoin);
    virtual void modelTransformMushroom ();
    virtual void carregaShaders();

  public slots:
    void MyTimerSlot();
    void atura();
    void engega();
    void setPerspectiva();
    void setOrtogonal();

  signals:
    void ort();
    void per();
    void eng();



  private:
    int printOglError(const char file[], int line, const char func[]);

    glm::vec3 puntmin;
    glm::vec3 puntmax;

    float fovIni;
    float angleCoin;
    float angleMortyY;
    glm::vec3 posMorty;
    float move;

    GLuint posFocusCamaraLoc;
    GLuint colFocusCamaraLoc;

    GLuint posFocusEscena1Loc;
    GLuint colFocusEscena1Loc;

    GLuint posFocusEscena2Loc;
    GLuint colFocusEscena2Loc;



    glm::vec3 colFocOn;
    glm::vec3 colFocOn2;
    glm::vec3 colFocOff;

    glm::vec3 posFocusCam;
    glm::vec3 posFocusEsc1;
    glm::vec3 posFocusEsc2;
    glm::vec3 posFantasma;

    float xval2;
    float yval2;
    float zval2;

    float posFx;
    float posFy;
    float posFz;

    bool menjat;
    glm::vec3 posMushroom;
};
