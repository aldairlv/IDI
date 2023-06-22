#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
  	virtual void initializeGL ();
  	virtual void iniEscena ();
        virtual void iniCamera ();
        virtual void paintGL ();
	virtual void RickTransform ();
	virtual void MortyTransform ();
	virtual void VideoCameraTransform ();
	virtual void viewTransform ();
	virtual void viewTransform2();
	virtual void projectTransform ();
	virtual void projectTransform2();
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void carregaShaders();
	virtual void resizeGL (int w, int h);

  private:
  	int printOglError(const char file[], int line, const char func[]);
    	float angleRick;
	float angleCameraX;
	float angleCameraY;    
	float angleX;
	float angleY;
	float angleMortyY;
	bool camera2;
	bool invisible;
	GLuint greenLoc;
	GLuint invisibleLoc;
	glm::vec3 colGreen;
	float fovIni;
};
