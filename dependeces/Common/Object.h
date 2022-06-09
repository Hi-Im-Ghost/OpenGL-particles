#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "stb_image.h"
#include "objloader.hpp"

bool LoadTexture(GLuint shaderID, const std::string& texturePath, GLchar *name,GLint out[]);

class Object {
protected:
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> uvs;
    size_t vertexCount;
    GLuint vao;
    GLuint vertexBuff;
    GLuint uvBuff;
    GLuint normalBuff;

    GLuint texture;
    GLuint texture2;

    GLuint textureID;
    GLuint textureID2;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix, MVP;
    glm::mat4 reflectionMatrix;
    glm::vec3 cameraPos;

    int windowWidth = 1024;
    int windowHeight = 768;

    std::string cubemap_filename[6];

protected:
    void initBuffers();

public:

    GLuint FramebufferName;
    GLuint renderbuffer;
    GLuint isTwoTex;

    Object();
    explicit Object(const std::string& objPath);

    ~Object();

    bool loadTexture( GLuint shaderID, const std::string& texturePath, GLchar *name, int texParam = 0);
    bool loadTexture2( GLuint shaderID, const std::string& texturePath, GLchar *name, int texParam = 0);

    void setTexture(GLuint textureID, GLuint texture);

    bool initFromArray(const std::vector<glm::vec3>& vertices, std::vector<glm::vec3> normals,
                        std::vector<glm::vec2> uvs, GLuint shederID, const std::string& texturePath, GLchar *name);

    bool initFromArray(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                        std::vector<glm::vec2> uvs);

    bool initFromFile(const std::string& path, GLuint shaderID, const std::string& texturePath, GLchar *name);
    bool initFromFile(const std::string& path);

    void translate(glm::vec3 vector);

    void rotate(glm::vec3 vector,float angle);

    void scale(glm::vec3 vector);

    void draw(GLuint MatrixID, GLuint ViewMatrixID, GLuint ModelMatrixID, bool reflected = false, int numberTex = 0);

    void setProjectionMatrix(glm::mat4 matrix) { projectionMatrix = matrix; }

    void setViewMatrix(glm::mat4 matrix) { viewMatrix = matrix; }

    void setModelMatrix(glm::mat4 matrix) { modelMatrix = matrix; }

    void setPositionCamera(GLuint shaderID, glm::vec3 CameraPosition) { cameraPos = CameraPosition;
        glUniform3fv(glGetUniformLocation(shaderID, "CameraPosition"), 1, &cameraPos[0]);}

    glm::mat4 getModelMatrix(){return modelMatrix;}

    glm::mat4 getReflectionMatrix() {return reflectionMatrix;}

    void setReflectionVector(glm::vec3 newVector);

    bool Cubemap(GLuint shaderID, GLchar *name, int texParam);

    void CubemapTexture(
                        const std::string& PosXFilename,
                        const std::string& NegXFilename,
                        const std::string& PosYFilename,
                        const std::string& NegYFilename,
                        const std::string& PosZFilename,
                        const std::string& NegZFilename);

    void drawSkybox(GLuint MatrixID, GLuint ViewMatrixID,GLuint ModelMatrixID);
    void drawMapModel(GLuint MatrixID, GLuint ViewMatrixID, GLuint ModelMatrixID);
};

#endif
