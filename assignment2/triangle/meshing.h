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

class GravitationalForce{
public:
	Force force;
	GravitationalForce();
	Force computeForce();
};

class ElectromagnetForce{
public:
	float k;
	Force force;
	ElectromagnetForce();
	Force computeForce(Position p1, Position p2, float q1, float q2);
	Force computeForce(Node* n1, Node* n2);
};

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
private:
	Velocity velocity;
	vector<Triangle*> triangles;
public:
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
private:
	vector<Node*> nodes;
public:
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