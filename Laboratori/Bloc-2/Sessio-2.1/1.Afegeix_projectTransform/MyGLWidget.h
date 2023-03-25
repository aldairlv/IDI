// MyGLWidget.h
#include "BL2GLWidget.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL();
    virtual void carregaShaders();
    virtual void projectTransform();
    GLuint projLoc;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
