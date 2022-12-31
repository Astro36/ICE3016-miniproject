#define _CRT_SECURE_NO_WARNINGS
#include "object.h"
#include <GL/freeglut.h>
#include <cstddef>
#include <cstdio>
#include <string>

Object::Object(const std::string& filename) {
    FILE* fp = std::fopen(filename.c_str(), "r");
    if (!fp) {
        std::perror("File opening failed");
    }
    while (true) {
        char lineHeader[128]; // Expected max length of 1 line = 128 bytes;
        int line = fscanf(fp, "%s", lineHeader);
        if (line == EOF) {
            break;
        }
        if (strcmp(lineHeader, "v") == 0) {
            float x, y, z;
            fscanf(fp, "%f %f %f\n", &x, &y, &z);
            vertices.push_back({ x, y, z });
        } else if (strcmp(lineHeader, "vt") == 0) {
            float x, y;
            fscanf(fp, "%f %f\n", &x, &y);
            textures.push_back({ x, y });
        } else if (strcmp(lineHeader, "vn") == 0) {
            float x, y, z;
            fscanf(fp, "%f %f %f\n", &x, &y, &z);
            normals.push_back({ x, y, z });
        } else if (strcmp(lineHeader, "f") == 0) {
            std::size_t v[3], vt[3], vn[3];
            int matches = fscanf(fp, "%zu/%zu/%zu %zu/%zu/%zu %zu/%zu/%zu\n", &v[0], &vt[0], &vn[0], &v[1], &vt[1], &vn[1], &v[2], &vt[2], &vn[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser, try exporting with other options\n");
                break;
            }
            vertex_indices.push_back(v[0]);
            vertex_indices.push_back(v[1]);
            vertex_indices.push_back(v[2]);
            texture_indices.push_back(vt[0]);
            texture_indices.push_back(vt[1]);
            texture_indices.push_back(vt[2]);
            normal_indices.push_back(vn[0]);
            normal_indices.push_back(vn[1]);
            normal_indices.push_back(vn[2]);
        } else if (strcmp(lineHeader, "o") == 0) {
            char buf[128];
            fscanf(fp, "%s", buf);
            std::string tmp_name = std::string(buf);
            name = tmp_name;
        }
    }
    fclose(fp);
}

void Object::render() const {
    glBegin(GL_TRIANGLES);
    for (std::size_t n = 0; n < vertex_indices.size(); n += 3) {
        glTexCoord2f(textures[texture_indices[n] - 1].x, textures[texture_indices[n] - 1].y);
        glNormal3f(normals[normal_indices[n] - 1].x, normals[normal_indices[n] - 1].y, normals[normal_indices[n] - 1].z);
        glVertex3f(vertices[vertex_indices[n] - 1].x, vertices[vertex_indices[n] - 1].y, vertices[vertex_indices[n] - 1].z);

        glTexCoord2f(textures[texture_indices[n + 1] - 1].x, textures[texture_indices[n + 1] - 1].y);
        glNormal3f(normals[normal_indices[n + 1] - 1].x, normals[normal_indices[n + 1] - 1].y, normals[normal_indices[n + 1] - 1].z);
        glVertex3f(vertices[vertex_indices[n + 1] - 1].x, vertices[vertex_indices[n + 1] - 1].y, vertices[vertex_indices[n + 1] - 1].z);

        glTexCoord2f(textures[texture_indices[n + 2] - 1].x, textures[texture_indices[n + 2] - 1].y);
        glNormal3f(normals[normal_indices[n + 2] - 1].x, normals[normal_indices[n + 2] - 1].y, normals[normal_indices[n + 2] - 1].z);
        glVertex3f(vertices[vertex_indices[n + 2] - 1].x, vertices[vertex_indices[n + 2] - 1].y, vertices[vertex_indices[n + 2] - 1].z);
    }
    glEnd();
}
