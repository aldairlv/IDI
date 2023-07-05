#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL ( );
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
  public slots:
    void setCamaraOrto();
    void setCamaraPers();
    void setCubAssociat(int idCub);
signals:
    void ort();
    void per();
    void resetCub(int val);
  private:
    int printOglError(const char file[], int line, const char func[]);
    float anglePatricio;
    bool patricioActive;
    bool focusGrocActive;
    glm::vec3 obs;
    glm::vec3 vrp;
    glm::vec3 up;
    float l,r,b,t;
    float angleHorari;
};
