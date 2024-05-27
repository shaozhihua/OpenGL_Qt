#ifndef CUSTOMGLWIDGET_H
#define CUSTOMGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QElapsedTimer>


class CustomGLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit CustomGLWidget(QWidget* parent = nullptr);
    ~CustomGLWidget();


public slots:
    void setWireframe(bool wireframe);


protected:
    // 初始化OpenGL的一些资源：缓冲对象vao、vbo、ebo、着色器、纹理、摄像机
    virtual void initializeGL() override;
    // OpenGL窗口绘制，画一次，循环的话在函数体内调用update()
    virtual void paintGL() override;
    // 当OpenGL视图、窗口改变时，自动调用
    virtual void resizeGL(int w, int h) override;

private:
    void initShader();
    // 绘制静态平面三角形
    void drawDataInit();


private:
    // time
    QElapsedTimer m_elapsedTime;

    // QOpenGL
    QOpenGLVertexArrayObject m_qvao;
    QOpenGLBuffer m_qvbo;
    QOpenGLBuffer m_qebo;
    QOpenGLShaderProgram m_shaderProgram;



};

#endif // CUSTOMGLWIDGET_H
