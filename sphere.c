#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
	long double x;
	long double y;
	long double z;
} Vertex;

typedef struct
{
	Vertex v1;
	Vertex v2;
	Vertex v3;
} Triangle;

typedef struct
{
	int v1;
	int v2;
	int v3;
} Face;

const int VSIZE = sizeof(Vertex);
const int TSIZE = sizeof(Triangle);
const int FSIZE = sizeof(Face);


int compare(Vertex v1, Vertex v2)
{
	if (v1.x != v2.x)
		if (v1.x > v2.x)
			return 1;
		else
			return -1;
	else if (v1.y != v2.y)
		if (v1.y > v2.y)
			return 1;
		else
			return -1;
	else if (v1.z != v2.z)
		if (v1.z > v2.z)
			return 1;
		else
			return -1;
	else
		return 0;
}

void sort(Vertex list[], int e1, int e2)
{
	int m1 = (e1 + e2) / 2, m2 = m1 + 1;

	if (e1 != m1)
		sort(list, e1, m1), sort(list, m2, e2);

	Vertex temp[e2 - e1 + 1];
	int i = 0, i1 = e1, i2 = m2;

	while (i1 <= m1 && i2 <= e2)
		if (compare(list[i1], list[i2]) <= 0)
			temp[i++] = list[i1++];
		else
			temp[i++] = list[i2++];

	if (i1 > m1)
		while (i2 <= e2)
			temp[i++] = list[i2++];
	else
		while (i1 <= m1)
			temp[i++] = list[i1++];

	for (i = e1; i <= e2; ++i)
		list[i] = temp[i - e1];
}

int find(Vertex target, Vertex list[], int e1, int e2)
{
	int m1 = (e1 + e2) / 2, m2 = m1 + 1;

	if (compare(target, list[m1]) == 0)
		return m1;
	else if (e1 == e2)
		return -1;
	else if (compare(target, list[m1]) < 0)
		return find(target, list, e1, m1);
	else
		return find(target, list, m2, e2);
}

void removeDuplicates(Vertex vertices[], int vSize)
{
	Vertex temp[vSize / 6 + 2];
	temp[0] = vertices[0];
	int i, j;

	for (i = 1, j = 1; i != vSize; ++i)
		if (compare(vertices[i], temp[j - 1]) != 0)
			temp[j++] = vertices[i];

	memcpy(vertices, temp, VSIZE * j);
}

Vertex middle(Vertex v1, Vertex v2)
{
	Vertex point;
	long double x1, y1, z1, x2, y2, z2, xm, ym, zm, size;
	x1 = v1.x, y1 = v1.y, z1 = v1.z, x2 = v2.x, y2 = v2.y, z2 = v2.z;
	xm = (x1 + x2) / 2, ym = (y1 + y2) / 2, zm = (z1 + z2) / 2;
	size = sqrtl(xm * xm + ym * ym + zm * zm);
	point.x = xm / size, point.y = ym / size, point.z = zm / size;

	return point;
}

void divide(Triangle triangles[], int tSize)
{
	Vertex m1, m2, m3;
	Triangle temp, newTriangles[4 * tSize];
	int i, j;

	for (i = 0, j = 0; i != tSize; ++i)
		m1 = middle(triangles[i].v1, triangles[i].v2),
		m2 = middle(triangles[i].v2, triangles[i].v3),
		m3 = middle(triangles[i].v3, triangles[i].v1),
		temp.v1 = triangles[i].v1, temp.v2 = m1, temp.v3 = m3,
		newTriangles[j++] = temp,
		temp.v1 = triangles[i].v2, temp.v2 = m2, temp.v3 = m1,
		newTriangles[j++] = temp,
		temp.v1 = triangles[i].v3, temp.v2 = m3, temp.v3 = m2,
		newTriangles[j++] = temp,
		temp.v1 = m1, temp.v2 = m2, temp.v3 = m3,
		newTriangles[j++] = temp;

	memcpy(triangles, newTriangles, TSIZE * j);
}

void setVertices(Triangle triangles[], int tSize, Vertex vertices[])
{
	int i, j;

	for (i = 0, j = 0; i != tSize; ++i)
		vertices[j++] = triangles[i].v1,
		vertices[j++] = triangles[i].v2,
		vertices[j++] = triangles[i].v3;

	sort(vertices, 0, j - 1);
	removeDuplicates(vertices, j);
}

void setFaces(Vertex vertices[], int vSize,
	Triangle triangles[], int tSize, Face faces[])
{
	Face temp;
	int i;

	for (i = 0; i != tSize; ++i)
		temp.v1 = find(triangles[i].v1, vertices, 0, vSize - 1),
		temp.v2 = find(triangles[i].v2, vertices, 0, vSize - 1),
		temp.v3 = find(triangles[i].v3, vertices, 0, vSize - 1),
		faces[i] = temp;
}

