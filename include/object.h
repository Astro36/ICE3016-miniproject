#ifndef OBJECT_H
#define OBJECT_H

#include <cstddef>
#include <string>
#include <vector>

struct Vec3f {
	float x;
	float y;
	float z;
};

struct Vec2f {
	float x;
	float y;
};

class Object {
public:
	Object(const std::string& filename);
	void draw() const;

private:
	std::string name;
	std::vector<Vec3f> vertices; // v
	std::vector<Vec2f> textures; // vt
	std::vector<Vec3f> normals; // vn
	std::vector<std::size_t> vertex_indices; // f
	std::vector<std::size_t> texture_indices; // f
	std::vector<std::size_t> normal_indices; // f
};

#endif // OBJECT_H
