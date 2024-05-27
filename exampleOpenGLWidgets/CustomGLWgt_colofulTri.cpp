#include "CustomGLWgt_colofulTri.h"

CustomGLWgt_colofulTri::CustomGLWgt_colofulTri(QWidget* parent):
    QOpenGLWidget(parent),
    m_qvao(),
    m_qvbo(QOpenGLBuffer::VertexBuffer),
    m_qebo(QOpenGLBuffer::IndexBuffer)
{

}

CustomGLWgt_colofulTri::~CustomGLWgt_colofulTri()
{
    makeCurrent();
    m_qvbo.destroy();
    doneCurrent();
}

void CustomGLWgt_colofulTri::initializeGL()
{
    // 1-初始化OpenGL functions
    initializeOpenGLFunctions();
    // 2-初始化shader
    initShader();
    // 3-初始化绘制数据
    drawDataInit();

    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // qDebug()<< "顶点数据最大属性数量支持为：" << nrAttributes;
}

void CustomGLWgt_colofulTri::setWireframe(bool wireframe)
{
    makeCurrent();
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // 否则不能重新绘制
    update();
    doneCurrent();
}

void CustomGLWgt_colofulTri::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制前重新绑定vao
    m_qvao.bind();

    // 2-绘制彩色三角形
    glDrawArrays(GL_TRIANGLES,0,3);


    m_qvao.release();
    // 循环绘制
    update();
}

void CustomGLWgt_colofulTri::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void CustomGLWgt_colofulTri::initShader()
{
    // 定义顶点着色器
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/bin/shader/colorfulTriangle.vert");

    // 定义片段着色器
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/bin/shader/colorfulTriangle.frag");
    if(!m_shaderProgram.link())
    {
        qDebug()<<"link failed!";
        return;
    }
    m_shaderProgram.bind();
}

void CustomGLWgt_colofulTri::drawDataInit()
{
    /*
        绘制该举行，为了效率，我们引用了元素缓冲对象，或者叫索引缓冲对象，就是因为一个矩形，由两个相邻的三角形组成，
    可以发现，有两个点是重合的，虽然是六个点，两个三角形嘛，六个点，但是有两对儿，是重合成了一个点，真正不在同一位置的
    点，或者说不同的点，一共也就四个，分别是矩形的四个顶点。所以，我们就只需要准备四个点数据在vertex数组中即可，但是
    问题又来了，如果只有四个点，那么他们之间的连线，或者说，他们之间先画哪个点，再画哪个点，这个顺序，以及点与点之间的
    连线，也就是顺序，怎么规定呢？这就引出了EBO——Element Buffer Object。

    */

    m_qvao.create();
    m_qvao.bind();

    // 准备顶点数据
    GLfloat vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };


    m_qvbo.create();
    m_qvbo.bind();
    m_qvbo.allocate(vertices,6 * 3 *sizeof(GLfloat));

    // 解析顶点数据
    int attr = -1;
    attr = m_shaderProgram.attributeLocation("aPos");
    m_shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    m_shaderProgram.enableAttributeArray(attr);

    attr = m_shaderProgram.attributeLocation("aColor");
    m_shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(GLfloat), 3, sizeof(GLfloat) * 6);
    m_shaderProgram.enableAttributeArray(attr);


    m_qvao.release();
    m_qvbo.release();
}
