#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

Mesh::~Mesh()
{

}

void Mesh::Draw(QOpenGLShaderProgram* shader,vector<QOpenGLTexture*> textureInstances)
{
shader->bind();
    for(unsigned int i = 0; i < textures.size(); i++)
    {
    //遍历网格纹理，获取纹理类型，并读出纹理对象
        QString name = textures[i].type;
        if(name == "texture_diffuse")
        {
        //bind(),QOpenGLTexture函数，直接绑定到指定的纹理单元，而不用再先glActiveTexture了，很方便
            textureInstances[textures[i].num]->bind(0);
            //shaderProgram也直接封装了，setUnifromxxx的函数也直接封装成setUnifromValue()这一个函数，很方便
            shader->setUniformValue(("material." + name).toLatin1().data(), 0);
            //toLatin1()从QStirng 转为QByteArray
            //data()转为char *
            //设置着色器中此变量纹理也第0槽纹理，也就是刚bind 的
        }
        else if(name == "texture_specular")
        {
        //同上
            textureInstances[textures[i].num]->bind(1);
            shader->setUniformValue(("material." + name).toLatin1().data(), 1);
        }
    }
    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader->release();
}

void Mesh::setupMesh()
{
    initializeOpenGLFunctions();
    //类似glew的init函数，初始化opengl函数库，必须在第一步用，否则下面的函数都不能使用
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
