#include "meshing.h"

int main(){

	float radius = 10.0;
	float center_x = 0;
	float center_y = 0;
	float partitions = 20;
	vector<Node*> nodes;
	vector<Triangle*> triangles;
	Node* n =  new Node(0,0);
	nodes.push_back(n);
	nodes[0]->id = 0;
	float angle = 2*3.1415/partitions;
	for(int i=0;i<partitions;i++){
		Node* n1 = new Node(center_x + (radius/2)*cos(i*angle),center_y + (radius/2)*sin(i*angle));
		nodes.push_back(n1);
		nodes[2*i+1]->id = 2*i+1;
		n1 = new Node(center_x + (radius)*cos(i*angle),center_y + (radius)*sin(i*angle));
		nodes.push_back(n1);
		nodes[2*i+2]->id = 2*i+2;
	}
	for(int i=0;i<partitions;i++){
		Triangle* t = new Triangle(nodes[0],nodes[(2*i+1)%nodes.size()],nodes[(2*i+3)%nodes.size()]);
		triangles.push_back(t);
		nodes[0]->addTriangle(triangles[3*i]);
		nodes[(2*i+1)%nodes.size()]->addTriangle(triangles[3*i]);
		nodes[(2*i+3)%nodes.size()]->addTriangle(triangles[3*i]);
		triangles[3*i]->id =3*i;

		t = new Triangle(nodes[(2*i+1)%nodes.size()],nodes[(2*i+2)%nodes.size()],nodes[(2*i+3)%nodes.size()]);
		triangles.push_back(t);
		nodes[(2*i+1)%nodes.size()]->addTriangle(triangles[3*i+1]);
		nodes[(2*i+2)%nodes.size()]->addTriangle(triangles[3*i+1]);
		nodes[(2*i+3)%nodes.size()]->addTriangle(triangles[3*i+1]);
		triangles[3*i+1]->id =3*i+1;

		t = new Triangle(nodes[(2*i+2)%nodes.size()],nodes[(2*i+3)%nodes.size()],nodes[(2*i+4)%nodes.size()]);
		triangles.push_back(t);
		nodes[(2*i+2)%nodes.size()]->addTriangle(triangles[3*i+2]);
		nodes[(2*i+3)%nodes.size()]->addTriangle(triangles[3*i+2]);
		nodes[(2*i+4)%nodes.size()]->addTriangle(triangles[3*i+2]);
		triangles[3*i+2]->id =3*i+2;
	}	
	Mesh mesh(nodes,triangles);
	mesh.print();

	//,triangles);
	// mesh.genMesh();// will construct triangles.
}