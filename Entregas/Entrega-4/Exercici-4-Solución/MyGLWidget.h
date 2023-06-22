// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();

    QTimer *timer;
  protected:
    virtual void initializeGL ( );
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintGL ( );
    virtual void modelTransformMorty ();
    virtual void modelTransformDiscoBall ();
    virtual void modelTransformFantasma (float posX);
    virtual void modelTransformFocus();
    virtual void carregaShaders();
  public slots:
    void MyTimerSlot();
  private:
    int printOglError(const char file[], int line, const char func[]);
    float angleDiscoBall;
    float angleMorty;
    float angleFantasma;
    GLuint transFocusLoc;
    GLuint colorFocusLocE, posFocusLocE;
    GLuint colorFocusID0Loc, posFocusID0Loc;
    GLuint colorFocusID1Loc, posFocusID1Loc;
    GLuint colorFocusID2Loc, posFocusID2Loc;
    GLuint colorFocusID3Loc, posFocusID3Loc;
    bool focusEscenaActiva;
    glm::vec3 posFocuse, colorFocuse;
    glm::vec3 posFocusID0, colorFocusID0;
    glm::vec3 posFocusID1, colorFocusID1;
    glm::vec3 posFocusID2, colorFocusID2;
    glm::vec3 posFocusID3, colorFocusID3;
    int numFocus;
    bool efectoDiscoActiu;
};
