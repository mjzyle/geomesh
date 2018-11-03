#define _CRT_SECURE_NO_WARNINGS

#ifndef MESH3_H
#define MESH3_H

#include <iostream>
#include <fstream>
#include "Vector.cpp"
#include "Face3.cpp"
using namespace std;

class Mesh3 {

public:

	Vector *vert_list;
	Face3 *face_list;
	int verts = 0, faces = 0;
	int geo_height = 1;
	char* name;

	Mesh3() {

	}

	Mesh3(char* file_name) {
		name = file_name;
		load_mesh(file_name);
	}

	Mesh3(Vector *verts, Face3 *faces) {
		vert_list = verts;
		face_list = faces;
	}

	void load_mesh(char* file_name) {
		// Load the mesh from the file name
		FILE *fp;
		fp = fopen(file_name, "r");

		if (fp == NULL) {
			printf("Cannot open file %s\n!", file_name);
			exit(0);
		}

		// Count vertices and faces
		float x, y, z;
		char letter;

		while (!feof(fp)) {
			fscanf(fp, "%c %f %f %f\n", &letter, &x, &y, &z);

			if (letter == 'v') {
				verts++;
			}
			else {
				faces++;
			}
		}

		fclose(fp);

		printf("Vertices: %d\n", verts);
		printf("Faces: %d\n", faces);

		// Allocate space for vertex/face lists
		vert_list = (Vector *)malloc(sizeof(Vector) * verts);
		face_list = (Face3 *)malloc(sizeof(Face3) * faces);

		fp = fopen(file_name, "r");

		// Read and store vertices
		for (int i = 0; i < verts; i++) {
			fscanf(fp, "%c %f %f %f\n", &letter, &x, &y, &z);
			vert_list[i].x = x;
			vert_list[i].y = y;
			vert_list[i].z = z;
		}

		// Read and store faces
		int ix, iy, iz;
		for (int i = 0; i < faces; i++) {
			fscanf(fp, "%c %d %d %d\n", &letter, &ix, &iy, &iz);
			face_list[i].a = vert_list[ix - 1];
			face_list[i].b = vert_list[iy - 1];
			face_list[i].c = vert_list[iz - 1];
		}

		fclose(fp);
	}

	// Save the current mesh as a .obj file
	void save_mesh(char* name) {
		ofstream file;
		file.open(name);
		
		// Write the vertices
		for (int i = 0; i < verts; i++) {
			file << "v ";
			file << vert_list[i].x;
			file << " ";
			file << vert_list[i].y;
			file << " ";
			file << vert_list[i].z;
			file << "\n";
		}

		// Write the faces
		for (int i = 0; i < faces; i++) {
			int x = 0, y = 0, z = 0;
			while (!vert_list[x].equals(face_list[i].a)) {
				x++;
			}
			while (!vert_list[y].equals(face_list[i].b)) {
				y++;
			}
			while (!vert_list[z].equals(face_list[i].c)) {
				z++;
			}

			file << "f ";
			file << x + 1;
			file << " ";
			file << y + 1;
			file << " ";
			file << z + 1;
			file << "\n";
		}

		file.close();
		printf("Mesh saved successfully\n");
	}

	// Create a geodesic version of the current mesh
	void geodesic(Mesh3 m) {
		Vector *temp_vert_list = (Vector *)malloc(sizeof(Vector) * verts * 3);
		Face3 *temp_face_list = (Face3 *)malloc(sizeof(Face3) * faces * 3);

		// Determine and save geodesic faces (i tracks faces, j tracks vertices)
		int i = 0, j = 0;
		while (i < faces * 3) {
			Vector center = face_list[i].center;
			Vector normal = face_list[i].normal;
			Vector height = center;

			normal.scale(geo_height);
			height.add(normal);

			// Add face 1
			temp_vert_list[  j] = face_list[i].a;
			temp_vert_list[++j] = face_list[i].b;
			temp_vert_list[++j] = height;
			temp_face_list[  i] = Face3(face_list[i].a, face_list[i].b, height);
			i++;
			j++;

			// Add face 2
			temp_vert_list[  j] = face_list[i - 1].b;
			temp_vert_list[++j] = face_list[i - 1].c;
			temp_vert_list[++j] = height;
			temp_face_list[  i] = Face3(face_list[i - 1].b, face_list[i - 1].c, height);
			i++;
			j++;

			// Add face 3
			temp_vert_list[  j] = face_list[i - 2].a;
			temp_vert_list[++j] = face_list[i - 2].c;
			temp_vert_list[++j] = height;
			temp_face_list[  i] = Face3(face_list[i - 2].a, face_list[i - 2].c, height);
			i++;
			j++;
		}

		m = Mesh3(temp_vert_list, temp_face_list);
	}

	void geodesic() {                                                                   // Overwrite current mesh
		Vector *temp_vert_list = (Vector *)malloc(sizeof(Vector) * verts * 3);
		Face3 *temp_face_list = (Face3 *)malloc(sizeof(Face3) * faces * 3);

		// Determine and save geodesic faces (i tracks faces, j tracks vertices)
		int i = 0, j = 0;
		while (i < faces * 3) {
			Vector center = face_list[i].center;
			Vector normal = face_list[i].normal;
			Vector height = center;

			normal.scale(geo_height);
			height.add(normal);

			// Add face 1
			temp_vert_list[j] = face_list[i].a;
			temp_vert_list[++j] = face_list[i].b;
			temp_vert_list[++j] = height;
			temp_face_list[i] = Face3(face_list[i].a, face_list[i].b, height);
			i++;
			j++;

			// Add face 2
			temp_vert_list[j] = face_list[i - 1].b;
			temp_vert_list[++j] = face_list[i - 1].c;
			temp_vert_list[++j] = height;
			temp_face_list[i] = Face3(face_list[i - 1].b, face_list[i - 1].c, height);
			i++;
			j++;

			// Add face 3
			temp_vert_list[j] = face_list[i - 2].a;
			temp_vert_list[++j] = face_list[i - 2].c;
			temp_vert_list[++j] = height;
			temp_face_list[i] = Face3(face_list[i - 2].a, face_list[i - 2].c, height);
			i++;
			j++;
		}

		vert_list = temp_vert_list;
		face_list = temp_face_list;
	}

	// Adjust the height parameter for an existing geodesic mesh
	void change_height(int mod, Mesh3 m) {
		geo_height += mod;
		geodesic(m);
	}

	void change_height(int mod) {
		geo_height += mod;
		geodesic();
	}
};

#endif