void changeRadius(Vertex vertices[], int vSize, long double radius)
{
	int i;

	for (i = 0; i != vSize; ++i)
		vertices[i].x *= radius,
		vertices[i].y *= radius,
		vertices[i].z *= radius;
}

void save(Vertex vertices[], int vSize, Face faces[], int fSize, int times)
{
	char filename[21];
	sprintf(filename, "Sphere %d.py", times);
	FILE *file = fopen(filename, "w");
	fprintf(file, "import bpy\n\n\n");
	fprintf(file, "vertices = []\t# Total %d\n\n", vSize);
	double x, y, z;
	int v1, v2, v3, i;

	for (i = 0; i != vSize; ++i)
		x = vertices[i].x, y = vertices[i].y, z = vertices[i].z,
		fprintf(file, "vertices.append((%.16g, %.16g, %.16g))\n", x, y, z);

	fprintf(file, "\n\nfaces = []\t# Total %d\n\n", fSize);

	for (i = 0; i != fSize; ++i)
		v1 = faces[i].v1, v2 = faces[i].v2, v3 = faces[i].v3,
		fprintf(file, "faces.append((%d, %d, %d))\n", v1, v2, v3);

	fprintf(file, "\n\nmesh = bpy.data.meshes.new(\"sphere\")\n");
	fprintf(file, "object = bpy.data.objects.new(\"sphere\", mesh)\n\n");
	fprintf(file, "object.location = bpy.context.scene.cursor_location\n");
	fprintf(file, "bpy.context.scene.objects.link(object)\n\n");
	fprintf(file, "mesh.from_pydata(vertices, [], faces)\n");
	fprintf(file, "mesh.update(calc_edges = True)\n");
	fclose(file);
}

int faceCount(int seed, int times)
{
	int i;

	for (i = 0; i != times; ++i)
		seed *= 4;

	return seed;
}

int vertexCount(int seed, int times)
{
	int i;

	for (seed /= 2, i = 0; i != times; ++i)
		seed *= 4;

	return seed + 2;
}


void main()
{
	Vertex v1, v2, v3, v4, v5, v6;
	v1.x = 1, v1.y = 0, v1.z = 0;
	v2.x = 0, v2.y = 1, v2.z = 0;
	v3.x = 0, v3.y = 0, v3.z = 1;
	v4.x = -1, v4.y = 0, v4.z = 0;
	v5.x = 0, v5.y = -1, v5.z = 0;
	v6.x = 0, v6.y = 0, v6.z = -1;

	int times;
	printf("Divisions: ");
	scanf("%d", &times);

	if (times < 0 || times > 8)
		exit(0);

	double radius;
	printf("Radius: ");
	scanf("%lf", &radius);

	int fSize = faceCount(8, times), vSize = 3 * fSize, i;
	Vertex *vertices = (Vertex *)malloc(VSIZE * vSize);
	Triangle *triangles = (Triangle *)malloc(TSIZE * fSize), temp;
	Face *faces = (Face *)malloc(FSIZE * fSize);

	temp.v1 = v1, temp.v2 = v2, temp.v3 = v3, triangles[0] = temp;
	temp.v1 = v2, temp.v2 = v4, temp.v3 = v3, triangles[1] = temp;
	temp.v1 = v4, temp.v2 = v5, temp.v3 = v3, triangles[2] = temp;
	temp.v1 = v5, temp.v2 = v1, temp.v3 = v3, triangles[3] = temp;
	temp.v1 = v6, temp.v2 = v2, temp.v3 = v1, triangles[4] = temp;
	temp.v1 = v6, temp.v2 = v4, temp.v3 = v2, triangles[5] = temp;
	temp.v1 = v6, temp.v2 = v5, temp.v3 = v4, triangles[6] = temp;
	temp.v1 = v6, temp.v2 = v1, temp.v3 = v5, triangles[7] = temp;

	for (i = 0; i != times; ++i)
		divide(triangles, faceCount(8, i));

	setVertices(triangles, fSize, vertices);
	printf("\nNumber of Vertices: %d\n", fSize / 2 + 2);
	setFaces(vertices, fSize / 2 + 2, triangles, fSize, faces);
	printf("Number of Faces: %d\n\n", fSize);
	changeRadius(vertices, fSize / 2 + 2, radius);

	save(vertices, vertexCount(8, times), faces, fSize, times);
	free(vertices), free(triangles), free(faces);
	system("pause");
}