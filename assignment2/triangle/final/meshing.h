#include <iostream>
#include <vector>
#include <set>
#include <algorithm>    // std::sort
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include "glm/mat2x2.hpp"
#include "glm/matrix.hpp"
using namespace std;
#define REAL float


typedef glm::vec2 Position;
typedef glm::vec2 Force;
typedef glm::vec2 Velocity;
class Triangle;
class Node;
class Mesh;

Force getGravitationalForce(Node* n);

class Stress{
	float lambda;
	float mu;
	glm::mat2 stress;
	Stress();
	Force computeForce(Node* n);//in turn call strain.
	glm::mat2 computeStress(Node *n);
};

class ViscousStress{
	float lambda;
	float mu;
	glm::mat2 stress;
	ViscousStress();
	Force computeForce(Node* n);//in turn call strain.
	glm::mat2 computeStr(Node* n);//in turn call strain.
};



class Node{

public:
	Velocity velocity;
	vector<Triangle*> triangles;
	Force force;
	int id;
	float charge;
	Position pos;
	Node();
	Node(float x, float y);
	Node(Position pos);
	void updatePosition(Position p);
	void updatePosition(float t);
	Position getPos();
	Velocity getVelocity();
	void updateVelocity(Velocity vec);
	void updateVelocity(Force f, float t);
	void initTriangles(vector<Triangle*> triangles);
	void addTriangle(Triangle* triangle);
	vector<Triangle*> getTriangles();
	vector<Node*> getNeighbours();
	void print();
};


class Triangle{

public:
	vector<Node*> nodes;
	int id;
	float volume;
	Triangle();
	Triangle(Node* n1, Node* n2, Node* n3);
	Triangle(vector<Node*>nodes);
	float getVolume();
	Node* getNode(int i);
	void updateVolume();
	vector<Node*> getNodes();
	void updateNodes(vector<Node*> nodes);
	void updateNodes(Node* n1, Node* n2, Node* n3);
	vector<Triangle*> getNeighbours();
	bool inTriangle(Node n);
	bool inTriangle(Node* n);
	bool inTriangle(Position p);
	void print();
};


class Mesh{

public:
	vector<Node*> nodes;
	vector<Triangle*> triangles;
	float volume;
	float density;
	float time;
	float timestep;
	float mass;

	Mesh();
	Mesh(vector<Node*>nodes);
	Mesh(vector<Node*>nodes,vector<Triangle*> triangles);
	float getVolume();
	float getVolumeofTriangles();

	void computeForcesOnNodes();
	void updateVelocityOfAllNodes();
	void updatePositionOfAllNodes();
	void updateTriangles();
	void removeNode(Node* n);
	void shiftPropertiesOverNeighbours();
	void shiftPropertiesOverNeighbourNodes();
	void shiftPropertiesOverNeighboursTriangles();

	int getNumNodes();
	int getNumTriangles();
	void genMesh();
	void getPositions(std::vector<glm::vec4> & v_positions,std::vector<glm::vec4> v_colors);
	void updateMesh();
	void print();
};

void report(struct triangulateio *io,
int markers,
int reporttriangles,
int reportneighbors,
int reportsegments,
int reportedges,
int reportnorms);