#ifndef CUSTOMGLWIDGET_H
#define CUSTOMGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QTime>


class CustomGLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit CustomGLWidget(QWidget* parent = nullptr);
    enum Shape{
        None,
        Triangle,
        Rectangle,
        Circle
    };

    // 通过EBO绘制矩形
    void drawShape(Shape shape);
    void setWireframe(bool wireframe);


protected:
    // 初始化OpenGL的一些资源：缓冲对象vao、vbo、ebo、着色器、纹理、摄像机
    virtual void initializeGL() override;
    // OpenGL窗口绘制，画一次，循环的话在函数体内调用update()
    virtual void paintGL() override;
    // 当OpenGL视图、窗口改变时，自动调用
    virtual void resizeGL(int w, int h) override;

private:
    void initGL();
    void initGLDrawPrepare();


private:
    //OpenGL
    GLuint m_VBOs[3];
    GLuint m_VAOs[3];
    GLuint m_VBO;
    GLuint m_VAO;
    GLuint m_EBO;

    // draw Shape
    Shape m_shape;

    // time
    qint64 m_time = 0;
    QTimer *m_timer = nullptr;

    // QOpenGL
    QOpenGLVertexArrayObject m_qvao;
    QOpenGLBuffer m_qvbo;
    QOpenGLShaderProgram shader;
    QOpenGLShaderProgram shader1;



};

#endif // CUSTOMGLWIDGET_H
