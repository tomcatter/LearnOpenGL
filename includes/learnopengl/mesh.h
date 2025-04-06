//
// Created by tom on 2025/3/31.
//

#ifndef LEARNOPENGL_MESH_H
#define LEARNOPENGL_MESH_H

#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;  // 位置
    glm::vec3 Normal;    // 法线
    glm::vec2 TexCoords; // 纹理坐标
    glm::vec3 Tangent;   // 切线
    glm::vec3 Bitangent; // 双切线
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


class Mesh {

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->setupMesh();
    }


    void Draw(const Shader &shader) {

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;

            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            } else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            } else if (name == "texture_normal") {
                number = std::to_string(normalNr++);
            } else if (name == "texture_height") {
                number = std::to_string(heightNr++);
            }
            // 设置纹理采样器到对应的纹理单元
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // 绑定纹理
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 重置激活纹理单元
        glActiveTexture(GL_TEXTURE0);
    }


private:

    void setupMesh() {
        // 创建顶点数组和缓冲对象
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // 上传数据到顶点缓冲
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // 上传索引缓冲数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // 设置顶点属性
        // 前三位表示顶点坐标
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

        // 顶点法线
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));

        // 顶点纹理坐标
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));

        // 顶点切线方向
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Tangent));

        // 顶点双切线
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Bitangent));

        // 骨骼id
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *) offsetof(Vertex, m_BoneIDs));

        // 权重
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, m_weights));

        glBindVertexArray(0);
    }

private:
    unsigned int VBO;
    unsigned int EBO;


};

#endif //LEARNOPENGL_MESH_H
