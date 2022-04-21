#include<iostream>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtx/transform.hpp>

#define ToRadian(x) ((x) * 3.14159265359 / 180.0f)

GLuint VBO;
GLuint IBO;

unsigned int Indices[] = { 0, 3, 1,
                           1, 3, 2,
                           2, 3, 0,
                           0, 2, 1 };

float scale = 0.000f;
const float ar = 1024 / 768;
const float tanHalfFOV = tanf(ToRadian(30.0f / 2.0f));
const float zNear = 1.0f;
const float zFar = 1000.0f;
const float zRange = zNear - zFar;

void RenderSceneCB()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    scale += 0.0500f;

    glEnableVertexAttribArray(0);
    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
    glDrawElements(GL_LINE_LOOP, 12, GL_UNSIGNED_INT, 0);
    //Triangle move
    glm::mat4 myMatrixMove(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        sinf(scale), 0.0f, 0.0f, 1.0f);
    //Triangle rotation
    glm::mat4 myMatrixRotateZ(cosf(scale), sinf(scale), 0.0f, 0.0f,
        -sinf(scale), cosf(scale), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 myMatrixRotateY(cosf(scale), 0.0f, -sinf(scale), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(scale), 0.0f, cosf(scale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    //Triangle size
    glm::mat4 myMatrixResize(sinf(scale), 0.0f, 0.0f, 0.0f,
        0.0f, sinf(scale), 0.0f, 0.0f,
        0.0f, 0.0f, sinf(scale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 myMatrixPersProj(1.0f / (tanHalfFOV * ar), 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f,
                               0.0f, 0.0f, (-zNear - zFar) / zRange, 2.0f * zFar * zNear / zRange,
                               0.0f, 0.0f, 1.0f, 0.0f);
    glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, -2.0),
                                       glm::vec3(0.0, 0.0, 0.0),
                                       glm::vec3(1.0, 2.0, 1.0));
    glm::mat4 myMatrixTransformation = myMatrixPersProj * ViewMatrix * myMatrixRotateY;
    glLoadMatrixf(reinterpret_cast<const float*>(&myMatrixTransformation));

    glutSwapBuffers();
    glDisableVertexAttribArray(0);
    glutIdleFunc(RenderSceneCB);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1280, 768);
    glutInitWindowPosition(200, 50);
    glutCreateWindow("Lab Work 2");
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glm::vec3 Vertices[4];
    Vertices[0] = glm::vec3(-0.5f, -0.5f, 0.0f);
    Vertices[1] = glm::vec3(0.0f, -0.5f, 0.5f);
    Vertices[2] = glm::vec3(0.5f, -0.5f, 0.0f);
    Vertices[3] = glm::vec3(0.0f, 0.5f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glutDisplayFunc(RenderSceneCB);
    glutMainLoop();
}