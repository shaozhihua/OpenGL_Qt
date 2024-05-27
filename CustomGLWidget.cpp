#include "CustomGLWidget.h"
#include <QDebug>
#include <QVector4D>
#include <QtMath>


CustomGLWidget::CustomGLWidget(QWidget* parent):
    QOpenGLWidget(parent),
    m_qvao(),
    m_qvbo(QOpenGLBuffer::VertexBuffer),
    m_qebo(QOpenGLBuffer::IndexBuffer)
{


}

CustomGLWidget::~CustomGLWidget()
{
    makeCurrent();
    m_qvbo.destroy();
    doneCurrent();
}




void CustomGLWidget::initializeGL()
{
    // 1-初始化OpenGL functions
    initializeOpenGLFunctions();

    // 2-初始化shader
    initShader();
    // 3-初始化顶点数据
    drawDataInit();

    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // qDebug()<< "顶点数据最大属性数量支持为：" << nrAttributes;

}

void CustomGLWidget::initShader()
{
    // 定义顶点着色器
    const char *vertexShader = "#version 330 core \n"
                               "layout (location = 0) in vec3 aPos; \n"
                               "void main(){ \n"
                               "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
                               "}";
    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShader);
    //m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/axxx");

    // 定义片段着色器
    const char *fragmentShader = "#version 330 core \n"
                                 "out vec4 FragColor; \n"
                                 "void main() { \n"
                                 "FragColor = vec4(0.5,0.2,0.3,1.0); \n"
                                 "}";

    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShader);
    if(!m_shaderProgram.link())
    {
        qDebug()<<"link failed!";
        return;
    }
    m_shaderProgram.bind();
}

void CustomGLWidget::setWireframe(bool wireframe)
{
    makeCurrent();
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    update();
    doneCurrent();
}

void CustomGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制前重新绑定vao
    m_qvao.bind();

    glDrawArrays(GL_TRIANGLES,0,3);

    m_qvao.release();
    // 循环绘制
    update();
}

void CustomGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void CustomGLWidget::drawDataInit()
{
    m_qvao.create();
    m_qvao.bind();
    // 定义三角形顶点,笛卡尔直角坐标系
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    m_qvbo.create();
    m_qvbo.bind();
    m_qvbo.allocate(vertices,3 * 3 * sizeof(GLfloat));



    // 解析顶点数据
    int attr = -1;
    attr = m_shaderProgram.attributeLocation("aPos");
    // 参数解释——少之华版
    // attr:我们要解析哪个location的数据，有的数据的location会在着色器程序中指明，有的则不会
    // GL_FLOAT:每个数据元素的数据类型
    // 0:从第几个元素开始
    // 3:一个顶点数据(你这个Location这儿的)是由几个数组成的，几维的，一个元组里几个数来表示你这一个顶点数据(位置也好，颜色也好)
    // 4:步长，一个顶点数据的步长，如果形容一个颜色，由r,g,b,a四个float数值表示，则这个的一个颜色(顶点)数据的步长为4*sizeof(float)
    // 备注：你可能会疑惑，最后一个值怎么可能会有是0的时候呢，其实，真的有，说一千道一万，这个函数只是解析顶点数据的功能
    // 我们如果解析到某个地方的时候，比如第四个到第六个，都是一元，一维数据，比如光强，那么前面的偏移将是3，后面的步长则为0，
    // 因为三个光强都是要传到着色器程序中去的，可能是三个时刻的光强等，所以是紧密挨着的，所以，最后一个参数步长为0是有可能的。
    m_shaderProgram.setAttributeBuffer(attr, GL_FLOAT,0,3,sizeof(GLfloat) * 3);
    m_shaderProgram.enableAttributeArray(attr);

    m_qvao.release();
    m_qvbo.release();

}









