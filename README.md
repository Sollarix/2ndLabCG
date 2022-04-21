# 2nd Laboratory of Computer Graphics

    glm::vec3 Vertices[4];
    Vertices[0] = glm::vec3(-0.5f, -0.5f, 0.0f);
    Vertices[1] = glm::vec3(0.0f, -0.5f, 0.5f);
    Vertices[2] = glm::vec3(0.5f, -0.5f, 0.0f);
    Vertices[3] = glm::vec3(0.0f, 0.5f, 0.0f);
Instead of using 3 points for triangle, we use 4 points for pyramid, so we can see clearer changes in our program.

And, for 3D graphic, we need to define buffer of indices, lets do this:
    
    GLuint IBO;

    unsigned int Indices[] = { 0, 3, 1,
                           1, 3, 2,
                           2, 3, 0,
                           0, 2, 1 };
     ...
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
With this code we create another global variable GLuint which stores inside the array of indices.

So, to draw our pyramid, lets change our drawing funtion in render:
    
    glDrawElements(GL_LINE_LOOP, 12, GL_UNSIGNED_INT, 0);
Instead of using GL_LINE_LOOP we could also use GL_TRIANGLES.

Lets now look at our matrices:

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
    //Perspective
    glm::mat4 myMatrixPersProj(1.0f / (tanHalfFOV * ar), 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f,
                               0.0f, 0.0f, (-zNear - zFar) / zRange, 2.0f * zFar * zNear / zRange,
                               0.0f, 0.0f, 1.0f, 0.0f);
    //Camera
    glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, -2.0),
                                       glm::vec3(0.0, 0.0, 0.0),
                                       glm::vec3(1.0, 2.0, 1.0));
This is our matrices of different transformations of our pyramid. We can move, rotate, resize, use perspecitve or change camera. In perspective matrix used this variables:

    const float ar = 1024 / 768;
    const float tanHalfFOV = tanf(ToRadian(30.0f / 2.0f));
    const float zNear = 1.0f;
    const float zFar = 1000.0f;
    const float zRange = zNear - zFar;
    
 To apply this matrices to our vector we use this function:
 
    glm::mat4 myMatrixTransformation = myMatrixPersProj * ViewMatrix * myMatrixRotateY;
    glLoadMatrixf(reinterpret_cast<const float*>(&myMatrixTransformation));

myMatrixTransformation can be product of all our transformation, it depends on our wishes.

**glLoadMatrixf(reinterpret_cast<const float\*>(&myMatrixTransformation))** - glLoadMatrix replaces the current matrix with the one whose elements are specified by m. 
The current matrix is the projection matrix, modelview matrix, or texture matrix, depending on the current matrix mode.

The current matrix, M, defines a transformation of coordinates. For instance, assume M refers to the modelview matrix. 
If v=(v[0],v[1],v[2],v[3]) is the set of object coordinates of a vertex, and m points to an array of 16 single- or double-precision floating-point values m={m[0],m[1],...,m[15]}, 
then the modelview transformation M(v) does the following: 

         |m[0]m[1]m[2]m[3]    |   |v[0]|
    M(v)=|m[4]m[5]m[6]m[7]    | * |v[1]|
         |m[8]m[9]m[10]m[11]  |   |v[2]|
         |m[12]m[13]m[14]m[15]|   |v[3]|
Projection and texture transformations are similarly defined.

 **glutIdleFunc(RenderSceneCB)** -  sets the global idle callback to be func so a GLUT program can perform background processing tasks or continuous animation when window system events are not being received. If enabled, the idle callback is continuously called when events are not being received. The callback routine has no parameters. The current window and current menu will not be changed before the idle callback. Programs with multiple windows and/or menus should explicitly set the current window and/or current menu and not rely on its current setting.

The amount of computation and rendering done in an idle callback should be minimized to avoid affecting the program's interactive response. In general, not more than a single frame of rendering should be done in an idle callback.

Passing NULL to glutIdleFunc disables the generation of the idle callback.
 
