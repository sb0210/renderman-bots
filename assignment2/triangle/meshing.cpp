#include "meshing.h"
extern "C" 
{

	#include "triangle.h"
}

#define REAL float


GravitationalForce::GravitationalForce(){
	// f.x=0;
	// f.y=-10.0;
}
Force GravitationalForce::computeForce(){
	return force;
}


ElectromagnetForce::ElectromagnetForce(){
	k = 0.005;
}

Force ElectromagnetForce::computeForce(Position p1, Position p2, float q1, float q2){
	Force force;
	// float mag = k*q1*q2/(glm::distance(p1,p2));
	// glm::vec2 unit = glm::normalize(p1 - p2);
	// force = mag*unit;
	return force;
}
Force ElectromagnetForce::computeForce(Node* n1, Node* n2)
{
	return computeForce(n1->pos, n2->pos,n1->charge, n2->charge);
}

Stress::Stress(){
	lambda = 10;
	mu = 10;
}
Force Stress::computeForce(Node* n){
	//using all the neighbours data compute divergence.

}
glm::mat2 Stress::computeStress(Node*n){
	glm::mat2 mat = computeStress(n);
	// mat =  lambda*glm::trace(mat)*glm::mat2(1.0f) + 2*mu*mat;
	return mat;			
}
//in turn call strain.
glm::mat2 computeStrain(Node *n){

	//nahi aata abhi to
	return glm::mat2(1.0f);
}

Node::Node(){

}
Node::Node(float x, float y){
	this->pos.x=x;
	this->pos.y=y;
}
Node::Node(Position pos){
	this->pos = pos;
}
Position Node::getPos(){
	return pos;
}
Velocity Node::getVelocity(){
	return velocity;
}
void Node::updateVelocity(Velocity vec){
	velocity = vec;
}
void Node::initTriangles(vector<Triangle*> triangles){
	this->triangles = triangles;
}
void Node::addTriangle(Triangle* triangle){
	this->triangles.push_back(triangle);
}
vector<Triangle*> Node::getTriangles(){
	return this->triangles;
}
vector<Node*> Node::getNeighbours(){
	vector<Node*> temp;
	set<Node*> temp_set;
	for(int i=0;i<triangles.size();i++){
			for(int j=0;j<3;j++){
			if (this==triangles[i]->getNode(j))continue;
			temp_set.insert(triangles[i]->getNode(i));
		}
	}
	temp.assign(temp_set.begin(),temp_set.end());
	return temp;
}
void Node::print(){
	cout<<"X: "<<pos.x<<", Y: "<<pos.y<<endl;
	cout<<"Neighbouring Triangles"<<endl;
	for(int i=0;i<triangles.size();i++){
		cout<<"\t"<<i<<"\t"<<triangles[i]->id<<endl;
	}
}


Triangle::Triangle(){

}
Triangle::Triangle(Node* n1, Node* n2, Node* n3){
	nodes.resize(3);
	nodes[0] = n1;
	nodes[1] = n2;
	nodes[2] = n3; 
}
Triangle::Triangle(vector<Node*>nodes){
	this->nodes = nodes;
}
Node* Triangle::getNode(int i){
	return nodes[i];
}
float Triangle::getVolume(){
	return volume;
}
void Triangle::updateVolume(){
	Position p1,p2,p3;
	p1 = nodes[0]->getPos();
	p2 = nodes[1]->getPos();
	p3 = nodes[2]->getPos();
	volume = (p1.x*(p2.y - p3.y)+ 
	          p2.x*(p3.y - p1.y) + 
	          p3.x*(p1.y - p2.y))/2;
}
vector<Node*> Triangle::getNodes(){
	return nodes;
}
void Triangle::updateNodes(vector<Node*> nodes){
	nodes=nodes;
	updateVolume();
}
void Triangle::updateNodes(Node* n1, Node* n2, Node* n3){
	nodes[0] = n1;
	nodes[1] = n2;
	nodes[2] = n3;
	updateVolume();
}
vector<Triangle*> Triangle::getNeighbours(){
	set<Triangle*> temp;
	vector<Triangle*> temp_vec;
	for(int i=0;i<3;i++){
		vector<Triangle*> temp_node = nodes[i]->getTriangles();
		for(int j=0;j<temp_node.size();j++){
			if(this==temp_node[j])continue;
			temp.insert(temp_node[j]);
		}
	}
	temp_vec.assign(temp.begin(), temp.end());
	return temp_vec;
}
bool Triangle::inTriangle(Node n){
	return inTriangle(n.getPos());
}
bool Triangle::inTriangle(Node* n){
	return inTriangle(n->getPos());
}
bool Triangle::inTriangle(Position p){
	Position p1 = nodes[0]->pos;
	Position p2 = nodes[1]->pos;
	Position p3 = nodes[2]->pos;
	float alpha = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) /
    ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
	float beta = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) /
   ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
	float gamma = 1.0f - alpha - beta;	
	return (alpha>0 && beta > 0 && gamma >0);
}
void Triangle::print(){
	// cout<<"Triangle Number "<<this->id<<endl;
	// cout<<"Points in triangle"<<endl;
	cout<<"Polygon[{";
	for(int i=0;i<3;i++){
		cout<<"{"<<nodes[i]->pos.x<<","<<nodes[i]->pos.y<<"}";
		if(i<2){
			cout<<",";
		}
	}
	cout<<"}],"<<endl;
}

Mesh::Mesh(){

}
Mesh::Mesh(vector<Node*> nodes){
	this->nodes = nodes;
}
Mesh::Mesh(vector<Node*> nodes,vector<Triangle*> triangles){
	this->nodes = nodes;
	this->triangles = triangles;
}
float Mesh::getVolume(){
	return volume;
}
float Mesh::getVolumeofTriangles(){
	float temp_volume;
	for(int i=0;i<triangles.size();i++){
		temp_volume += triangles[i]->volume;
	}
	return temp_volume;
}
int Mesh::getNumNodes(){
	return nodes.size();
}
int Mesh::getNumTriangles(){
	return triangles.size();
}
void Mesh::print(){
	cout<<"Points\n\n"<<endl;
	for(int i=0;i<nodes.size();i++){
		nodes[i]->print();
	}
	cout<<"Triangles\n\n"<<endl;
	for(int i=0;i<triangles.size();i++){
		triangles[i]->print();
	}
}

void Mesh::updateMesh(){
	//calculate force on all the nodes.
	//calculate new velocity of the nodes.
	//update the position of the nodes.
	//check all the nodes and all the triangles. if a node ovelap with triangle, compute a set
	//perform contraction pass. remove edges if too short.
	//split overlapping elements.
	//with each split, again contract.
	//check the triangles. check nodes are present in another triangle. remove them.
	//check for changes in length, merge nodes if length too short.
	//check for big sides. Divide edge if length too big.
	//check
}

void updatePositionOfAllNodes();
void updateTriangles();
void removneNode(Node* n);
void shiftPropertiesOverNeighbours();
void shiftPropertiesOverNeighbourNodes();
void shiftPropertiesOverNeighboursTriangles();
