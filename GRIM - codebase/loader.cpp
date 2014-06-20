
#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <glload/gl_3_3.h>
#include <glutil/glutil.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include "../framework/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class OBJ{

public:
	std::vector < glm::vec3 > out_vertices;
	std::vector < glm::vec2 > out_uvs;
	std::vector < glm::vec3 > out_normals;
	GLuint vbo;
	GLuint vao;

	OBJ(const char * path):vao(0){
		this->load(path);
	}

	bool load(const char * path){
		//tymczasowe zmienne:
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;
		FILE * file = fopen(path, "r");
		int count = 0;
		if( file == NULL ){
			printf("Impossible to open the file !\n");
			return false;
		}
		while( 1 ){
			count++;
			printf("%d\n", count);
		    char lineHeader[128];
		    // read the first word of the line
		    int res = fscanf(file, "%s", lineHeader);
			printf("%s\n", lineHeader);
		    if (res == EOF)
		        break; // EOF = End Of File. Quit the loop.
		 
		    // else : parse lineHeader
			if ( strcmp( lineHeader, "v" ) == 0 ){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
				temp_vertices.push_back(vertex);
			}else if ( strcmp( lineHeader, "vt" ) == 0 ){
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y );
				temp_uvs.push_back(uv);
			}else if ( strcmp( lineHeader, "vn" ) == 0 ){
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
				temp_normals.push_back(normal);
			}else if ( strcmp( lineHeader, "f" ) == 0 ){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
				if (matches != 9){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices    .push_back(uvIndex[0]);
				uvIndices    .push_back(uvIndex[1]);
				uvIndices    .push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			// For each vertex of each triangle
			for( unsigned int i=0; i<vertexIndices.size(); i++ ){
				unsigned int vertexIndex = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
				out_vertices.push_back(vertex);
			}
		}
		generateBuffers();
		return true;
	}

	void generateBuffers(){
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, getArraySize(), getArrayData(), GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	};

	int getArraySize(){
		return sizeof(float) * getArrayCount();
	}

	int getArrayCount(){
		int size = out_vertices.size() * 3 +  out_uvs.size() * 2 + out_normals.size() * 3 ;
		return size;
	}

	float*  getArrayData(){
		int size = getArrayCount();
		float* temp_array = new float[size];
		int count = 0;
		for(std::vector<glm::vec3 >::iterator it = out_vertices.begin(); it != out_vertices.end(); ++it) {
			temp_array[count]=it->x;
			count++;
			temp_array[count]=it->y;
			count++;
			temp_array[count]=it->z;
			count++;
		}
		for(std::vector<glm::vec2 >::iterator it = out_uvs.begin(); it != out_uvs.end(); ++it) {
			temp_array[count]=it->x;
			count++;
			temp_array[count]=it->y;
			count++;
		}
		for(std::vector<glm::vec3 >::iterator it = out_normals.begin(); it != out_normals.end(); ++it) {
			temp_array[count]=it->x;
			count++;
			temp_array[count]=it->y;
			count++;
			temp_array[count]=it->z;
			count++;
		}
		return temp_array;
	}

	void render(){ 
		if(!vao)
			return;

		glBindVertexArray(vao);

		glBindVertexArray(0);
	}
};

