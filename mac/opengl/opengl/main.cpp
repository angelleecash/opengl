//
//  main.cpp
//  opengl
//
//  Created by chenliang on 13-12-12.
//  Copyright (c) 2013年 chenliang. All rights reserved.
//

#include "main.h"
#include <GLTools.h>

#include <GLUT/GLUT.h>

GLBatch triangleBatch;
GLShaderManager shaderManager;

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SetupRenderingContext()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    
    GLfloat vs[] = {-0.5f, 0.0f, 0.0f,
                    0.5f, 0.0f, 0.0f,
                    0.0f, 0.5f, 0.0f};
    
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vs);
    triangleBatch.End();
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, red);
    
    triangleBatch.Draw();
    
    glutSwapBuffers();
    
}

int main(int argc, char* argv[])
{
    for (int i=0; i<argc; i++) {
        printf("%s\n", argv[i]);
    }
    
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("triangle");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        fprintf(stderr, "GLEW error %s\n", glewGetErrorString(err));
        return 1;
    }
    
    SetupRenderingContext();
    
    GLfloat pointSizes[2];
    GLfloat pointSizeStep;
    
    glGetFloatv(GL_POINT_SIZE_RANGE, pointSizes);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &pointSizeStep);
    
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* shadingLanguageVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("OpenGL version: %s \nvendor: %s \nrenderer: %s \nshading language: %s", version, vendor, renderer, shadingLanguageVersion);
    
    for (int i=0; i<GL_NUM_EXTENSIONS; i++) {
        //const GLubyte* extension = glGetStringi(GL_EXTENSIONS, i);
        //printf("%s \n", extension);
    }
    return 0;
    glutMainLoop();
    
    return 0;
}