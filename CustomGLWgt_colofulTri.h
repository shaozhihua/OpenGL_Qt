#ifndef CUSTOMGLWGT_COLOFULTRI_H
#define CUSTOMGLWGT_COLOFULTRI_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_5_Core>

class CustomGLWgt_colofulTri : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    explicit CustomGLWgt_colofulTri(QWidget* parent = nullptr);
    ~CustomGLWgt_colofulTri();

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
    // QOpenGL
    QOpenGLVertexArrayObject m_qvao;
    QOpenGLBuffer m_qvbo;
    QOpenGLBuffer m_qebo;
    QOpenGLShaderProgram m_shaderProgram;
};

#endif // CUSTOMGLWGT_COLOFULTRI_H
