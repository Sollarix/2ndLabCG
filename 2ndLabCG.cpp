#include<iostream>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>

GLuint VBO;
float scale = 0.000f;

void RenderSceneCB()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    scale += 0.0025f;

    glEnableVertexAttribArray(0);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
    glm::mat4 myMatrixTransformation = myMatrixMove * myMatrixRotateZ * myMatrixRotateY * myMatrixResize;
    glLoadMatrixf(reinterpret_cast<const float*>(&myMatrixTransformation));

    glutSwapBuffers();
    glDisableVertexAttribArray(0);
    glutIdleFunc(RenderSceneCB);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(200, 50);
    glutCreateWindow("Lab Work 2");
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glm::vec3 Vertices[3] = { {0.25f, 0.25f, 0.0f}, {-0.25f, 0.25f, 0.0f}, {0.0f, -0.25f, 0.0f} };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glutDisplayFunc(RenderSceneCB);
    glutMainLoop();
}