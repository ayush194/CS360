//Code Credits: Calvin1602@github.com

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>

void parseMTL(char*);
void splitByDelim(char*, char, int*, int*, int*);

typedef struct Material {
	char name[50];
	unsigned int ns;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	unsigned int d;
	unsigned int illum;
	char map_kd[100];
	char map_ks[100];
} Material;

typedef struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	unsigned int mtl_idx;
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 u, unsigned int mi) : 
		position(p), normal(n), uv(u), mtl_idx(mi) {}
} Vertex;

bool loadOBJ(const char*, 
	std::vector<Vertex>&,
	std::vector<Material*>&
);

#endif
