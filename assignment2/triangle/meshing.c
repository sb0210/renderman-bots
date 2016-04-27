#include "meshing.h"
#include "triangle.h"


	Node::Node(){

	}
	Node::Node(Position pos){
		this->pos = pos;
	}
	Position Node::getPos(){
		return pos;
	}
	float Node::getVelocity(){
		return velocity;
	}
	void Node::setVelocity(float vec){
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


	Triangle::Triangle(){

	}
	Triangle::Triangle(Node* n1, Node* n2, Node* n3){
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
	void Mesh::genMesh(){
		  struct triangulateio in, mid, out, vorout;

		  /* Define input points. */

		  in.numberofpoints = getNumNodes();
		  in.numberofpointattributes = 0;
		  in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
		  in.pointlist[0] = 0.0;
		  in.pointlist[1] = 0.0;
		  in.pointlist[2] = 1.0;
		  in.pointlist[3] = 0.0;
		  in.pointlist[4] = 1.0;
		  in.pointlist[5] = 10.0;
		  in.pointlist[6] = 0.0;
		  in.pointlist[7] = 10.0;

		  in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
		  in.pointmarkerlist[0] = 110;
		  in.pointmarkerlist[1] = 22;
		  in.pointmarkerlist[2] = 33;
		  in.pointmarkerlist[3] = 44;

		  in.numberofsegments = 0;
		  in.numberofholes = 0;
		  in.numberofregions = 0;

		  printf("Input point set:\n\n");
		  report(&in, 1, 0, 0, 0, 0, 0);

		  /* Make necessary initializations so that Triangle can return a */
		  /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */

		  mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
		  /* Not needed if -N switch used or number of point attributes is zero: */
		  mid.pointattributelist = (REAL *) NULL;
		  mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
		  mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
		  /* Not needed if -E switch used or number of triangle attributes is zero: */
		  mid.triangleattributelist = (REAL *) NULL;
		  mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
		  /* Needed only if segments are output (-p or -c) and -P not used: */
		  mid.segmentlist = (int *) NULL;
		  /* Needed only if segments are output (-p or -c) and -P and -B not used: */
		  mid.segmentmarkerlist = (int *) NULL;
		  mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
		  mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */

		  vorout.pointlist = (REAL *) NULL;        /* Needed only if -v switch used. */
		  /* Needed only if -v switch used and number of attributes is not zero: */
		  vorout.pointattributelist = (REAL *) NULL;
		  vorout.edgelist = (int *) NULL;          /* Needed only if -v switch used. */
		  vorout.normlist = (REAL *) NULL;         /* Needed only if -v switch used. */

		  /* Triangulate the points.  Switches are chosen to read and write a  */
		  /*   PSLG (p), preserve the convex hull (c), number everything from  */
		  /*   zero (z), assign a regional attribute to each element (A), and  */
		  /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
		  /*   neighbor list (n).                                              */

		  triangulate("z", &in, &mid, &vorout);

		  printf("Initial triangulation:\n\n");
		  report(&mid, 1, 1, 1, 1, 1, 0);
	}


void report(struct triangulateio *io,
int markers,
int reporttriangles,
int reportneighbors,
int reportsegments,
int reportedges,
int reportnorms)
{
  int i, j;

  for (i = 0; i < io->numberofpoints; i++) {
    printf("Point %4d:", i);
    for (j = 0; j < 2; j++) {
      printf("  %.6g", io->pointlist[i * 2 + j]);
    }
    if (io->numberofpointattributes > 0) {
      printf("   attributes");
    }
    for (j = 0; j < io->numberofpointattributes; j++) {
      printf("  %.6g",
             io->pointattributelist[i * io->numberofpointattributes + j]);
    }
    if (markers) {
      printf("   marker %d\n", io->pointmarkerlist[i]);
    } else {
      printf("\n");
    }
  }
  printf("\n");

  if (reporttriangles || reportneighbors) {
    for (i = 0; i < io->numberoftriangles; i++) {
      if (reporttriangles) {
        printf("Triangle %4d points:", i);
        for (j = 0; j < io->numberofcorners; j++) {
          printf("  %4d", io->trianglelist[i * io->numberofcorners + j]);
        }
        if (io->numberoftriangleattributes > 0) {
          printf("   attributes");
        }
        for (j = 0; j < io->numberoftriangleattributes; j++) {
          printf("  %.6g", io->triangleattributelist[i *
                                         io->numberoftriangleattributes + j]);
        }
        printf("\n");
      }
      if (reportneighbors) {
        printf("Triangle %4d neighbors:", i);
        for (j = 0; j < 3; j++) {
          printf("  %4d", io->neighborlist[i * 3 + j]);
        }
        printf("\n");
      }
    }
    printf("\n");
  }

  if (reportsegments) {
    for (i = 0; i < io->numberofsegments; i++) {
      printf("Segment %4d points:", i);
      for (j = 0; j < 2; j++) {
        printf("  %4d", io->segmentlist[i * 2 + j]);
      }
      if (markers) {
        printf("   marker %d\n", io->segmentmarkerlist[i]);
      } else {
        printf("\n");
      }
    }
    printf("\n");
  }

  if (reportedges) {
    for (i = 0; i < io->numberofedges; i++) {
      printf("Edge %4d points:", i);
      for (j = 0; j < 2; j++) {
        printf("  %4d", io->edgelist[i * 2 + j]);
      }
      if (reportnorms && (io->edgelist[i * 2 + 1] == -1)) {
        for (j = 0; j < 2; j++) {
          printf("  %.6g", io->normlist[i * 2 + j]);
        }
      }
      if (markers) {
        printf("   marker %d\n", io->edgemarkerlist[i]);
      } else {
        printf("\n");
      }
    }
    printf("\n");
  }
}


int main(){

}