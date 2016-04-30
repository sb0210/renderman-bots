#include "meshing.h"

#define REAL float
#define MASS 4.0f
#define TIMESCALE 20.0f;
#define MINIMUM_EDGE_LENGTH 6
#define MINIMUM_MAX_EDGE_LENGTH 4
#define MAXIMUM_EDGE_LENGTH 18


Force getGravitationalForce() {
	return glm::vec2(0.0,0.0)*MASS;//MASS*glm::vec2(0, -10);
}

Force getForceField(Node* node) {
	// if(node->pos.x>0)
		return MASS*glm::vec2(node->pos.x,node->pos.y);
	// else{
		// return MASS*glm::vec2(0,node->pos.y);		
	// }
	// return MASS*glm::vec2(1,1);
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

void Mesh::computeForcesOnNodes() {
	for(int i = 0; i<nodes.size(); i++) {
		nodes[i]->force = getGravitationalForce();
		nodes[i]->force += getForceField(nodes[i]);
	}
}

void Mesh::updateVelocityOfAllNodes() {
	for(int i = 0; i<nodes.size(); i++) {
		nodes[i]->velocity += ((nodes[i]->force)/MASS)/TIMESCALE;
	}
}


void Mesh::updatePositionOfAllNodes() {
	for(int i = 0; i<nodes.size(); i++) {
		// if(rand()%2==0)
			// nodes[i]->pos += nodes[i]->velocity;
		// else
		if(nodes[i]->pos.x>0)
			nodes[i]->pos += glm::normalize(nodes[i]->velocity);//nodes[i]->velocity;
		if(nodes[i]->pos.y < -30) {
			nodes[i]->pos.y = -30;
		}
	}
}


void Mesh::updateMesh(){
	cout<<"Number of Triangle "<<triangles.size()<<endl;
	cout<<"Number of Nodes "<<nodes.size()<<endl;

	computeForcesOnNodes();
	updateVelocityOfAllNodes();
	updatePositionOfAllNodes();
	updateTriangles();
	//calculate new velocity of the nodes.
	//update the position of the nodes.
	//check all the nodes and all the triangles. if a node ovelap with triangle, compute a set
	//perform contraction pass. remove edges if too short.
	//split overlapping elements of the s.
	//with each split, again contract.
	//
	//check the triangles. check nodes are present in another triangle. remove them.
	//check for changes in length, merge nodes if length too short.
	//check for big sides. Divide edge if length too big.
	//check
}

#define SCALING 50.0
void Mesh::getPositions(std::vector<glm::vec4> & v_positions,std::vector<glm::vec4> & v_colors){
  v_positions.resize(3*triangles.size());
  v_colors.resize(3*triangles.size());
  glm::vec4 color(1.0,0.5,0.7,0.5);
  std::vector<glm::vec4> temp_positions;
  for(int i=0;i<triangles.size();i++){
          v_positions[3*i] = glm::vec4(triangles[i]->nodes[0]->pos.x/SCALING,triangles[i]->nodes[0]->pos.y/SCALING,0,1);
          v_positions[3*i+1] = glm::vec4(triangles[i]->nodes[1]->pos.x/SCALING,triangles[i]->nodes[1]->pos.y/SCALING,0,1);
          v_positions[3*i+2] = glm::vec4(triangles[i]->nodes[2]->pos.x/SCALING,triangles[i]->nodes[2]->pos.y/SCALING,0,1);
          v_colors[3*i] = color;
          v_colors[3*i+1] = color;
          v_colors[3*i+2] = color;
  }
}
void Mesh::updateTriangles(){
	vector<int> removeList;
	vector<Triangle*> triangleList;
	vector<Node*> nodeList;
	for(int i=0;i<triangles.size();i++){
		float edgeLengths[3];
		float max_edge_length,min_edge_length;
		max_edge_length = 0.0;
		min_edge_length =  100;
		int max_i,min_i;
		for(int j=0;j<3;j++){
			edgeLengths[j]= glm::distance(triangles[i]->nodes[j%3]->pos,triangles[i]->nodes[(j+1)%3]->pos);
			if(max_edge_length<=edgeLengths[j]){
				max_edge_length=edgeLengths[j];
				max_i = j;
			}
			if(min_edge_length>=edgeLengths[j]){
				min_edge_length=edgeLengths[j];
				min_i = j;
			}
		}
		if(max_edge_length>MAXIMUM_EDGE_LENGTH ){
			Node* node =  new Node((triangles[i]->nodes[max_i%3]->pos + triangles[i]->nodes[(max_i+1)%3]->pos)/2.0f);
			node->force = (triangles[i]->nodes[max_i%3]->force + triangles[i]->nodes[(max_i+1)%3]->force)/2.0f;
			node->velocity = (triangles[i]->nodes[max_i%3]->velocity + triangles[i]->nodes[(max_i+1)%3]->velocity)/2.0f;
			if(triangles[i]->nodes[max_i%3]->is_boundary && triangles[i]->nodes[(max_i+1)%3]->is_boundary)
				node->is_boundary = true;
			else
				node->is_boundary = false;

			Triangle* t1 = new Triangle(triangles[i]->nodes[max_i%3],node,triangles[i]->nodes[(max_i+2)%3]);
			Triangle* t2 = new Triangle(triangles[i]->nodes[(max_i+1)%3],node,triangles[i]->nodes[(max_i+2)%3]);
			node->addTriangle(t1);
			node->addTriangle(t2);
			t1->volume = t2->volume = triangles[i]->volume/2;
			removeList.push_back(i);
			triangleList.push_back(t1);
			triangleList.push_back(t2);
			nodeList.push_back(node);
			free(triangles[i]);
			triangles.erase (triangles.begin()+i);
			i--;
		}
		// else if(min_edge_length<MINIMUM_EDGE_LENGTH){
		// 	if(true){	
		// 		cout<<"Delete"<<endl;
		// 		Node * remove_node = triangles[i]->nodes[min_i];
		// 		cout<<remove_node->pos.x<<remove_node->pos.y<<endl;
		// 		Node* neighbour_node = triangles[i]->nodes[(min_i+1)%3];
		// 		cout<<neighbour_node->pos.x<<neighbour_node->pos.y<<endl;
		// 		vector<Triangle*> neighbours =  remove_node->getTriangles();
		// 		for(int p=0;p<neighbours.size();p++){
		// 			neighbours[p]->print();
		// 		}
		// 		int x;
		// 		cin>>x;	
		// 		neighbour_node->addTriangles(neighbours);
		// 		cout<<"[a[a"<<endl;
		// 		for(int p=0;p<neighbours.size();p++){
		// 			neighbours[p]->print();
		// 		}
		// 		cin>>x;
		// 		for(int ii=0;ii<neighbours.size();ii++){
		// 			if(neighbours[ii]==triangles[i])
		// 				continue;
		// 			for(int k=0;k<3;k++){
		// 				if(neighbours[ii]->nodes[k]==remove_node){
		// 					neighbours[ii]->nodes[k]=neighbour_node;
		// 				}
		// 			}
		// 		}

		// 		for(int kk=0;kk<3;kk++){
		// 			if(triangles[i]->nodes[kk]==remove_node){
		// 				if(remove_node!=NULL)
		// 				free(remove_node);
		// 				nodes.erase(std::remove(nodes.begin(), nodes.end(), triangles[i]->nodes[kk]), nodes.end());						
		// 			}
		// 			else if(triangles[i]->nodes[kk]->getNeighbours().size()==1){
		// 				if(triangles[i]->nodes[kk]!=NULL)
		// 				free(triangles[i]->nodes[kk]);
		// 				cout<<"chutiay"<<endl;
		// 				nodes.erase(std::remove(nodes.begin(), nodes.end(), triangles[i]->nodes[kk]), nodes.end());
		// 			}
		// 		}
		// 		if(triangles[i]!=NULL)
		// 		free(triangles[i]);
		// 		triangles.erase (triangles.begin()+i);
		// 		i--;
		// 		cout<<"SP"<<endl;
		// 		// break;
		// 		break;
		// 	}
			// else{
			// 	vector<Triangle*> neighbours =  triangles[i]->nodes[min_i]->getTriangles();
			// 	triangles[i]->nodes[(min_i+1)%3]->addTriangles(neighbours);
			// 	for(int ii=0;ii<neighbours.size();ii++){
			// 		if(neighbours[ii]==triangles[i])
			// 			continue;
			// 		for(int k=0;k<3;k++){
			// 			if(neighbours[ii]->nodes[k]==triangles[i]->nodes[min_i]){
			// 				neighbours[ii]->nodes[k]=triangles[i]->nodes[(min_i+1)%3];
			// 			}
			// 		}
			// 	}
			// 	triangles[i]->print();

			// 	int x;
			// 	cin>>x;
			// 	// free(triangles[i]->nodes[min_i]);
			// 	// free(triangles[i]);
			// 	triangles.erase (triangles.begin()+i);
			// 	i--;
			// }
		}



	triangles.insert(triangles.end(), triangleList.begin(),triangleList.end());
	nodes.insert(nodes.end(), nodeList.begin(),nodeList.end());
}

void Node::addTriangles(vector<Triangle*> neighbours){
	for(int i=0;i<neighbours.size();i++){
		if(find(triangles.begin(),triangles.end(),neighbours[i])==triangles.end()){
			triangles.push_back(neighbours[i]);
		}
	}
}
void removeNode(Node* n);
void shiftPropertiesOverNeighbours();
void shiftPropertiesOverNeighbourNodes();
void shiftPropertiesOverNeighboursTriangles();

bool are_boundary(Node* n1, Node* n2){
	return n1->is_boundary && n2->is_boundary;
}