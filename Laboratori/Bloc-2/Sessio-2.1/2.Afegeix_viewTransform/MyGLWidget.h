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
    virtual void viewTransform();
    virtual void ini_camera();
    GLuint projLoc;
    GLuint viewLoc;
    glm::vec3 obs;
    glm::vec3 vrp;
    glm::vec3 up;
    float fov;
    float ra;
    float znear;
    float zfar;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
