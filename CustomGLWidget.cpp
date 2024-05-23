#include "CustomGLWidget.h"
#include <QDebug>
#include <QVector4D>
#include <QtMath>



CustomGLWidget::CustomGLWidget(QWidget* parent):
    QOpenGLWidget(parent)
{
    m_shape = None;

}

void CustomGLWidget::drawShape(Shape shape)
{
    m_shape = shape;
    update();
}


void CustomGLWidget::initializeGL()
{
    // 初始化OpenGL functions
    initializeOpenGLFunctions();

    initGL();

}

void CustomGLWidget::setWireframe(bool wireframe)
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

void CustomGLWidget::paintGL()
{
    // 清除屏幕颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    //shader1.bind();
    //glBindVertexArray(m_VAO[1]);
    //glDrawArrays(GL_TRIANGLES,0,3);

    // 绑定shader
    GLuint location = shader1.uniformLocation("uniColor");
    // 这重载程度我甚至不需要得到那个location
    shader1.setUniformValue("uniColor",QVector4D(0.4,qAbs(qSin((float)(time.second()))),0.3,1.0));
    shader1.bind();
    // 绑定VAO
    glBindVertexArray(m_VAOs[1]);


    switch (m_shape){
    case Triangle:
        glDrawArrays(GL_TRIANGLES,0,3);
        break;
    case Rectangle:
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        break;
    // 否则，不进行绘制
    default:
        break;
    }

    // 绘制数组-最后一个参数可以换成(void*)indices[3] 第三个元素地址,这样就只能画一半的索引
    glDrawArrays(GL_TRIANGLES,0,3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

    // 循环绘制
    update();
}

void CustomGLWidget::resizeGL(int w, int h)
{

}

void CustomGLWidget::initGL()
{
    // 设置清除颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    // 3个顶点
    GLfloat vertexs[] = {
        -0.9f,-0.5f,0.0f,
        0.0f,-0.5f,0.0f,
        0.45f,0.5f,0.0f
    };
    // 4个顶点
    // GLfloat vertexs[] = {
    //     0.5f, 0.5f, 0.0f,
    //     0.5f,-0.5f, 0.0f,
    //     -0.5f,-0.5f,0.0f,
    //     -0.5f,0.5f,0.0f
    // };

    float vertexs1[] = {
        // 第二个三角形的顶点数据
        0.0f, -0.5f, 0.0f,
        0.9f, -0.5f, 0.0f,
        0.45f, 0.5f, 0.0f
    };

    GLuint indices[] = {
        0,1,3,
        1,2,3
    };

    // 创建VAO
    // 1-需要创建的缓存数量
    // m_VAO-VAO的id
    glGenVertexArrays(3,m_VAOs);
    // 生成一个VBO id
    glGenBuffers(3,m_VBOs);
    // 生成EBO id
    glGenBuffers(1,&m_EBO);


    /// 绑定第一个三角形相关
    // 绑定VAO至OpenGL，所有跟GL_ARRAY_BUFFER有关的操作，VAO都会记录下来
    glBindVertexArray(m_VAOs[0]);
    // 配置VBO相关
    // 绑定缓冲对象VBO到OpenGL状态机上
    glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[0]);
    // 把当前绑定在某种类型的数据，所指向的内存对象传输到GPU缓存
    // GL_STATIC_DRAW-数据几乎不会改变
    // GL_DYNAMIC_DRAW-数据会被改变很多
    // GL_STREAM_DRAW-数据每次绘制时都会改变
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexs),vertexs,GL_STATIC_DRAW);

    // 配置EBO相关
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    // 设置绘制模式
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);



    // 设置顶点属性指针(解析顶点数据)
    // 0-对应顶点着色器代码中的location
    // 3-顶点属性的大小，纹理就是2，如果这是这种三维顶点，那就写3,以后可能还会有颜色和光照的数据放到VBO数据中，所以，
    // 这个值先待定吧
    // GL_FLOAT 数据类型
    // GL_FALSE 是否将顶点数据标准化，如果标准化的话，不论你写的什么数据，哪怕你有意而为之的
    // 没有将顶点数据写在[-1,1]之间，他也会将数据标准化到[-1,1]，然后显示在视窗中
    // 3*sizeof(GLfloat)
    // 一个顶点的步长就像上面说的，有可能一个顶点的三个数据后，还有对于该点的颜色，光照等信息，所以具体都多少个，
    // 这里的步长很重要，在绘制函数drawXXX中，那个顶点数就是根据这个步长来定的数，为什么3个float是一个顶点。
    // 就用几乘上GLFloat
    // (void*)0 从第几个数据开始读
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    // 以顶点属性位置值作为参数，启用顶点属性，让着色器可以访问这块数据
    glEnableVertexAttribArray(0);


    // OpenGL绑定空缓冲区,解绑操作，先解绑VAO，再解绑VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /// 绑定第二个三角形
    glBindVertexArray(m_VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs1), vertexs1, GL_STATIC_DRAW);
    // 还是放在location = 0的位置
    // 顶点数据解析
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLfloat*)0);
    // 启用指针
    glEnableVertexAttribArray(0);

    // 解绑
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    /// 绑定第三个三角形
    // 顶点数据
    float vertices_data[18] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };
    glBindVertexArray(m_VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);
    // 还是放在location = 0的位置

    // 顶点数据解析-顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    // 顶点数据解析-2顶点颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // 着色器代码
    // 1-着色器时运行在GPU里的程序
    // 2-着色器是使用GLSL高级着色器语言进行编写的

    // 创建顶点着色器
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceFile("./shader/triangle.vert");
    //bool success = vertexShader.compileSourceCode(vertexShaderSource);
    qDebug()<<"vertex compileSourceFile: "<<success;

    // 创建片段着色器1
    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success = fragmentShader.compileSourceFile("./shader/triangle.frag");
    qDebug()<<"fragment compileSourceFile: "<<success;

    // 创建片段着色器2
    QOpenGLShader fragmentShader1(QOpenGLShader::Fragment);
    success = fragmentShader1.compileSourceFile("./shader/triangle1.frag");
    qDebug()<<"fragment compileSourceFile: "<<success;
    // 连接着色器程序1
    shader.addShader(&vertexShader);
    shader.addShader(&fragmentShader);
    success = shader.link();

    // 连接着色器程序2
    shader1.addShader(&vertexShader);
    shader1.addShader(&fragmentShader1);
    success = shader1.link();

}



