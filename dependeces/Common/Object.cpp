#include "Object.h"

#include <utility>

Object::Object() {
    modelMatrix = glm::mat4(1.0);
    setReflectionVector(glm::vec3(1.0f, -1.0f, 1.0f));
    this->textureID = 0;
    this->textureID2 = 1;
}

Object::Object(const std::string &objPath) {
    modelMatrix = glm::mat4(1.0);
    setReflectionVector(glm::vec3(1.0f, -1.0f, 1.0f));
    this->textureID = 0;
    this->textureID2 = 1;
    initFromFile(objPath);
}

Object::~Object() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexBuff);
    glDeleteBuffers(1, &uvBuff);
    glDeleteBuffers(1, &normalBuff);
}


void Object::translate(glm::vec3 vector) {
    modelMatrix = glm::translate(modelMatrix, vector);
}

void Object::scale(glm::vec3 vector) {
    modelMatrix = glm::scale(modelMatrix, vector);
}

void Object::setTexture(GLuint textureID, GLuint texture) {
    this->textureID=textureID;
    this->texture=texture;
}
void Object::rotate(glm::vec3 vector, float angle) {
    modelMatrix = glm::rotate(modelMatrix,glm::radians(angle),vector);
}
bool Object::initFromArray(const std::vector<glm::vec3>& vertices, std::vector<glm::vec3> normals,
                                      std::vector<glm::vec2> uvs, GLuint shaderID, const std::string& texturePath,
                                      GLchar *name) {
    bool check = true;
    this->vertices = vertices;
    this->normals = std::move(normals);
    this->uvs = std::move(uvs);
    this->vertexCount = vertices.size();
    this->textureID = glGetUniformLocation(shaderID, name);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        check = false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    initBuffers();

    return check;
}

bool Object::initFromFile(const std::string& path, GLuint shaderID, const std::string& texturePath, GLchar *name) {
    bool check = true;
    loadOBJ(path.c_str(), vertices, uvs, normals);
    this->vertexCount = vertices.size();
    this->textureID = glGetUniformLocation(shaderID, name);
    this->textureID2 = glGetUniformLocation(shaderID, name);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        check = false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    initBuffers();

    return check;
}


bool Object::loadTexture(GLuint shaderID, const std::string& texturePath, GLchar *name, int texParam) {
    bool check = true;


    this->textureID = glGetUniformLocation(shaderID, name);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        check = false;
    }
    if(texParam==0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    }
    else if(texParam==1){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    }
    else if(texParam==2){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    }
    else if(texParam==3){
        float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    }else if(texParam==4) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glGenFramebuffers(1, &FramebufferName);
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

        glGenRenderbuffers(1, &renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 256, 256);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture,0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
        // Set the list of draw buffers.
        //DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        //glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            return false;
    }else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    return check;
}

bool Object::loadTexture2(GLuint shaderID, const std::string& texturePath, GLchar *name, int texParam) {
    bool check = true;

    this->textureID2 = glGetUniformLocation(shaderID, name);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        check = false;
    }
    if(texParam==0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);

    }
    else if(texParam==1){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);

    }
    else if(texParam==2){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);

    }
    else if(texParam==3){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);

    }else if(texParam==4){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

        glGenRenderbuffers(1, &renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
        // Set the list of draw buffers.
        //DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        //glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            return false;
    }else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    return check;
}

bool Object::Cubemap(GLuint shaderID, GLchar *name, int texParam) {
    bool check = true;

    this->textureID = glGetUniformLocation(shaderID, name);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (int i = 0; i < 6; i++) {
        int width, height, nchan;
        unsigned char* dt = stbi_load(cubemap_filename[i].c_str(), &width, &height, &nchan, 0);
        std::cout<<cubemap_filename[i].c_str()<<std::endl;
        std::cout<<GL_TEXTURE_CUBE_MAP_POSITIVE_X + i<<std::endl;
        if (!dt) {
            puts("Cannot load Texture");
            check = false;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, dt);
        stbi_image_free(dt);
    }
    if(texParam==0) {
        // Filtrowanie BEZ mipmap:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // Brak powielania - obcinanie do krawędzi na trzech współrzędnych:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }else{
        // Filtrowanie z mipmap:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // Brak powielania - obcinanie do krawędzi na trzech współrzędnych:
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    return check;
}

bool Object::initFromArray(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                                      std::vector<glm::vec2> uvs) {
    bool check = true;
    this->vertices = vertices;
    this->normals = normals;
    this->uvs = uvs;
    this->vertexCount = vertices.size();

    initBuffers();
    return check;

}

bool Object::initFromFile(const std::string& path) {
    bool check = true;
    loadOBJ(path.c_str(), vertices, uvs, normals);
    this->vertexCount = vertices.size();

    initBuffers();
    return check;


}

void Object::drawSkybox(GLuint MatrixID,GLuint ViewMatrixID, GLuint ModelMatrixID) {
//    glUniform1i(textureID, 0);
//    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

    glDepthMask(GL_FALSE);
    glBindVertexArray(vao);

    MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDepthMask(GL_TRUE);
}

void Object::drawMapModel(GLuint MatrixID, GLuint ViewMatrixID, GLuint ModelMatrixID) {


    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

    glBindVertexArray(vao);

    MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Object::draw(GLuint MatrixID, GLuint ViewMatrixID, GLuint ModelMatrixID, bool reflected, int numberTex) {
    if(1>=numberTex) {
        glUniform1i(textureID, 0);
        glUniform1i(isTwoTex, false);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }else if(0==numberTex)
    {
        glUniform1i(textureID, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    }else{
    glUniform1i(textureID, 0);
    glUniform1i(textureID, 1);
    glUniform1i(isTwoTex, true);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    }

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

    glBindVertexArray(vao);

    MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
//

void Object::initBuffers() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *) 0 // array buffer offset
    );

    glGenBuffers(1, &uvBuff);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuff);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1,        // attribute
            2,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *) 0 // array buffer offset
    );

    glGenBuffers(1, &normalBuff);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuff);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
            2,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *) 0 // array buffer offset
    );

    glBindVertexArray(0);
}

void Object::setReflectionVector(glm::vec3 newVector) {
    reflectionMatrix = glm::scale(modelMatrix, newVector);
}

void Object::CubemapTexture(const std::string &PosXFilename, const std::string &NegXFilename,
                            const std::string &PosYFilename, const std::string &NegYFilename,
                            const std::string &PosZFilename, const std::string &NegZFilename) {

    cubemap_filename[0] = PosXFilename;
    cubemap_filename[1] = NegXFilename;
    cubemap_filename[2] = PosYFilename;
    cubemap_filename[3] = NegYFilename;
    cubemap_filename[4] = PosZFilename;
    cubemap_filename[5] = NegZFilename;


}

bool LoadTexture(GLuint shaderID, const std::string& texturePath, GLchar *name,GLint out[2]){
    GLuint textureId ,texture;
    textureId = glGetUniformLocation(shaderID, name);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        return  false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    out[0]=textureId;
    out[1]=texture;
    return true;
}
