/*
    GLUT GRA
    Jui-Cheng,Sung. R.O.C.
    Lyre Mellark.
    Started from 2016.Nov.09
*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <vector>
#include "helpGra.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#define PI 3.14159265

int Width = 640;
int Height= 480;

using namespace std;

std::vector< vertex >   theSetOfMouse ;         //.stay                 ; ; Original input points
std::vector< vertex >   theSet2OfMouse ;        //.stay                 ; ; Original input points
std::vector< vertex >	theSetOfInputPoint ;    //.clear() in the end   ; ; tmp input set, only used in Delaunay
//std::vector< vertex >	subInputPointSet ;		//.clear() in the end   ; ;
std::vector< vertex >	tmp_PointSet ;			//.clear() in the end   ; ;
std::vector< vertex >   tmp_PointSet_2 ;        //.clear() in the end   ; ;
std::vector< vertex >   tmp_PointSet_3 ;        //.clear() in the end   ; ;
std::vector< vertex >   theSetOfNotedVertex ;   //.stay                 ; ; Original input points


//std::vector< vertex >   tmpRL_vertex_pool ;     //.clear() in the end   ; ; CDT 10.14.2016

std::vector< vertex >	bone_vertex_pool ;      //.clear() in the end   ; ; warping
std::vector< vertex >	bone_prime_vPool ;		//.clear() in the end   ; ; warping
std::vector< vertex >   bone_end_vPool ;        //.clear() in the end   ; ; warping
std::vector< vertex > 	bone_lower_vPool ;     	//.stay                 ; ; warping
std::vector< vertex > 	bone_uper_vPool ;       //.stay                 ; ; warping

std::vector< vertex > 	arch_pool_1 ;       	//.clear() in the end   ; ; warping
std::vector< vertex > 	arch_pool_2 ;       	//.clear() in the end   ; ; warping

std::vector< edge >		edgePool ;              //.clear() when started ; ; or in the end ;
std::vector< edge >		tmp_edgePool ;          //.clear() when started ; ; or in the end ;
std::vector< edge > 	bone_edgePool ;         //.stay                 ; ;
//std::vector< edge > 	bone_lower_ePool ;     	//.stay                 ; ; warping
//std::vector< edge > 	bone_uper_ePool ;       //.stay                 ; ; warping

std::vector< triangle > trianglePool ;          //.stay                 ;;
std::vector< triangle > badTrianglePool ;       // ???                  ;;
std::vector< triangle > subTrianglePool ;       //.clear() in the end   ;;

std::vector< triangle > meshPool ;       		//.stay                 ;;
std::vector< triangle > tmp_meshPool ;          //.stay tmp             ;;

std::vector< edge > edge_sharp_Pool ;           //.stay                 ;;
std::vector< vertex > sharp_bone_vPool ;        //.stay                 ;;


std::vector< vertex > Seed_Pool ;              //.clear() when started ;; GRA


/* Paremeters */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

triangle superDT;

int test = 15;//early use
int number;//for bone, isPathTriangles
int more = 0;

GLfloat prev_mouse_X=0;
GLfloat prev_mouse_Y=0;

bool meshDrawn = false;
bool meshBeenMade = true;
bool is_first_time = true;
bool drawn = true;
bool mode_2_on = false;
bool mode_3_on = false;
bool on = true;
bool on8 = false;
bool on7 = true;
bool mark_done = false;
bool draw_done = false;

//from cubeword
int yRotated = 0; 	//from cubeword
int zRotated = 0;	//from cubeword


//GRA variables-------------------------------------------

short shift_x = 5;
float ratioN1 = 25.0/128.0;
/*
struct polygon
{
    float vertex[6][2];
}*/


/* Shared function */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

bool isSameVertex( vertex A , vertex B )//("are" some vertecies) Using "is" is easier to distinguish between two functions.
{
	if(((A.x == B.x)&&(A.y == B.y))&&(A.z == B.z))
		return true;

	return false;
}

bool oneVertexOfEdge(vertex test_point, edge line)
{
    if( isSameVertex(test_point, line.v1) || isSameVertex(test_point, line.v2) )
        return true;
    return false;
}

bool isSameVertex2( vertex A , vertex B )//("are" some vertecies) Using "is" is easier to distinguish between two functions.
{
	if((A.x == B.x)&&(A.y == B.y))
		return true;

	return false;
}

bool areSameEdges( edge edgeA , edge edgeB )
{
    bool vA1B1 =(((edgeA.v1.x == edgeB.v1.x)&&(edgeA.v1.y == edgeB.v1.y))&&(edgeA.v1.z == edgeB.v1.z))&&
                (((edgeA.v2.x == edgeB.v2.x)&&(edgeA.v2.y == edgeB.v2.y))&&(edgeA.v2.z == edgeB.v2.z));
    bool vA1B2 =(((edgeA.v1.x == edgeB.v2.x)&&(edgeA.v1.y == edgeB.v2.y))&&(edgeA.v1.z == edgeB.v2.z))&&
                (((edgeA.v2.x == edgeB.v1.x)&&(edgeA.v2.y == edgeB.v1.y))&&(edgeA.v2.z == edgeB.v1.z));

	if(vA1B1||vA1B2)
		return true;

	return false;
}

bool areSameTriangles( triangle T1, triangle T2 )//New CDT
{
    if(isSameVertex( T1.v1, T2.v1 )){
        if(isSameVertex( T1.v2, T2.v2 )&&isSameVertex( T1.v3, T2.v3 ))
            return true;
        if(isSameVertex( T1.v2, T2.v3 )&&isSameVertex( T1.v3, T2.v2 ))
            return true;
    }
    else if(isSameVertex( T1.v1, T2.v2 )){
        if(isSameVertex( T1.v2, T2.v1 )&&isSameVertex( T1.v3, T2.v3 ))
            return true;
        if(isSameVertex( T1.v2, T2.v3 )&&isSameVertex( T1.v3, T2.v1 ))
            return true;
    }
    else if(isSameVertex( T1.v1, T2.v3 )){
        if(isSameVertex( T1.v2, T2.v1 )&&isSameVertex( T1.v3, T2.v2 ))
            return true;
        if(isSameVertex( T1.v2, T2.v2 )&&isSameVertex( T1.v3, T2.v1 ))
            return true;
    }
    else return false;
}

bool cross(vertex t1, vertex t2, edge e)//New CDT
{
    if(!onTheSameSideCDT(t1, e.v1, e.v2, t2) && !onTheSameSideCDT( e.v1, t1, t2, e.v2))
        return true;

    return false;
}

triangle findSuperTriangle()
{
    vertex vertex_1 = { 0.0, 1.0, 0.0 };
    vertex vertex_2 = { 0.5, 0.0, 0.0};
    vertex vertex_3 = { -0.5, 0.0, 0.0};
/*
    vertex vertex_1 = {-10.0f, -4.1f, 0.0f};
    vertex vertex_2 = {4.5f, 13.7f, 0.0f};
    vertex vertex_3 = {24.0f,-6.3f, 0.0f};
*/
    triangle super;

    for(int i=0;i<theSetOfInputPoint.size();i++)
    {
        if(outsideTheTriangle( theSetOfInputPoint[i], vertex_1, vertex_2, vertex_3 ))
        {
            again:
            if(!onTheSameSide( theSetOfInputPoint[i], vertex_1, vertex_2, vertex_3 )){
                vertex_1.x = vertex_1.x + vertex_1.x - vertex_3.x;
                vertex_2.x = vertex_2.x + vertex_2.x - vertex_3.x;
                vertex_3.x = vertex_3.x + vertex_3.x - theSetOfInputPoint[i].x;

                vertex_1.y = vertex_1.y + vertex_1.y - vertex_3.y;
                vertex_2.y = vertex_2.y + vertex_2.y - vertex_3.y;
                vertex_3.y = vertex_3.y + vertex_3.y - theSetOfInputPoint[i].y;
            }
            if(!onTheSameSide( theSetOfInputPoint[i], vertex_2, vertex_3, vertex_1 )){
                vertex_2.x = vertex_2.x + vertex_2.x - vertex_1.x;
                vertex_3.x = vertex_3.x + vertex_3.x - vertex_1.x;

                vertex_2.y = vertex_2.y + vertex_2.y - vertex_1.y;
                vertex_3.y = vertex_3.y + vertex_3.y - vertex_1.y;
            }
            if(!onTheSameSide( theSetOfInputPoint[i], vertex_3, vertex_1, vertex_2 )){
                vertex_1.x = vertex_1.x + vertex_1.x - vertex_2.x;
                vertex_3.x = vertex_3.x + vertex_3.x - vertex_2.x;

                vertex_1.y = vertex_1.y + vertex_1.y - vertex_2.y;
                vertex_3.y = vertex_3.y + vertex_3.y - vertex_2.y;
            }
        }
        if (outsideTheTriangle( theSetOfInputPoint[i], vertex_1, vertex_2, vertex_3 ))
        goto again;
    }

    super.v1 = vertex_1;
    super.v2 = vertex_2;
    super.v3 = vertex_3;

    return super;
}

vertex findMidPoint( vertex v1, vertex v2 )
{
    vertex mid_point;
    mid_point.x = (v1.x + v2.x)*0.5;
    mid_point.y = (v1.y + v2.y)*0.5;
    mid_point.z = 0.0;//or transform to

    return mid_point;
}

vertex findCenterPoint( triangle cencer_triangle )
{
	vertex center_point;
    vertex mid_point;
    mid_point = findMidPoint(cencer_triangle.v2, cencer_triangle.v3);
    center_point.x = cencer_triangle.v1.x - (cencer_triangle.v1.x - mid_point.x)*0.66;
    center_point.y = cencer_triangle.v1.y - (cencer_triangle.v1.y - mid_point.y)*0.66;
    center_point.z = 0.0;//or transform to the plain you want

    return center_point;
}

//float might need to be changed
void addToEdgePool( vertex v1 , vertex v2 )
{
    edge edgetp;
    edgetp.v1 = v1;
    edgetp.v2 = v2;

    edgePool.push_back(edgetp);
}

void addToTmpEdgePool( vertex v1 , vertex v2 )
{
    edge edgetp;
    edgetp.v1 = v1;
    edgetp.v2 = v2;

    tmp_edgePool.push_back(edgetp);
}

void addToBoneEdgePool( vertex v1 , vertex v2 )
{
    edge edgetp;
    edgetp.v1 = v1;
    edgetp.v2 = v2;

    bone_edgePool.push_back(edgetp);
}

void addToTrianglePool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    trianglePool.push_back(triangletp);
}

void addToBadTrianglePool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    badTrianglePool.push_back(triangletp);
}

void addToMeshPool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    meshPool.push_back(triangletp);
}


/* Triangle Base */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

void deletDoubleEdge()
{
    int i;
    int j;
    int k;
    int h;

    start:
    for(i=0;i<edgePool.size();i++)
    {
        for(j=0;j<edgePool.size();j++)
        {
            if( areSameEdges(edgePool[i],edgePool[j]) && (i!=j) )
            {

                for(k=0; k<edgePool.size(); k++){
                    if((k!=i)&&(k!=j))
                    tmp_edgePool.push_back(edgePool[k]);
                }

                edgePool.clear();
                for(h=0; h<tmp_edgePool.size(); h++){
                    edgePool.push_back(tmp_edgePool[h]);
                }
                tmp_edgePool.clear();
                goto start;
            }
        }
    }
}

bool triangulationContainSuperDT( triangle test, triangle super )
{
    vertex s1 = super.v1;
    vertex s2 = super.v2;
    vertex s3 = super.v3;

    if(
    (((test.v1.x == s1.x)&&(test.v1.y == s1.y))&&(test.v1.z == s1.z))||
    (((test.v1.x == s2.x)&&(test.v1.y == s2.y))&&(test.v1.z == s2.z))||
    (((test.v1.x == s3.x)&&(test.v1.y == s3.y))&&(test.v1.z == s3.z))||
    (((test.v2.x == s1.x)&&(test.v2.y == s1.y))&&(test.v2.z == s1.z))||
    (((test.v2.x == s2.x)&&(test.v2.y == s2.y))&&(test.v2.z == s2.z))||
    (((test.v2.x == s3.x)&&(test.v2.y == s3.y))&&(test.v2.z == s3.z))||
    (((test.v3.x == s1.x)&&(test.v3.y == s1.y))&&(test.v3.z == s1.z))||
    (((test.v3.x == s2.x)&&(test.v3.y == s2.y))&&(test.v3.z == s2.z))||
    (((test.v3.x == s3.x)&&(test.v3.y == s3.y))&&(test.v3.z == s3.z))
       )
       return true;

    return false;
}

bool triangulationContainPoint( triangle test, vertex s1 )
{
    if(
    (((test.v1.x == s1.x)&&(test.v1.y == s1.y))&&(test.v1.z == s1.z))||
    (((test.v2.x == s1.x)&&(test.v2.y == s1.y))&&(test.v2.z == s1.z))||
    (((test.v3.x == s1.x)&&(test.v3.y == s1.y))&&(test.v3.z == s1.z))
       )
       return true;

    return false;
}

bool rotation_type( vertex v1, vertex v2, vertex v3 )//true:clockwise ;false:Counterclockwise
{
    vertex p;
    p.x= v1.x + v2.y - v1.y;
    p.y= v1.y + v1.x - v2.x;
    p.z=0.0;
    if( onTheSameSide( p, v1, v2, v3 ) )
        return true;	//clockwise
    return false;		//Counterclockwise
}

bool check_inputPonits_rotationtype()//true:clockwise ;false:Counterclockwise
{
	int size = theSetOfInputPoint.size();
	int n1,n2,n3;
	n1 = 0;
	n2 = size/3;
	n3 = (size*2)/3;
	return rotation_type( theSetOfInputPoint[n1], theSetOfInputPoint[n2], theSetOfInputPoint[n3] );
}

bool isOutEars( triangle testTri, edge testEdge, bool origins_Rotation_Type ) //true:clockwise ;false:Counterclockwise
{
    if( triangulationContainPoint( testTri , testEdge.v1 ) ) {
    if( triangulationContainPoint( testTri , testEdge.v2 ) ){
        edge e1,e2,e3;
        e1.v1=testTri.v1; e1.v2=testTri.v2;
        e2.v1=testTri.v2; e2.v2=testTri.v3;
        e3.v1=testTri.v3; e3.v2=testTri.v1;
        vertex p;

        if( areSameEdges( e1,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( onTheSameSide( p, testTri.v1, testTri.v2, testTri.v3 ) )
            return true;
        }
        else if( areSameEdges( e2,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( onTheSameSide( p, testTri.v2, testTri.v3, testTri.v1 ) )
            return true;
        }
        else if( areSameEdges( e3,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( onTheSameSide( p, testTri.v3, testTri.v1, testTri.v2 ) )
            return true;
        }
    }
    }

    return false;
}

bool isPrimeEars( triangle testTri, bool origins_Rotation_Type )
{
	int t1,t2,t3;
	int n=0;
	for (int cot=0; cot<theSetOfInputPoint.size(); cot++)
	{
		if(n==0)
		{
			if(isSameVertex( testTri.v1 , theSetOfInputPoint[cot] ))  t1 = cot;
			if(isSameVertex( testTri.v2 , theSetOfInputPoint[cot] ))  t1 = cot;
			if(isSameVertex( testTri.v3 , theSetOfInputPoint[cot] ))  t1 = cot;
			n++;
		}

		if(n==1)
		{
			if(isSameVertex( testTri.v1 , theSetOfInputPoint[cot] ))  t2 = cot;
			if(isSameVertex( testTri.v2 , theSetOfInputPoint[cot] ))  t2 = cot;
			if(isSameVertex( testTri.v3 , theSetOfInputPoint[cot] ))  t2 = cot;
			n++;
		}

		if(n==2)
		{
			if(isSameVertex( testTri.v1 , theSetOfInputPoint[cot] ))  t3 = cot;
			if(isSameVertex( testTri.v2 , theSetOfInputPoint[cot] ))  t3 = cot;
			if(isSameVertex( testTri.v3 , theSetOfInputPoint[cot] ))  t3 = cot;
		}
	}
	//if( rotation_type( theSetOfInputPoint[t1], theSetOfInputPoint[t2], theSetOfInputPoint[t3] ) != origins_Rotation_Type )
    vertex p;
    p.x = theSetOfInputPoint[t1].x+theSetOfInputPoint[t2].y-theSetOfInputPoint[t1].y;
    p.y = theSetOfInputPoint[t1].y+theSetOfInputPoint[t1].x-theSetOfInputPoint[t2].x;
    p.z = 0.0;
	if( onTheSameSide( p, theSetOfInputPoint[t1], theSetOfInputPoint[t2], theSetOfInputPoint[t3] )
					 &&
					 ( ( ((t2-t1)!=1)&&((t3-t2)!=1) ) &&((t1-t3)!=1) ) )//theSetOfInputPoint[t1+(t2-t1)/2]
		return true;
	return false;
}

void normalize( GLfloat vec[3])
{
	float lenth = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
	lenth = sqrt(lenth);
	vec[0]= vec[0]/lenth;
	vec[1]= vec[1]/lenth;
	vec[2]= vec[2]/lenth;
}

bool isTooFlat( vertex mid_vertex, vertex long1_vertex, vertex long2_vertex )
{
	GLfloat vectorOne[] ={mid_vertex.x - long1_vertex.x, mid_vertex.y - long1_vertex.y, mid_vertex.z - long1_vertex.z};
	GLfloat vectorTwo[] ={long2_vertex.x - mid_vertex.x, long2_vertex.y - mid_vertex.y, long2_vertex.z - mid_vertex.z};
	GLfloat vectorBase[]={long2_vertex.x - long1_vertex.x, long2_vertex.y - long1_vertex.y, long2_vertex.z - long1_vertex.z};
	GLfloat oneDotTwo;

	normalize(vectorOne);
	normalize(vectorTwo);
	normalize(vectorBase);

	oneDotTwo = vectorOne[0]*vectorTwo[0] + vectorOne[1]*vectorTwo[1] + vectorOne[2]*vectorTwo[2];

	if(fabs(oneDotTwo-1)<0.005)
		return true;
	return false;
}

bool isNotTooSmall(triangle test_triangle)
{
	if(isTooFlat( test_triangle.v1, test_triangle.v2, test_triangle.v3 ))
		return false;
	if(isTooFlat( test_triangle.v2, test_triangle.v3, test_triangle.v1 ))
		return false;
	if(isTooFlat( test_triangle.v3, test_triangle.v1, test_triangle.v2 ))
		return false;
	return true;
}

void twoVertexIntoOneEdge(vertex vt1, vertex vt2, edge *edge_to_become)
{
    //edge_v1
    (*edge_to_become).v1.x = vt1.x;
    (*edge_to_become).v1.y = vt1.y;
    (*edge_to_become).v1.z = 0.0;

    //edge_v2
    (*edge_to_become).v2.x = vt2.x;
    (*edge_to_become).v2.y = vt2.y;
    (*edge_to_become).v2.z = 0.0;
}

void EndTriangle(triangle test_triangle)
{
    int count=0;

    bool numberED1 = false;
    bool numberED2 = false;
    bool numberED3 = false;

    edge e1, e2, e3;

    twoVertexIntoOneEdge(test_triangle.v1, test_triangle.v2, &e1);
    twoVertexIntoOneEdge(test_triangle.v2, test_triangle.v3, &e2);
    twoVertexIntoOneEdge(test_triangle.v3, test_triangle.v1, &e3);

    for(int i=0; i<tmp_edgePool.size(); i++)
    {
        if( areSameEdges(e1, tmp_edgePool[i]) ){
        count++;
        numberED1=true;
        }
        if( areSameEdges(e2, tmp_edgePool[i]) ){
        count++;
        numberED2=true;
        }
        if( areSameEdges(e3, tmp_edgePool[i]) ){
        count++;
        numberED3=true;
        }
    }

    if(count==2){
        //cout<<"222222"<<"\n";
        if(!numberED1)
            bone_end_vPool.push_back( findMidPoint( test_triangle.v1, test_triangle.v2 ) );
        if(!numberED2)
            bone_end_vPool.push_back( findMidPoint( test_triangle.v2, test_triangle.v3 ) );
        if(!numberED3)
            bone_end_vPool.push_back( findMidPoint( test_triangle.v3, test_triangle.v1 ) );
    }

}

bool isEndTriangle(triangle test_triangle)
{
    int count=0;
    int numberED=0;
    edge e1, e2, e3;

    twoVertexIntoOneEdge(test_triangle.v1, test_triangle.v2, &e1);
    twoVertexIntoOneEdge(test_triangle.v2, test_triangle.v3, &e2);
    twoVertexIntoOneEdge(test_triangle.v3, test_triangle.v1, &e3);

    for(int i=0; i</*tmp_*/edgePool.size(); i++)
    {
        if( areSameEdges(e1, /*tmp_*/edgePool[i]) ){
        count++;
        }
        if( areSameEdges(e2, /*tmp_*/edgePool[i]) ){
        count++;
        }
        if( areSameEdges(e3, /*tmp_*/edgePool[i]) ){
        count++;
        }

    }

    if(count==2){
        //cout<<"222222"<<"\n";
        return true;
    }

    return false;

}

bool isPathTriangle(triangle test_triangle)
{
    int count=0;
    edge e1, e2, e3;

    twoVertexIntoOneEdge(test_triangle.v1, test_triangle.v2, &e1);
    twoVertexIntoOneEdge(test_triangle.v2, test_triangle.v3, &e2);
    twoVertexIntoOneEdge(test_triangle.v3, test_triangle.v1, &e3);

    for(int i=0; i<tmp_edgePool.size(); i++)
    {
        if( areSameEdges(e1, tmp_edgePool[i]) ){
        count++;
        number=i;
        }
        if( areSameEdges(e2, tmp_edgePool[i]) ){
        count++;
        number=i;
        }
        if( areSameEdges(e3, tmp_edgePool[i]) ){
        count++;
        number=i;
        }
        /*
        if( areSameEdges(e1, tmp_edgePool[i]) )
        count++;
        if( areSameEdges(e2, tmp_edgePool[i]) )
        count++;
        if( areSameEdges(e3, tmp_edgePool[i]) )
        count++;
        if(count==1){
            number=i;
            return true;
        }
        */
    }

    if(count==1){
        return true;
    }

    return false;
}

bool isCenterTriangle(triangle test_triangle)
{
	int count=0;
    edge e1, e2, e3;

    twoVertexIntoOneEdge(test_triangle.v1, test_triangle.v2, &e1);
    twoVertexIntoOneEdge(test_triangle.v2, test_triangle.v3, &e2);
    twoVertexIntoOneEdge(test_triangle.v3, test_triangle.v1, &e3);

    for(int i=0; i<tmp_edgePool.size(); i++)
    {
        if( areSameEdges(e1, tmp_edgePool[i]) )
            count++;
        if( areSameEdges(e2, tmp_edgePool[i]) )
            count++;
        if( areSameEdges(e3, tmp_edgePool[i]) )
            count++;
    }

    if(count==0)
        return true;

    return false;
}

//GRA------------------------------------//
vertex cnt_clockwise_a_vertex(float degree, vertex vtoR)
{
    // |cos -sin|*|x|
    // |sin  cos| |y|
    GLfloat newX = cos(degree * PI / 180.0) * vtoR.x - sin(degree * PI / 180.0) * vtoR.y ;
    GLfloat newY = sin(degree * PI / 180.0) * vtoR.x + cos(degree * PI / 180.0) * vtoR.y ;
    vtoR.x = newX;
    vtoR.y = newY;

    return vtoR;
}

void read_Data_GRA()
{
    ;
}

//New CDT Start
/*------------------------------------------------------------------*/

/*
void swap_vertex(int number_a, int number_b)
{
    vertex tmp;
    tmp                     = theSetOfInputPoint[number_a];
    theSetOfInputPoint[number_a] = theSetOfInputPoint[number_b];
    theSetOfInputPoint[number_b] = tmp;
}

//template<typename T>
void quick_sort_recursive_CDT(int start, int end) {
    if (start >= end) return;
    GLfloat mid_x = theSetOfInputPoint[end].x;
    int left = start;
    int right = end - 1;

    while (left < right) {
        while (theSetOfInputPoint[left].x < mid_x && left < right) left++;
        while (theSetOfInputPoint[right].x >= mid_x && left < right) right--;
        swap_vertex(left, right);
    }
    if (theSetOfInputPoint[left].x >= theSetOfInputPoint[end].x)
        swap_vertex(left, end);
    else
        left++;
    quick_sort_recursive_CDT(start, left - 1);
    quick_sort_recursive_CDT(left + 1, end);
}
//template<typename T> //整數或浮點數皆可使用,若要使用物件(class)時必須設定"小於"(<)、"大於"(>)、"不小於"(>=)的運算子功能

void quick_sort_left_vPool(int start, int end) {
    if (start >= end) return;
    GLfloat mid_y = theSetOfInputPoint[end].y;
    int below = start, above = end - 1;
    while (below < above) {
        while (theSetOfInputPoint[below].y < mid_y && below < above) below++;
        while (theSetOfInputPoint[above].y >= mid_y && below < above) above--;
        swap_vertex(below, above);
    }
    if (theSetOfInputPoint[below].x >= theSetOfInputPoint[end].x)
        swap_vertex(below, end);
    else
        below++;
    quick_sort_left_vPool(start, below - 1);
    quick_sort_left_vPool(below + 1, end);
}

void quick_sort_right_vPool(int start, int end) {
    if (start >= end) return;
    GLfloat mid_y = theSetOfInputPoint[end].y;
    int below = start, above = end - 1;
    while (below < above) {
        while (theSetOfInputPoint[below].y < mid_y && below < above) below++;
        while (theSetOfInputPoint[above].y >= mid_y && below < above) above--;
        swap_vertex(below, above);
    }
    if (theSetOfInputPoint[below].y >= theSetOfInputPoint[end].y)
        swap_vertex(below, end);
    else
        below++;
    quick_sort_right_vPool(start, below - 1);
    quick_sort_right_vPool(below + 1, end);
}

void generate_ConstraintDelaunayTriangleEdges(int start, int end)//, edges of the G-graph )
{
    std::vector< vertex >   left_vertex_pool ;      //.clear() in the end   ; ; CDT 09.30.2016
    std::vector< vertex >   right_vertex_pool ;     //.clear() in the end   ; ; CDT 09.30.2016

    if((end - start) > 2){// start, x, y, end
        int mid = start + 2 ;
        float mid_x = (theSetOfInputPoint[start].x - theSetOfInputPoint[end].x)/2;
        float distance_from_mid_x_to_inputPoint = mid_x;

        for (int i = start; i < (end+1); i++)
        {
            if(abs(theSetOfInputPoint[i].x - mid_x) < distance_from_mid_x_to_inputPoint){
                distance_from_mid_x_to_inputPoint = abs(theSetOfInputPoint[i].x - mid_x);
                mid = i;
            }
        }

        for(int i = start; i < (end+1); i++){
            if(theSetOfInputPoint[i].x == mid_x){
                mid++;
            }
        }

        //define points_before_boundary,
        int pbb_start = start;
        int pbb_end   = mid-1;

        //define points_after_boundary,
        int pab_start = mid;
        int pab_end   = end;
cout<<"CDT_OK"<<"\n";
        generate_ConstraintDelaunayTriangleEdges(pbb_start, pbb_end);
cout<<"CDT_EZ"<<"\n";
        generate_ConstraintDelaunayTriangleEdges(pab_start, pab_end);

cout<<"dddddddddddddddd"<<"\n";

        //sticth points_before_boundary and points_after_boundary together;
        for(int i = pbb_start; i< (pbb_end+1); i++)
        {
            for(int j=0; j<edgePool.size(); j++){
                if( cross(theSetOfInputPoint[ i ], theSetOfInputPoint[ pab_start ], edgePool[j])==false ){
                    left_vertex_pool.push_back(theSetOfInputPoint[i]);
                }
            }
        }
        for(int i = pab_start; i< (pab_end+1); i++)
        {
            for(int j=0; j<edgePool.size(); j++){
                if( cross(theSetOfInputPoint[ pbb_end ], theSetOfInputPoint[ i ], edgePool[j])==false ){
                    right_vertex_pool.push_back(theSetOfInputPoint[i]);
                }
            }
        }
        quick_sort_left_vPool( 0, left_vertex_pool.size()-1 );
        quick_sort_right_vPool( 0, right_vertex_pool.size()-1 );


        //Stich vertices from left and vertices from right together,
        //or we can say "connect left strip and right strip."

        //Sort left-strip's vertices and right-strip's veretices

        int i1=0;
        int j1=0;
        int pa=0;
        int pb=0;

        addToEdgePool(left_vertex_pool[pa], right_vertex_pool[pb]);

        bool i_validity = true;
        bool j_validity = true;
        //bool crossLine = false;
        vertex candidateL;
        vertex candidateR;

        while( (i1< left_vertex_pool.size()) || (j1< right_vertex_pool.size()) )
        {
            candidateL = left_vertex_pool[i1];
            candidateR = right_vertex_pool[j1];

            vertex centerL_CDT = centerOfCircumscribedCircle(left_vertex_pool[pa], right_vertex_pool[pb], candidateL);
            vertex centerR_CDT = centerOfCircumscribedCircle(left_vertex_pool[pa], right_vertex_pool[pb], candidateR);

            i_validity = true;
            j_validity = true;

            for(int i = 0; i<left_vertex_pool.size(); i++){
                if((i!=pa)&&(i!=i1)){
                    if(!outsideTheTriangle(left_vertex_pool[i], left_vertex_pool[pa], right_vertex_pool[pb], candidateL)){
                        i_validity = false;
                    }
                }
            }

            for(int j = 0; j<right_vertex_pool.size(); j++){
                if((j!=pb)&&(j!=j1)){
                    if(!outsideTheTriangle(right_vertex_pool[j], left_vertex_pool[pa], right_vertex_pool[pb], candidateR)){
                        j_validity = false;
                    }
                }
            }

            if((centerL_CDT.y < centerR_CDT.y) && i_validity){
                //addToEdgePool(left_vertex_pool[pa], right_vertex_pool[pb]);
                addToEdgePool(left_vertex_pool[pa], left_vertex_pool[i1]);
                addToEdgePool(left_vertex_pool[i1], right_vertex_pool[pb]);
                pa = i1;
                i1++;
            }
            else if((centerR_CDT.y <= centerL_CDT.y) && j_validity){
                //addToEdgePool(left_vertex_pool[pa], right_vertex_pool[pb]);
                addToEdgePool(left_vertex_pool[pa], left_vertex_pool[j1]);
                addToEdgePool(left_vertex_pool[j1], right_vertex_pool[pb]);
                pb = j1;
                j1++;
            }

        }

        left_vertex_pool.clear();
        right_vertex_pool.clear();

        //return;
    }

    else if(start>=end){
        //return;
    }
    else if((end-start)==1)// start, end
    {
        cout<<"wwwwwwwwww"<<"\n";
        //connect points;
        addToEdgePool(theSetOfInputPoint[start],theSetOfInputPoint[end]);
        return;
    }

    else if((end-start)==2)// start, mid, end
    {
        cout<<"cccccccccccccccccccc"<<"\n";

        for(int i = start; i<end; i++){
            //connect points;
            addToEdgePool(theSetOfInputPoint[i],theSetOfInputPoint[i+1]);
        }
        return;
    }

}

void transform_CDTEdges_to_CDT()
{
    int      tri3;
    vertex   vt;
    edge     et;
    triangle tt;

    for(int i=0; i<edgePool.size(); i++){
        for(int j=0; j<edgePool.size(); j++){
            if( !areSameEdges(edgePool[i], edgePool[j]) ){
                if(oneVertexOfEdge(edgePool[j].v1, edgePool[i])){
                    et = edgePool[j];
                    vt = edgePool[j].v2;
                    tri3 = j;
                }
                if(oneVertexOfEdge(edgePool[j].v2, edgePool[i])){
                    et = edgePool[j];
                    vt = edgePool[j].v1;
                    tri3 = j;
                }
            }
        }
        for(int j=0; j<edgePool.size(); j++){
            if( !areSameEdges(edgePool[i], edgePool[j]) && !areSameEdges(edgePool[tri3], edgePool[j]) ){
                if(oneVertexOfEdge(vt, edgePool[j]) && oneVertexOfEdge(edgePool[i].v2, edgePool[j])
                    && oneVertexOfEdge(vt, et) && oneVertexOfEdge(edgePool[i].v1, et) )
                {
                    tt.v1 = vt;
                    tt.v2 = edgePool[i].v1;
                    tt.v3 = edgePool[i].v2;

                    for(int x=0; x<trianglePool.size(); x++){
                        if(!areSameTriangles(tt, trianglePool[x]))
                            addToTrianglePool(vt, edgePool[i].v1, edgePool[i].v2);
                    }
                }
                if(oneVertexOfEdge(vt, edgePool[j])&&oneVertexOfEdge(edgePool[i].v1, edgePool[j])
                    && oneVertexOfEdge(vt, et) && oneVertexOfEdge(edgePool[i].v2, et) )
                {
                    tt.v1 = vt;
                    tt.v2 = edgePool[i].v1;
                    tt.v3 = edgePool[i].v2;

                    for(int x=0; x<trianglePool.size(); x++){
                        if(!areSameTriangles(tt, trianglePool[x]))
                            addToTrianglePool(vt, edgePool[i].v1, edgePool[i].v2);
                    }
                }
            }
        }
    }
}

void ConstraintDelaunayTriangle()
{
    //get InputPointSet,
    edgePool.clear();
    for (int cot=0; cot<theSetOfInputPoint.size(); cot++){
        addToEdgePool( theSetOfInputPoint[cot], theSetOfInputPoint[(cot+1)%theSetOfInputPoint.size()] );
        //(cot+1)%theSetOfInputPoint.size()
    }
    //reorder InputPointSet by their coordenates of axis x (left to right);
    int points_start = 0;
    int points_end   = theSetOfInputPoint.size()-1;
    quick_sort_recursive_CDT(points_start, points_end);
    generate_ConstraintDelaunayTriangleEdges(points_start, points_end );//, edges of the G-graph );

    transform_CDTEdges_to_CDT();
}
*/
//New CDT Ends
/*------------------------------------------------------------------*/


void generateDelaunayTriangle()
{
    //triangle superDT;
    triangle test;
    edge edgeFT;
    vertex center;
    float radius;
    bool contains_superDT;

    trianglePool.clear();

    superDT = findSuperTriangle();
    addToTrianglePool(superDT.v1, superDT.v2, superDT.v3);


    for(int i=0; i<theSetOfInputPoint.size(); i++)
    {
        badTrianglePool.clear();
        for(int j=0; j<trianglePool.size(); j++)
            {
                test.v1 = trianglePool[j].v1;
                test.v2 = trianglePool[j].v2;
                test.v3 = trianglePool[j].v3;
                center = centerOfCircumscribedCircle( test.v1 , test.v2 , test.v3 );
                //center =
                radius = radiusOfCCircle( test.v1 , center );

                if( insideTheCircle( theSetOfInputPoint[i] , center , radius ) ){
                        addToBadTrianglePool( test.v1 , test.v2 , test.v3 );

                        //Delete badTri. from tri.Pool
                        if(j!=trianglePool.size()-1){
                            trianglePool[j]=trianglePool[trianglePool.size()-1];
                            j--;
                        }
                        trianglePool.pop_back();
                }

            }

        edgePool.clear();
        for(int q=0; q<badTrianglePool.size(); q++)
            {
                addToEdgePool( badTrianglePool[q].v1 , badTrianglePool[q].v2 );
                addToEdgePool( badTrianglePool[q].v2 , badTrianglePool[q].v3 );
                addToEdgePool( badTrianglePool[q].v3 , badTrianglePool[q].v1 );
            }

        deletDoubleEdge();

        //Part of pseudo code. Have been writen in Line 199 "Delete badTri. from tri.Pool"

//        for each triangle in badTriangles do // remove them from the data structure
//            remove triangle from triangulation

        for(int k=0;k<edgePool.size();k++)
            {
            	edgeFT = edgePool[k];
                addToTrianglePool( edgeFT.v1 , edgeFT.v2 , theSetOfInputPoint[i] );
            }

    }

    for (int ct=0; ct<trianglePool.size(); ct++) // done inserting points, now clean up
    {
        if( triangulationContainSuperDT( trianglePool[ct], superDT ) )
        {
            if(ct!=trianglePool.size()-1){
                trianglePool[ct]=trianglePool[trianglePool.size()-1];
                ct--;
            }
            trianglePool.pop_back();
        }
    }

    //--Trimming outside Triangles--
    //trim 01 : attached triangle
    edgePool.clear();
    for (int cot=0; cot<theSetOfInputPoint.size(); cot++){
        addToEdgePool( theSetOfInputPoint[cot], theSetOfInputPoint[(cot+1)%theSetOfInputPoint.size()] );
        //(cot+1)%theSetOfInputPoint.size()
    }
    for (int cnt=0; cnt<edgePool.size(); cnt++) // done inserting points, now clean up
    {
        for (int ad=0; ad<trianglePool.size(); ad++){
            if( isOutEars( trianglePool[ad], edgePool[cnt] , false ) )
            {
                if(ad!=trianglePool.size()-1){
                    trianglePool[ad]=trianglePool[trianglePool.size()-1];
                    ad--;
                }
                trianglePool.pop_back();
            }
        }
    }

    //trim 02 : prime triangle
    /*
    bool originRtype = check_inputPonits_rotationtype();
    for (int ad=0; ad<trianglePool.size(); ad++)
    {
        if ( isPrimeEars(trianglePool[ad], originRtype ) )
        {
            if( ad!=trianglePool.size()-1 ){
                    trianglePool[ad]=trianglePool[trianglePool.size()-1];
                    ad--;
            }
            trianglePool.pop_back();
        }
    }
*/
}


/* Bone line Finding*/
//-------------------------------------------------------------------------------------------------------------------------------------------------//

void generateBoneLine()
{

		tmp_edgePool.clear();
    	for(int i=0; i<theSetOfInputPoint.size(); i++)
    	{
        	if(i!=theSetOfInputPoint.size()-1)
            	addToTmpEdgePool(theSetOfInputPoint[i],theSetOfInputPoint[i+1]);
        	else
            	addToTmpEdgePool(theSetOfInputPoint[i],theSetOfInputPoint[0]);
    	}

	    for(int i=0; i<trianglePool.size(); i++)
	    {
	    	if( isCenterTriangle(trianglePool[i]) )
	    	{
	    		vertex center = findCenterPoint(trianglePool[i]);
	    		vertex mid1 = findMidPoint(trianglePool[i].v1, trianglePool[i].v2);
	    		vertex mid2 = findMidPoint(trianglePool[i].v2, trianglePool[i].v3);
	    		vertex mid3 = findMidPoint(trianglePool[i].v3, trianglePool[i].v1);
	    		addToBoneEdgePool(center, mid1);
	    		addToBoneEdgePool(center, mid2);
	    		addToBoneEdgePool(center, mid3);
                bone_prime_vPool.push_back(center);
	    	}
	    	if( isPathTriangle(trianglePool[i]) )
	    	{
	    			edge tri_edge_1;
	    			edge tri_edge_2;
	    			edge tri_edge_3;

	    			tri_edge_1.v1 = trianglePool[i].v1;
	    			tri_edge_1.v2 = trianglePool[i].v2;

	    			tri_edge_2.v1 = trianglePool[i].v2;
	    			tri_edge_2.v2 = trianglePool[i].v3;

	    			tri_edge_3.v1 = trianglePool[i].v3;
	    			tri_edge_3.v2 = trianglePool[i].v1;

	    			vertex bone_edge_add_1;
	    			vertex bone_edge_add_2;

	    			if( areSameEdges(tri_edge_1, tmp_edgePool[number]) )
	    			{
	    				bone_edge_add_1 = findMidPoint(tri_edge_2.v1, tri_edge_2.v2);
	    				bone_edge_add_2 = findMidPoint(tri_edge_3.v1, tri_edge_3.v2);
	    				addToBoneEdgePool(bone_edge_add_1, bone_edge_add_2);
	    			}

	    			else if( areSameEdges(tri_edge_2, tmp_edgePool[number]) )
	    			{
	    				bone_edge_add_1 = findMidPoint(tri_edge_3.v1, tri_edge_3.v2);
	    				bone_edge_add_2 = findMidPoint(tri_edge_1.v1, tri_edge_1.v2);
	    				addToBoneEdgePool(bone_edge_add_1, bone_edge_add_2);
	    			}

	    			else if( areSameEdges(tri_edge_3, tmp_edgePool[number]) )
	    			{
	    				bone_edge_add_1 = findMidPoint(tri_edge_1.v1, tri_edge_1.v2);
	    				bone_edge_add_2 = findMidPoint(tri_edge_2.v1, tri_edge_2.v2);
	    				addToBoneEdgePool(bone_edge_add_1, bone_edge_add_2);
	    			}
	    		}
                //cout<<"556655"<<"\n";
            if(isEndTriangle(trianglePool[i]))
            {
                //cout<<"5566"<<"\n";
                EndTriangle(trianglePool[i]);
            }

	    }
		cout<<"test"<<"\n";
        cout<<"InputPointSet: "<<theSetOfInputPoint.size()<<"\n";
        cout<<"Size of BoneEdge: "<<bone_edgePool.size()<<"\n";
        cout<<"triangle number: "<<trianglePool.size()<<"\n";
        cout<<"tmp_edge number: "<<tmp_edgePool.size()<<"\n";
        cout<<"\n";
	tmp_edgePool.clear();
	//edgePool.clear();
}


/* Generate Mesh */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

bool is_in_triangle_but_not_on_the_edge(vertex test_vertex, triangle test_triangle)
{
	if(!outsideTheTriangle(test_vertex, test_triangle.v1, test_triangle.v2, test_triangle.v3)){
		if(onTheTriangleEdges(test_vertex, test_triangle.v1, test_triangle.v2, test_triangle.v3))
			return false;
		return true;
	}
	return false;
}

bool is_a_prime_bone_line_vertex(vertex candidate)
{
	for(int i=0; i<trianglePool.size(); i++){
		if( is_in_triangle_but_not_on_the_edge(candidate, trianglePool[i]) ){
			return true;
		}
	}
	return false;
}

void delete_double_bone_vertex() // Clean the bone vertex pool, delete the double vertex
{
	int i;
    int j;
    int k;
    int h;

    start780:
    for(i=0;i<bone_vertex_pool.size();i++)
    {
        for(j=0;j<bone_vertex_pool.size();j++)
        {
            if( isSameVertex(bone_vertex_pool[i], bone_vertex_pool[j]) && (i!=j) )
            {

                for(k=0; k<bone_vertex_pool.size(); k++){
                    if((k!=i)&&(k!=j))
                    	tmp_PointSet.push_back(bone_vertex_pool[k]);
                	if(k==i)
                		tmp_PointSet.push_back(bone_vertex_pool[k]);
                }

                bone_vertex_pool.clear();
                for(h=0; h<tmp_PointSet.size(); h++){
                    bone_vertex_pool.push_back(tmp_PointSet[h]);
                }
                tmp_PointSet.clear();
                goto start780;
            }
        }
    }
    for(int m=0; m<bone_vertex_pool.size(); m++)
    {
    	for(int n=0; n<edgePool.size(); n++)
    	{
    		if(onTheEdge(bone_vertex_pool[m], edgePool[n].v1, edgePool[n].v2)){
    			bone_vertex_pool[m] = bone_vertex_pool[bone_vertex_pool.size()-1];
    			m--;
    			bone_vertex_pool.pop_back();
    		}
    	}
    }
}

bool in_prime_bone_vpool(vertex test_v)
{
	for(int i=0; i<bone_prime_vPool.size(); i++)
	{
		if(isSameVertex2(test_v, bone_prime_vPool[i]))
			return true;
	}
	return false;
}

void convert_bone_edge_to_vertex()
{
	for(int i=0; i<bone_edgePool.size(); i++)
	{
		bone_vertex_pool.push_back(bone_edgePool[i].v1);
		bone_vertex_pool.push_back(bone_edgePool[i].v2);
	}
	delete_double_bone_vertex();

	//Add prime vertex to bone_prime_vPool
    /*
	for (int i = 0; i < bone_vertex_pool.size(); i++)
	{
		if(is_a_prime_bone_line_vertex(bone_vertex_pool[i]))
			bone_prime_vPool.push_back(bone_vertex_pool[i]);
	}
    */
}

GLfloat distanceOfVertex(vertex a, vertex b)
{
	float x,y,z,dist;

	x = (a.x-b.x)*(a.x-b.x);
	y = (a.y-b.y)*(a.y-b.y);
	z = (a.z-b.z)*(a.z-b.z);

	dist = sqrt(x+y+z);

	return dist;
}

void create_uper_vpool()
{
	for(int i=0; i<bone_vertex_pool.size(); i++){
		for(int j=0; j<trianglePool.size(); j++){
			if(onTheEdgeMid(bone_vertex_pool[i],trianglePool[j].v1,trianglePool[j].v2))
			{
				bone_vertex_pool[i].z = distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v1)/1.5;
			}

			if(onTheEdgeMid(bone_vertex_pool[i],trianglePool[j].v2,trianglePool[j].v3))
			{
				bone_vertex_pool[i].z = distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v2)/1.5;
			}

			if(onTheEdgeMid(bone_vertex_pool[i],trianglePool[j].v3,trianglePool[j].v1))
			{
				bone_vertex_pool[i].z = distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v1)/1.5;
			}

			if( in_prime_bone_vpool(bone_vertex_pool[i]) && !outsideTheTriangle(bone_vertex_pool[i], trianglePool[j].v1, trianglePool[j].v2, trianglePool[j].v3) ){
				bone_vertex_pool[i].z = (
                    distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v1) +
                    distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v2) +
                    distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v3)
                    )/4.5;
			}
		}
	}

	for(int i=0; i<bone_vertex_pool.size(); i++){
		bone_uper_vPool.push_back(bone_vertex_pool[i]);
	}
}

void create_lower_vpool()
{
	for(int i=0; i<bone_vertex_pool.size(); i++){
		for(int j=0; j<trianglePool.size(); j++){
			if(onTheEdgeMid(bone_vertex_pool[i],trianglePool[j].v1,trianglePool[j].v2))
			{
				bone_vertex_pool[i].z = 0.0 - distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v1)/1.5;
			}

			if(onTheEdgeMid(bone_vertex_pool[i],trianglePool[j].v2,trianglePool[j].v3))
			{
				bone_vertex_pool[i].z = 0.0 - distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v2)/1.5;
			}

			if(onTheEdgeMid(bone_vertex_pool[i],trianglePool[j].v3,trianglePool[j].v1))
			{
				bone_vertex_pool[i].z = 0.0 - distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v1)/1.5;
			}

			if(in_prime_bone_vpool(bone_vertex_pool[i])&&!outsideTheTriangle(bone_vertex_pool[i], trianglePool[j].v1, trianglePool[j].v2, trianglePool[j].v3)){
				bone_vertex_pool[i].z = 0.0 - (
                    distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v1) +
                    distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v2) +
                    distanceOfVertex(bone_vertex_pool[i], trianglePool[j].v3)
                    )/4.5;
			}
		}
	}

	for(int i=0; i<bone_vertex_pool.size(); i++){
		bone_lower_vPool.push_back(bone_vertex_pool[i]);
	}
}

void addArch_stitch_store(vertex top, vertex ground, vertex top_2, vertex ground_2)
{
	vertex a12, a13, a14, a22, a23, a24;
	bool top_is_infected = false;
	bool top_2_is_infected = false;

	for(int i=0; i<sharp_bone_vPool.size(); i++)
	{
		if(isSameVertex2(top, sharp_bone_vPool[i])){
			top_is_infected = true;
            //cout <<"Wow!"<<"\n";
		}

		if(isSameVertex2(top_2, sharp_bone_vPool[i])){
			top_2_is_infected = true;
		    //cout <<"Wow!"<<"\n";
        }
	}

if( top_is_infected==false ){
	a12.x = top.x + (ground.x - top.x)*0.3827; // 0.3827 = (1/2.613);
	a12.y = top.y + (ground.y - top.y)*0.3827; // 0.3827 = (1/2.613);
	a12.z = ground.z + (top.z - ground.z)*0.923; // 0.923 = (2.414/2.613)

	a13.x = top.x + (ground.x - top.x)*0.707; // 0.707 = (1/1.414);
	a13.y = top.y + (ground.y - top.y)*0.707; // 0.707 = (1/1.414);
	a13.z = ground.z + (top.z - ground.z)*0.707; // 0.707 = (1/1.414);

	a14.x = top.x + (ground.x - top.x)*0.923; // 0.923 = (2.414/2.613)
	a14.y = top.y + (ground.y - top.y)*0.923; // 0.923 = (2.414/2.613)
	a14.z = ground.z + (top.z - ground.z)*0.3827; // 0.3827 = (1/2.613);
}
if( top_is_infected==true ){
	a12.x = top.x + (ground.x - top.x)*0.1; // 0.3827 = (1/2.613);
	a12.y = top.y + (ground.y - top.y)*0.1; // 0.3827 = (1/2.613);
	a12.z = ground.z + (top.z - ground.z)*0.61; // 0.923 = (2.414/2.613)

	a13.x = top.x + (ground.x - top.x)*0.3; // 0.707 = (1/1.414);
	a13.y = top.y + (ground.y - top.y)*0.3; // 0.707 = (1/1.414);
	a13.z = ground.z + (top.z - ground.z)*0.3; // 0.707 = (1/1.414);

	a14.x = top.x + (ground.x - top.x)*0.61; // 0.923 = (2.414/2.613)
	a14.y = top.y + (ground.y - top.y)*0.61; // 0.923 = (2.414/2.613)
	a14.z = ground.z + (top.z - ground.z)*0.1; // 0.3827 = (1/2.613);
}

if( top_2_is_infected==false ){
	a22.x = top_2.x + (ground_2.x - top_2.x)*0.3827; // 0.3827 = (1/2.613);
	a22.y = top_2.y + (ground_2.y - top_2.y)*0.3827; // 0.3827 = (1/2.613);
	a22.z = ground_2.z + (top_2.z - ground_2.z)*0.923; // 0.923 = (2.414/2.613)

	a23.x = top_2.x + (ground_2.x - top_2.x)*0.707; // 0.707 = (1/1.414);
	a23.y = top_2.y + (ground_2.y - top_2.y)*0.707; // 0.707 = (1/1.414);
	a23.z = ground_2.z + (top_2.z - ground_2.z)*0.707; // 0.707 = (1/1.414);

	a24.x = top_2.x + (ground_2.x - top_2.x)*0.923; // 0.923 = (2.414/2.613)
	a24.y = top_2.y + (ground_2.y - top_2.y)*0.923; // 0.923 = (2.414/2.613)
	a24.z = ground_2.z + (top_2.z - ground_2.z)*0.3827; // 0.3827 = (1/2.613);
}
if( top_2_is_infected==true ){
	a22.x = top_2.x + (ground_2.x - top_2.x)*0.1; // 0.3827 = (1/2.613);
	a22.y = top_2.y + (ground_2.y - top_2.y)*0.1; // 0.3827 = (1/2.613);
	a22.z = ground_2.z + (top_2.z - ground_2.z)*0.61; // 0.923 = (2.414/2.613)

	a23.x = top_2.x + (ground_2.x - top_2.x)*0.3; // 0.707 = (1/1.414);
	a23.y = top_2.y + (ground_2.y - top_2.y)*0.3; // 0.707 = (1/1.414);
	a23.z = ground_2.z + (top_2.z - ground_2.z)*0.3; // 0.707 = (1/1.414);

	a24.x = top_2.x + (ground_2.x - top_2.x)*0.61; // 0.923 = (2.414/2.613)
	a24.y = top_2.y + (ground_2.y - top_2.y)*0.61; // 0.923 = (2.414/2.613)
	a24.z = ground_2.z + (top_2.z - ground_2.z)*0.1; // 0.3827 = (1/2.613);
}

	arch_pool_1.push_back(top);
	arch_pool_1.push_back(a12);
	arch_pool_1.push_back(a13);
	arch_pool_1.push_back(a14);
	arch_pool_1.push_back(ground);

	arch_pool_2.push_back(top_2);
	arch_pool_2.push_back(a22);
	arch_pool_2.push_back(a23);
	arch_pool_2.push_back(a24);
	arch_pool_2.push_back(ground_2);


	if(isSameVertex(arch_pool_1[0], arch_pool_2[0])){
		for(int i=0; i<arch_pool_1.size()-1; i++)
		{
			if(i==0){
				addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
			}
			else{
				addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
				addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
			}
		}
	}

	else if(isSameVertex(arch_pool_1[4], arch_pool_2[4])){
		for(int i=0; i<arch_pool_1.size()-1; i++)
		{
			if(i==arch_pool_1.size()-1){
				addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
			}
			else{
				addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
				addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
			}
		}
	}

	else{
		for(int i=0; i<arch_pool_1.size()-1; i++)
		{
			addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
			addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
		}
	}
	/*
	for(int i=0; i<arch_pool_1.size()-1; i++)
		{
			addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
			addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
		}
*/
	arch_pool_1.clear();
	arch_pool_2.clear();
}

void generateMesh()
{
	meshDrawn = false;
	//calculate the height of each bone line;
	convert_bone_edge_to_vertex();
	create_lower_vpool();
	create_uper_vpool();
	/*bone_vertex_pool.clear();*/
	/*bone_edgePool.clear();*/

	cout<<"test"<<"\n";
	cout<<"Size of bone_edgePool: "<<bone_edgePool.size()<<"\n";
	cout<<"Size of bone_vertex_pool: "<<bone_vertex_pool.size()<<"\n";
    cout<<"Size of bone_uper_vPool: "<<bone_uper_vPool.size()<<"\n";
    cout<<"Size of bone_lower_vPool: "<<bone_lower_vPool.size()<<"\n";
    cout<<"\n";

    int end=0;
	int path=0;
	int prime=0;

    //set=1 means End Edge

    for(int i=0; i<bone_end_vPool.size(); i++)
    {
        int ed_i=0;

        for(int j=0; j<bone_vertex_pool.size(); j++)
        {
            if( isSameVertex2(bone_end_vPool[i],bone_vertex_pool[j]) ){
                ed_i=j;

                for(int k=0; k<trianglePool.size(); k++)
                {
                    if(isEndTriangle(trianglePool[k])){
                        if(onTheEdgeMid(bone_end_vPool[i], trianglePool[k].v1, trianglePool[k].v2)){
                            addArch_stitch_store( bone_uper_vPool[ed_i], trianglePool[k].v1, bone_uper_vPool[ed_i], trianglePool[k].v3 );
                            addArch_stitch_store( bone_uper_vPool[ed_i], trianglePool[k].v2, bone_uper_vPool[ed_i], trianglePool[k].v3 );

                            addArch_stitch_store( bone_lower_vPool[ed_i], trianglePool[k].v1, bone_lower_vPool[ed_i], trianglePool[k].v3 );
                            addArch_stitch_store( bone_lower_vPool[ed_i], trianglePool[k].v2, bone_lower_vPool[ed_i], trianglePool[k].v3 );
                        }

                        if(onTheEdgeMid(bone_end_vPool[i], trianglePool[k].v2, trianglePool[k].v3)){
                            addArch_stitch_store( bone_uper_vPool[ed_i], trianglePool[k].v2, bone_uper_vPool[ed_i], trianglePool[k].v1 );
                            addArch_stitch_store( bone_uper_vPool[ed_i], trianglePool[k].v3, bone_uper_vPool[ed_i], trianglePool[k].v1 );

                            addArch_stitch_store( bone_lower_vPool[ed_i], trianglePool[k].v2, bone_lower_vPool[ed_i], trianglePool[k].v1 );
                            addArch_stitch_store( bone_lower_vPool[ed_i], trianglePool[k].v3, bone_lower_vPool[ed_i], trianglePool[k].v1 );
                        }

                        if(onTheEdgeMid(bone_end_vPool[i], trianglePool[k].v3, trianglePool[k].v1)){
                            addArch_stitch_store( bone_uper_vPool[ed_i], trianglePool[k].v3, bone_uper_vPool[ed_i], trianglePool[k].v2 );
                            addArch_stitch_store( bone_uper_vPool[ed_i], trianglePool[k].v1, bone_uper_vPool[ed_i], trianglePool[k].v2 );

                            addArch_stitch_store( bone_lower_vPool[ed_i], trianglePool[k].v3, bone_lower_vPool[ed_i], trianglePool[k].v2 );
                            addArch_stitch_store( bone_lower_vPool[ed_i], trianglePool[k].v1, bone_lower_vPool[ed_i], trianglePool[k].v2 );
                        }
                    }
                }

            }
        }


    }

	//draw mesh (once uper part)
	for(int i=0; i<bone_edgePool.size(); i++)
	{
		int set=0;
		int let_i=0;
		int let_j=0;

		for(int j=0; j<bone_vertex_pool.size(); j++)
		{
			if(isSameVertex2(bone_edgePool[i].v1, bone_vertex_pool[j])){ let_i=j; set++; }//set=1
			if(isSameVertex2(bone_edgePool[i].v2, bone_vertex_pool[j])){ let_j=j; set++; }//set=2
		}
/*
            //set=1 means End Edge
            if(set==1){
                end++;
                //cout<<"test_set_1"<<"\n";
                for(int k=0; k<trianglePool.size(); k++) //這裡k++我原本打成j++
                {
                    if(onTheEdge(bone_vertex_pool[let_i], trianglePool[k].v1, trianglePool[k].v2)){
                        addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_i], trianglePool[k].v3 );
                        addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_i], trianglePool[k].v3 );

                        addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_i], trianglePool[k].v3 );
                        addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_i], trianglePool[k].v3 );
                    }

                    if(onTheEdge(bone_vertex_pool[let_i], trianglePool[k].v2, trianglePool[k].v3)){
                        addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_i], trianglePool[k].v1 );
                        addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_i], trianglePool[k].v1 );

                        addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_i], trianglePool[k].v1 );
                        addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_i], trianglePool[k].v1 );
                    }

                    if(onTheEdge(bone_vertex_pool[let_i], trianglePool[k].v3, trianglePool[k].v1)){
                        addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_i], trianglePool[k].v2 );
                        addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_i], trianglePool[k].v2 );

                        addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_i], trianglePool[k].v2 );
                        addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_i], trianglePool[k].v2 );
                    }
                }
            }
*/
			//set=2 means Path Edge or Prime-Path Edge
			if(set==2){
				//cout<<"test_set_2"<<"\n";
				short set_2 = 0;
				short set_v = 0;

				for(int p=0; p<bone_prime_vPool.size();p++)// Decide Bone_Edge is (Path Edge / Prime-Path Edge)
				{
					if( isSameVertex2(bone_edgePool[i].v1, bone_prime_vPool[p]) ){
						set_2 = 1;
						set_v = 1;
					}
					if( isSameVertex2(bone_edgePool[i].v2, bone_prime_vPool[p]) ){
						set_2 = 1;
						set_v = 2;
						//cout<<"test_prime"<<"\n";
					}
				}

				if( set_2 == 0 ){// Path Edge
					//cout<<"test_path"<<"\n";
					path++;
					if(let_i==let_i)//==j
					{
						//cout<<"test_let_i"<<"\n";
						for(int k=0; k<trianglePool.size(); k++) //這裡k++我原本打成j++
						{
							//cout<<"test_k"<<"\n";
							if(onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v1, trianglePool[k].v2)&&onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v2, trianglePool[k].v3)){
							//cout<<"xtgog"<<"\n";
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_j], trianglePool[k].v2 );//22
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_j], trianglePool[k].v3 );//13

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_j], trianglePool[k].v2 );//22
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_j], trianglePool[k].v3 );//13
							//cout<<"test_Edge"<<"\n";
							}

							if(onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v2, trianglePool[k].v3)&&onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v3, trianglePool[k].v1)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_j], trianglePool[k].v3 );//33
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_j], trianglePool[k].v1 );//21

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_j], trianglePool[k].v3 );//33
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_j], trianglePool[k].v1 );//21
							}

							if(onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v3, trianglePool[k].v1)&&onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v1, trianglePool[k].v2)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_j], trianglePool[k].v1 );//11
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_j], trianglePool[k].v2 );//32

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_j], trianglePool[k].v1 );//11
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_j], trianglePool[k].v2 );//32
							}

							if(onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v1, trianglePool[k].v2)&&onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v2, trianglePool[k].v3)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_j], trianglePool[k].v2 );//22
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_j], trianglePool[k].v1 );//31

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_j], trianglePool[k].v2 );//22
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_j], trianglePool[k].v1 );//31
							}

							if(onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v2, trianglePool[k].v3)&&onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v3, trianglePool[k].v1)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_j], trianglePool[k].v3 );//33
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_j], trianglePool[k].v2 );//12

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_j], trianglePool[k].v3 );//33
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_j], trianglePool[k].v2 );//12
							}

							if(onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v3, trianglePool[k].v1)&&onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v1, trianglePool[k].v2)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_j], trianglePool[k].v1 );//11
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_j], trianglePool[k].v3 );//23

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_j], trianglePool[k].v1 );//11
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_j], trianglePool[k].v3 );//23
							}
						}
					}
				}

				if( set_2 == 1 ){// Prime-Path Edge
					//cout<<"test_Prime-Path"<<"\n";
					prime++;
					for(int k=0; k<trianglePool.size(); k++) //這裡k++我原本打成j++,結果跑不出來
					{
						if(set_v==2)//(let_i==j && set_v==2)
						{
							if(onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v1, trianglePool[k].v2)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_j], trianglePool[k].v1 );
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_j], trianglePool[k].v2 );

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_j], trianglePool[k].v1 );
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_j], trianglePool[k].v2 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v2, trianglePool[k].v3)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v2, bone_uper_vPool[let_j], trianglePool[k].v2 );
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_j], trianglePool[k].v3 );

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v2, bone_lower_vPool[let_j], trianglePool[k].v2 );
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_j], trianglePool[k].v3 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdgeMid(bone_vertex_pool[let_i], trianglePool[k].v3, trianglePool[k].v1)){
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v3, bone_uper_vPool[let_j], trianglePool[k].v3 );
							addArch_stitch_store( bone_uper_vPool[let_i], trianglePool[k].v1, bone_uper_vPool[let_j], trianglePool[k].v1 );

							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v3, bone_lower_vPool[let_j], trianglePool[k].v3 );
							addArch_stitch_store( bone_lower_vPool[let_i], trianglePool[k].v1, bone_lower_vPool[let_j], trianglePool[k].v1 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );
						}
						if(set_v==1)//(let_j==j && set_v==1)
						{
							if(onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v1, trianglePool[k].v2)){
							addArch_stitch_store( bone_uper_vPool[let_j], trianglePool[k].v1, bone_uper_vPool[let_i], trianglePool[k].v1 );
							addArch_stitch_store( bone_uper_vPool[let_j], trianglePool[k].v2, bone_uper_vPool[let_i], trianglePool[k].v2 );

							addArch_stitch_store( bone_lower_vPool[let_j], trianglePool[k].v1, bone_lower_vPool[let_i], trianglePool[k].v1 );
							addArch_stitch_store( bone_lower_vPool[let_j], trianglePool[k].v2, bone_lower_vPool[let_i], trianglePool[k].v2 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v2, trianglePool[k].v3)){
							addArch_stitch_store( bone_uper_vPool[let_j], trianglePool[k].v2, bone_uper_vPool[let_i], trianglePool[k].v2 );
							addArch_stitch_store( bone_uper_vPool[let_j], trianglePool[k].v3, bone_uper_vPool[let_i], trianglePool[k].v3 );

							addArch_stitch_store( bone_lower_vPool[let_j], trianglePool[k].v2, bone_lower_vPool[let_i], trianglePool[k].v2 );
							addArch_stitch_store( bone_lower_vPool[let_j], trianglePool[k].v3, bone_lower_vPool[let_i], trianglePool[k].v3 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdgeMid(bone_vertex_pool[let_j], trianglePool[k].v3, trianglePool[k].v1)){
							addArch_stitch_store( bone_uper_vPool[let_j], trianglePool[k].v3, bone_uper_vPool[let_i], trianglePool[k].v3 );
							addArch_stitch_store( bone_uper_vPool[let_j], trianglePool[k].v1, bone_uper_vPool[let_i], trianglePool[k].v1 );

							addArch_stitch_store( bone_lower_vPool[let_j], trianglePool[k].v3, bone_lower_vPool[let_i], trianglePool[k].v3 );
							addArch_stitch_store( bone_lower_vPool[let_j], trianglePool[k].v1, bone_lower_vPool[let_i], trianglePool[k].v1 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );
						}
					}
				}
			}

		/*}*/

	}

	//draw lower part

	/*
	for( each vertex in bone line ){
		for( each edge in edge Pool ){
			if( edge near bone line ){
				;
			}
			if( edge near bone line ){
				;
			}
			generate mesh;
			store mesh into mesh pool;
		}
	}*/

	cout<<"test"<<"\n";
    cout<<"Size of Mesh: "<<meshPool.size()<<"\n";
    cout<<"Size of End: "<< bone_end_vPool.size() <<"\n";
    cout<<"Size of Path: "<< path <<"\n";
    cout<<"Size of Prime: "<< bone_prime_vPool.size()/*prime*/ <<"\n";
    cout<<"\n";
    cout<<"\n";
	//bone_vertex_pool.clear();
	bone_prime_vPool.clear();
    bone_end_vPool.clear();
	bone_lower_vPool.clear();
	bone_uper_vPool.clear();
	meshDrawn = true;
}


/* Printing block */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

void printTrianglePool()
{
    for(int i=0; i<trianglePool.size(); i++)
    {
        glBegin(GL_LINE_LOOP);
            glVertex3f( trianglePool[i].v1.x, trianglePool[i].v1.y, trianglePool[i].v1.z );
            glVertex3f( trianglePool[i].v2.x, trianglePool[i].v2.y, trianglePool[i].v2.z );
            glVertex3f( trianglePool[i].v3.x, trianglePool[i].v3.y, trianglePool[i].v3.z );
        glEnd();
    }
}

void printEdgePool()
{
    for(int i=0;i<bone_edgePool.size();i++){
        glBegin(GL_LINE_STRIP);
            glVertex3f( edgePool[i].v1.x, edgePool[i].v1.y, edgePool[i].v1.z );
            glVertex3f( edgePool[i].v2.x, edgePool[i].v2.y, edgePool[i].v2.z );
        glEnd();
    }
}

void printStroke()
{
    //if(!drawn)
    //{
    glBegin(GL_LINE_STRIP);
        for(int i=0;i<theSetOfMouse.size();i++){
            glVertex3f( theSetOfMouse[i].x, theSetOfMouse[i].y, theSetOfMouse[i].z );
        }
    glEnd();
    //}
}

void printBone()
{
	for(int i=0;i<bone_edgePool.size();i++){
		glBegin(GL_LINE_STRIP);
			glVertex3f( bone_edgePool[i].v1.x, bone_edgePool[i].v1.y, bone_edgePool[i].v1.z );
            glVertex3f( bone_edgePool[i].v2.x, bone_edgePool[i].v2.y, bone_edgePool[i].v2.z );
    	glEnd();
	}
}

void printMesh()
{
	for(int i=0; i<meshPool.size(); i++)
    {
        glBegin(GL_LINE_LOOP);
            glVertex3f( meshPool[i].v1.x, meshPool[i].v1.y, meshPool[i].v1.z );
            glVertex3f( meshPool[i].v2.x, meshPool[i].v2.y, meshPool[i].v2.z );
            glVertex3f( meshPool[i].v3.x, meshPool[i].v3.y, meshPool[i].v3.z );
        glEnd();
    }
}

void printMesh2()
{
	for(int i=0; i<meshPool.size(); i++)
    {
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f( meshPool[i].v1.x, meshPool[i].v1.y, meshPool[i].v1.z );
            glVertex3f( meshPool[i].v2.x, meshPool[i].v2.y, meshPool[i].v2.z );
            glVertex3f( meshPool[i].v3.x, meshPool[i].v3.y, meshPool[i].v3.z );
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f( meshPool[i].v3.x, meshPool[i].v3.y, meshPool[i].v3.z );
            glVertex3f( meshPool[i].v2.x, meshPool[i].v2.y, meshPool[i].v2.z );
            glVertex3f( meshPool[i].v1.x, meshPool[i].v1.y, meshPool[i].v1.z );
        glEnd();
    }
}

void printTmpMesh()
{
    for(int i=0; i<tmp_meshPool.size(); i++)
    {
        glBegin(GL_LINE_LOOP);
            glVertex3f( tmp_meshPool[i].v1.x, tmp_meshPool[i].v1.y, tmp_meshPool[i].v1.z );
            glVertex3f( tmp_meshPool[i].v2.x, tmp_meshPool[i].v2.y, tmp_meshPool[i].v2.z );
            glVertex3f( tmp_meshPool[i].v3.x, tmp_meshPool[i].v3.y, tmp_meshPool[i].v3.z );
        glEnd();
    }
}

void printTmpMesh2()
{
    for(int i=0; i<tmp_meshPool.size(); i++)
    {
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f( tmp_meshPool[i].v1.x, tmp_meshPool[i].v1.y, tmp_meshPool[i].v1.z );
            glVertex3f( tmp_meshPool[i].v2.x, tmp_meshPool[i].v2.y, tmp_meshPool[i].v2.z );
            glVertex3f( tmp_meshPool[i].v3.x, tmp_meshPool[i].v3.y, tmp_meshPool[i].v3.z );
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f( tmp_meshPool[i].v3.x, tmp_meshPool[i].v3.y, tmp_meshPool[i].v3.z );
            glVertex3f( tmp_meshPool[i].v2.x, tmp_meshPool[i].v2.y, tmp_meshPool[i].v2.z );
            glVertex3f( tmp_meshPool[i].v1.x, tmp_meshPool[i].v1.y, tmp_meshPool[i].v1.z );
        glEnd();
    }
}

void printSharpE()
{
    //if(!drawn)
    //{
    for(int i=0;i<edge_sharp_Pool.size();i++){
		glBegin(GL_LINE_STRIP);
			glVertex3f( edge_sharp_Pool[i].v1.x, edge_sharp_Pool[i].v1.y, edge_sharp_Pool[i].v1.z );
            glVertex3f( edge_sharp_Pool[i].v2.x, edge_sharp_Pool[i].v2.y, edge_sharp_Pool[i].v2.z );
    	glEnd();
	}
    //}
}

void printNoted()
{
    glBegin(GL_LINE_STRIP);
        for(int i=0;i<theSetOfNotedVertex.size();i++){
            glVertex3f( theSetOfNotedVertex[i].x, theSetOfNotedVertex[i].y, theSetOfNotedVertex[i].z );
        }
    glEnd();
}

void printObsticle0()
{
    float obst0[8][2] = {{9.000000, -7.000000}, {13.000000, 0.000000}, {9.000000, 6.000000}, {-11.000000, 6.000000}
                        , {-14.000000, 0.000000}, {-11.000000, -7.000000}, {11.000000, 8.000000}, {7.000000, 8.000000}};


    vertex rtm;
    for(int i=0; i<6; i++){
        rtm.x = obst0[i][0];
        rtm.y = obst0[i][1];
        rtm.z = 0.0;
        rtm = cnt_clockwise_a_vertex( 300, rtm );
        obst0[i][0] = rtm.x;
        obst0[i][1] = rtm.y;
    }

    glBegin(GL_POLYGON);
        glVertex3f( (obst0[0][0]+40.0-64.0)*ratioN1, (obst0[0][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst0[1][0]+40.0-64.0)*ratioN1, (obst0[1][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst0[2][0]+40.0-64.0)*ratioN1, (obst0[2][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst0[3][0]+40.0-64.0)*ratioN1, (obst0[3][1]+30.0-64.0)*ratioN1, 0.0  );
        glVertex3f( (obst0[4][0]+40.0-64.0)*ratioN1, (obst0[4][1]+30.0-64.0)*ratioN1, 0.0  );
        glVertex3f( (obst0[5][0]+40.0-64.0)*ratioN1, (obst0[5][1]+30.0-64.0)*ratioN1, 0.0 );
    glEnd();
}

void printObsticle1()
{
    float obst1[8][2] = {{17.000000, 6.000000}, {-17.000000, 6.000000}, {-17.000000, -7.000000}, {25.000000, -7.000000}
                        , {-14.000000, 0.000000}, {-11.000000, -7.000000}, {11.000000, 8.000000}, {7.000000, 8.000000}};


    vertex rtm;
    for(int i=0; i<4; i++){
        rtm.x = obst1[i][0];
        rtm.y = obst1[i][1];
        rtm.z = 0.0;
        rtm = cnt_clockwise_a_vertex( 3.75, rtm );
        obst1[i][0] = rtm.x;
        obst1[i][1] = rtm.y;
    }

    glBegin(GL_POLYGON);
        glVertex3f( (obst1[0][0]+90.0-64.0)*ratioN1, (obst1[0][1]+51.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst1[1][0]+90.0-64.0)*ratioN1, (obst1[1][1]+51.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst1[2][0]+90.0-64.0)*ratioN1, (obst1[2][1]+51.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst1[3][0]+90.0-64.0)*ratioN1, (obst1[3][1]+51.0-64.0)*ratioN1, 0.0  );
    glEnd();
}

void printObsticle2()
{
    float obst2[8][2] = {{9.000000, -3.000000}, {9.000000, 6.000000}, {-11.000000, 6.000000}, {-11.000000, -3.000000}
                        , {1.000000, 6.000000}, {1.000000, 10.000000}, {-2.000000, 10.000000}, {-2.000000, 6.000000}};


    vertex rtm;
    for(int i=0; i<8; i++){
        rtm.x = obst2[i][0];
        rtm.y = obst2[i][1];
        rtm.z = 0.0;
        rtm = cnt_clockwise_a_vertex( 90, rtm );
        obst2[i][0] = rtm.x;
        obst2[i][1] = rtm.y;
    }

    glBegin(GL_POLYGON);
        glVertex3f( (obst2[0][0]+56.0-64.0)*ratioN1, (obst2[0][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst2[1][0]+56.0-64.0)*ratioN1, (obst2[1][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst2[2][0]+56.0-64.0)*ratioN1, (obst2[2][1]+30.0-64.0)*ratioN1, 0.0  );
        glVertex3f( (obst2[3][0]+56.0-64.0)*ratioN1, (obst2[3][1]+30.0-64.0)*ratioN1, 0.0 );
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f( (obst2[4][0]+56.0-64.0)*ratioN1, (obst2[4][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst2[5][0]+56.0-64.0)*ratioN1, (obst2[5][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst2[6][0]+56.0-64.0)*ratioN1, (obst2[6][1]+30.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (obst2[7][0]+56.0-64.0)*ratioN1, (obst2[7][1]+30.0-64.0)*ratioN1, 0.0  );
    glEnd();
}

void printRobot0()
{
    //robot#0
    float robo0[8][2] = {{15.000000, 4.000000}, {-3.000000, 4.000000}, {-3.000000, -4.000000}, {15.000000, -4.000000}
                        , {7.000000, 4.000000}, {11.000000, 4.000000}, {11.000000, 8.000000}, {7.000000, 8.000000}};

    vertex rtm;
    for(int i=0; i<8; i++){
        rtm.x = robo0[i][0];
        rtm.y = robo0[i][1];
        rtm.z = 0.0;
        rtm = cnt_clockwise_a_vertex( 90, rtm );
        robo0[i][0] = rtm.x;
        robo0[i][1] = rtm.y;
    }

    glBegin(GL_POLYGON);
        glVertex3f( (robo0[0][0]+64.0-64.0)*ratioN1, (robo0[0][1]+64.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo0[1][0]+64.0-64.0)*ratioN1, (robo0[1][1]+64.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo0[2][0]+64.0-64.0)*ratioN1, (robo0[2][1]+64.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo0[3][0]+64.0-64.0)*ratioN1, (robo0[3][1]+64.0-64.0)*ratioN1, 0.0  );
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f( (robo0[4][0]+64.0-64.0)*ratioN1, (robo0[4][1]+64.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo0[5][0]+64.0-64.0)*ratioN1, (robo0[5][1]+64.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo0[6][0]+64.0-64.0)*ratioN1, (robo0[6][1]+64.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo0[7][0]+64.0-64.0)*ratioN1, (robo0[7][1]+64.0-64.0)*ratioN1, 0.0  );
    glEnd();

}

void printRobot1()
{
    //robot#1
    float robo1[8][2] = {{-5.000000, -5.000000}, {5.000000, -5.000000}, {0.000000, 5.000000}, {15.000000, -4.000000}
                        , {7.000000, 4.000000}, {11.000000, 4.000000}, {11.000000, 8.000000}, {7.000000, 8.000000}};

    vertex rtm;
    for(int i=0; i<3; i++){
        rtm.x = robo1[i][0];
        rtm.y = robo1[i][1];
        rtm.z = 0.0;
        rtm = cnt_clockwise_a_vertex( 90, rtm );
        robo1[i][0] = rtm.x;
        robo1[i][1] = rtm.y;
    }

    glBegin(GL_POLYGON);
        glVertex3f( (robo1[0][0]+20.0-64.0)*ratioN1, (robo1[0][1]+20.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo1[1][0]+20.0-64.0)*ratioN1, (robo1[1][1]+20.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (robo1[2][0]+20.0-64.0)*ratioN1, (robo1[2][1]+20.0-64.0)*ratioN1, 0.0 );
    glEnd();
}//-shift_x

void printRobot0g()
{
    //robot#0
    glBegin(GL_POLYGON);
        glVertex3f( (15.000000+80.0-64.0)*ratioN1, (4.000000+80.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (-3.000000+80.0-64.0)*ratioN1, (4.000000+80.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (-3.000000+80.0-64.0)*ratioN1, (-4.000000+80.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (15.000000+80.0-64.0)*ratioN1, (-4.000000+80.0-64.0)*ratioN1, 0.0  );
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f( (7.000000+80.0-64.0)*ratioN1, (4.000000+80.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (11.000000+80.0-64.0)*ratioN1, (4.000000+80.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (11.000000+80.0-64.0)*ratioN1, (8.000000+80.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (7.000000+80.0-64.0)*ratioN1, (8.000000+80.0-64.0)*ratioN1, 0.0  );
    glEnd();

}

void printRobot1g()
{
    //robot#1
    glBegin(GL_POLYGON);
        glVertex3f( (-5.000000+30.0-64.0)*ratioN1, (-5.000000+100.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (5.000000+30.0-64.0)*ratioN1, (-5.000000+100.0-64.0)*ratioN1, 0.0 );
        glVertex3f( (0.000000+30.0-64.0)*ratioN1, (5.000000+100.0-64.0)*ratioN1, 0.0 );
    glEnd();
}//-shift_x

void printBorder()
{
    glBegin(GL_POLYGON);
        glVertex3f( -12.50, -12.50, 0.0 );
        glVertex3f( 12.50, -12.50, 0.0 );
        glVertex3f( 12.50, 12.50, 0.0 );
        glVertex3f( -12.50, 12.50, 0.0 );
    glEnd();
}

void printButton()
{
    for(int i=0; i<6; i++){
        glBegin(GL_LINE_LOOP);
            glVertex3f( -19.0, 8.7-i*4, 0.0 );
            glVertex3f( -13.5, 8.7-i*4, 0.0 );
            glVertex3f( -13.5, 11.7-i*4, 0.0 );
            glVertex3f( -19.0, 11.7-i*4, 0.0 );
        glEnd();
    }

    for(int i=0; i<6; i++){
        glBegin(GL_LINE_LOOP);
            glVertex3f( 13.5, 8.7-i*4, 0.0 );
            glVertex3f( 19.0, 8.7-i*4, 0.0 );
            glVertex3f( 19.0, 11.7-i*4, 0.0 );
            glVertex3f( 13.5, 11.7-i*4, 0.0 );
        glEnd();
    }

}

void printSeed()
{
    
    for(int i=0; i<Seed_Pool.size(); i++){
        glBegin(GL_POINTS);
            glVertex3f( (Seed_Pool[i].x-64) * ratioN1, (Seed_Pool[i].y-64) * ratioN1, 1.0 );
        glEnd();
    }
    
}
/* Callback function */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

void recordMousePos( int x, int y )
{
    vertex pre_pos;
    GLfloat xf,yf;

    xf = x;
    yf = y;
    xf = (xf-Width/2)/(Width/40);
    yf = (yf-Height/2)/(Height/30);
/*
    GLfloat deltaX = xf - prev_mouse_X;
    GLfloat deltaY = yf - prev_mouse_Y;

    xf = xf + deltaX/1.25;
    yf = yf + deltaY/1.25;
*/
    pre_pos.x=xf;
    pre_pos.y=-yf;
    pre_pos.z=0.0;
if(!mode_2_on && !mode_3_on)
    theSetOfMouse.push_back(pre_pos);
if(mode_2_on && !mode_3_on)
	theSet2OfMouse.push_back(pre_pos);
if(mode_3_on && !mode_2_on)
    theSet2OfMouse.push_back(pre_pos);
/*
    prev_mouse_X = xf;
    prev_mouse_Y = yf;
*/
}

/* even the set of mouse */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

GLfloat distanceVnV(vertex tV1, vertex tV2)
{
    float distance;

    float vx = tV1.x-tV2.x;
    float vy = tV1.y-tV2.y;

    distance = sqrt(pow(vx, 2.0)+pow(vy, 2.0));

    return distance;
}

void evendistance()
{
    GLfloat distance_1 = 0.0;
    int size = theSetOfMouse.size();

    if(theSetOfMouse.size()!=0)
    {
        for(int i=0; i<size; i++)
        {
            if( i != size-1 )
                distance_1 = distance_1 + distanceVnV( theSetOfMouse[i], theSetOfMouse[i+1] );
        }
        distance_1 = distance_1 / size;
    }

    if( distanceVnV(theSetOfMouse[size-1], theSetOfMouse[0]) > distance_1 )
    {
        int count=0;
        count = distanceVnV(theSetOfMouse[size-1], theSetOfMouse[0]) / distance_1;

        vertex tmp;
        float vx = (theSetOfMouse[0].x-theSetOfMouse[size-1].x) / (count + 1);
        float vy = (theSetOfMouse[0].y-theSetOfMouse[size-1].y) / (count + 1);

        for(int i=0; i<count-1; i++)
        {
            tmp.x = theSetOfMouse[size-1].x + vx*(i+1);
            tmp.y = theSetOfMouse[size-1].y + vy*(i+1);
            tmp.z = 0.0;
            theSetOfMouse.push_back(tmp);
            //cout<<"10101 "<<theSetOfMouse.size()<<"\n";
        }
        //cout<<"size size "<<size<<"\n";
        //cout<<"real size "<<theSetOfMouse.size()<<"\n";
    }

    int j = 0;
    float tmp_lenth = distance_1 / 10.0;
    tmp_PointSet.push_back( theSetOfMouse[0] );

    for(int i=1; i<theSetOfMouse.size(); i++)
    {
        j = i;
        again522:
        if( (i!=theSetOfMouse.size()-1) && distanceVnV(theSetOfMouse[j-1], theSetOfMouse[i]) < distance_1 ){
            i++;
            goto again522;
        }
        if( i!=theSetOfMouse.size()-1 )
            tmp_PointSet.push_back( theSetOfMouse[i] );

    }

    theSetOfMouse.clear();

    for(int i=0; i<tmp_PointSet.size(); i++){
        theSetOfMouse.push_back(tmp_PointSet[i]);
    }

    tmp_PointSet.clear();
}

void evendistance2()
{
    GLfloat distance_1 = 0.0;
    int size = theSet2OfMouse.size();

    if(theSet2OfMouse.size()!=0)
    {
        for(int i=0; i<size; i++)
        {
            if( i != size-1 )
                distance_1 = distance_1 + distanceVnV( theSet2OfMouse[i], theSet2OfMouse[i+1] );
        }
        distance_1 = distance_1 / size;
    }

    if( distanceVnV(theSet2OfMouse[size-1], theSet2OfMouse[0]) > distance_1 )
    {
        int count=0;
        count = distanceVnV(theSet2OfMouse[size-1], theSet2OfMouse[0]) / distance_1;

        vertex tmp;
        float vx = (theSet2OfMouse[0].x-theSet2OfMouse[size-1].x) / (count + 1);
        float vy = (theSet2OfMouse[0].y-theSet2OfMouse[size-1].y) / (count + 1);

        for(int i=0; i<count-1; i++)
        {
            tmp.x = theSet2OfMouse[size-1].x + vx*(i+1);
            tmp.y = theSet2OfMouse[size-1].y + vy*(i+1);
            tmp.z = 0.0;
            theSet2OfMouse.push_back(tmp);
            //cout<<"10101 "<<theSet2OfMouse.size()<<"\n";
        }
        //cout<<"size size "<<size<<"\n";
        //cout<<"real size "<<theSet2OfMouse.size()<<"\n";
    }

    int j = 0;
    float tmp_lenth = distance_1 / 10.0;
    tmp_PointSet.push_back( theSet2OfMouse[0] );

    for(int i=1; i<theSet2OfMouse.size(); i++)
    {
        j = i;
        again5478:
        if( (i!=theSet2OfMouse.size()-1) && distanceVnV(theSet2OfMouse[j-1], theSet2OfMouse[i]) < distance_1 ){
            i++;
            goto again5478;
        }
        if( i!=theSet2OfMouse.size()-1 )
            tmp_PointSet.push_back( theSet2OfMouse[i] );

    }

    theSet2OfMouse.clear();

    for(int i=0; i<tmp_PointSet.size(); i++){
        theSet2OfMouse.push_back(tmp_PointSet[i]);
    }

    tmp_PointSet.clear();
}

/* glut function */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

void rout_bone_line()
{
    //make seed point
    float x = 0.0;
    float y = 0.0;
    vertex tmp;
    tmp.z = 0.0;
    for(int i=7; i<121; i=i+8){
        for(int j=7; j<121; j+=8){
            x = i;
            y = j;
            tmp.x = x;
            tmp.y = y;
            Seed_Pool.push_back(tmp);
        }
    }

    //make
    
    //..............find start and end
    vertex start_point; 
    vertex end_point;
    for(int j=0; j<Seed_Pool.size(); j++){
        ;
    }
}

void teddy_test()
{
    int jump = 2;
    if(!meshBeenMade)
    {
        evendistance();

        jump = theSetOfMouse.size()/70 + 1 + more;
        for(int i=0; i<theSetOfMouse.size(); i=i+jump){
        theSetOfInputPoint.push_back(theSetOfMouse[i]);//keeping the strokes number under 100 will perform better
        //cout<<"test"<<"\n";
        }
        //generateDelaunayTriangle();//Using CDT so don't need
            //ConstraintDelaunayTriangle();
        //generateBoneLine();
        //generateMesh();
        theSetOfInputPoint.clear();

        meshBeenMade=true;
    }
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    Width  = width;
    Height = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    /*const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;*/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (meshDrawn==false)
    {
    glPushMatrix();
    	glColor3d(1,0,0);
        glTranslated(0.0,0.0,-30);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printStroke();
    glPopMatrix();
    }

    glPushMatrix();
    	glColor3d(0,1,0);
        glTranslated(0.0,0.0,-50);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printSharpE();
    glPopMatrix();

    if(draw_done==false)
    {
    glPushMatrix();
        glColor3d(1,0,0);
        glTranslated(0.0,0.0,-30);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printNoted();
    glPopMatrix();
    }

    if (on8)
    {
    glPushMatrix();
        glTranslated(0.0,0.0,-30);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printTrianglePool();
    glPopMatrix();

    glPushMatrix();
    	glColor3d(0,0,1);
        glTranslated(0.0,0.0,-30);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printBone();
    glPopMatrix();
    }

    if (meshDrawn&&on)
    {
	glPushMatrix();
    	glColor3d(0.0,0.0,0.0);
        glTranslated(0.0,0.0,-30);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printMesh();
        printTmpMesh();

        if(on7){
        glColor3d(1.0,1.0,1.0);
        printMesh2();
        printTmpMesh2();
        }
    glPopMatrix();
    }
/*
    glPushMatrix();//new CDT test
        glTranslated(0.0,0.0,-30);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printEdgePool();
    glPopMatrix();
*/
    glPushMatrix();//GRA test
        glTranslated(0.0,0.0,-30);
        glRotated(yRotated, 0, 1, 0);
        glRotated(zRotated, 1, 0, 0);
        printObsticle0();
        printObsticle1();
        printObsticle2();
        glColor3d(0,0,1);
        printRobot0();
        printRobot1();
        glColor3d(0,1,0);
        printRobot0g();
        printRobot1g();
        glColor3d(0,0,0);
        printBorder();
        printButton();
        glColor3d(1,0,0);
        printSeed();
    glPopMatrix();

    glutSwapBuffers();
}


/* Adjust Mesh */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

GLfloat between_edge_Mark(vertex tV1, vertex tV2, vertex markP)
{
    float distance;

    float vx = tV1.x-tV2.x;
    float vy = tV1.y-tV2.y;

    if(tV1.x!=tV2.x)
    {
        distance = fabs(markP.y - ( tV1.y + vy*(markP.x-tV1.x)/vx));
    }

    //calculate x distance from test point to line.
    if(tV1.x==tV2.x)
    {
        distance = fabs(markP.x-tV1.x);
    }

    distance = distance - sqrt(pow(vx, 2.0)+pow(vy, 2.0)) +
                          sqrt(pow(tV1.x-markP.x, 2.0)+pow(tV1.y-markP.y, 2.0)) +
                          sqrt(pow(tV2.x-markP.x, 2.0)+pow(tV2.y-markP.y, 2.0));

    return distance;
}

bool triangleContain(vertex bV, edge sE, triangle testTriangle)
{
    edge e1, e2, e3;
    e1.v1 = testTriangle.v1;
    e1.v2 = testTriangle.v2;
    e2.v1 = testTriangle.v2;
    e2.v2 = testTriangle.v3;
    e3.v1 = testTriangle.v3;
    e3.v2 = testTriangle.v1;

    if(
        (areSameEdges(sE,e1)||areSameEdges(sE,e2)||areSameEdges(sE,e3))
        &&
        (onTheEdge(bV, e1.v1, e1.v2)||onTheEdge(bV, e2.v1, e2.v2)||onTheEdge(bV, e3.v1, e3.v2))
    ) return true;

    return false;
}

bool boneV_near_sharpE(vertex boneV, edge sharpE)
{
    for(int i=0; i<trianglePool.size(); i++)
    {
        if(triangleContain(boneV, sharpE, trianglePool[i]))
            return true;
    }
    return false;
}

void mark_sharp_edge()
{
    cout<<"\n";
    cout<<"Size of tri: "<< trianglePool.size()<<"\n";
    cout<<"Size of BVpool: "<< bone_vertex_pool.size()<<"\n";
    int jumpAg = 2;
    jumpAg = theSetOfMouse.size()/70 + 1 + more;
    theSetOfInputPoint.clear();
    for(int i=0;i<theSetOfMouse.size();i=i+jumpAg){
            theSetOfInputPoint.push_back(theSetOfMouse[i]);
    }

    for(int i=0;i<theSet2OfMouse.size();i=i+jumpAg){
        tmp_PointSet.push_back(theSet2OfMouse[i]);
    }

    if(mark_done)
    {
    //start to label
        for(int i=0;i<tmp_PointSet.size();i++)
        {
            int record_j=0;
            GLfloat between_edge_and_Mark = 100.0;//between_edge_Mark(theSetOfInputPoint[0], theSetOfInputPoint[1], tmp_PointSet[i]);
            edge tmp_sE;

            for(int j=0; j < theSetOfInputPoint.size();j++)
            {
                if( between_edge_Mark(theSetOfInputPoint[j], theSetOfInputPoint[j+1], tmp_PointSet[i]) < between_edge_and_Mark ){
                    record_j = j;
                    between_edge_and_Mark = between_edge_Mark(theSetOfInputPoint[j], theSetOfInputPoint[j+1], tmp_PointSet[i]);
                }
            }
            tmp_sE.v1 = theSetOfInputPoint[record_j];
            tmp_sE.v2 = theSetOfInputPoint[record_j+1];

            edge_sharp_Pool.push_back( tmp_sE );
        }

        for(int i=0; i<bone_vertex_pool.size(); i++)
        {
            for(int j=0; j < edge_sharp_Pool.size(); j++)
            {
                if( boneV_near_sharpE( bone_vertex_pool[i], edge_sharp_Pool[j] ) ){
                    sharp_bone_vPool.push_back(bone_vertex_pool[i]);
                }
            }
        }


        int num123 = 0;
        /*
        for(int i=0; i<bone_vertex_pool.size(); i++)
        {
            for(int j=0; j < sharp_bone_vPool.size(); j++)
            {
                if( isSameVertex2( bone_vertex_pool[i], sharp_bone_vPool[j] ) ){
                    num123++;
                }
            }
        }
        */
        meshPool.clear();
        trianglePool.clear();

        bone_edgePool.clear();
        bone_vertex_pool.clear();
        bone_uper_vPool.clear();
        bone_lower_vPool.clear();

        meshBeenMade=false;
        generateDelaunayTriangle();
        generateBoneLine();
        generateMesh();

        cout<<"\n";
        cout<<"Size of ESP: "<< edge_sharp_Pool.size()<<"\n";
        cout<<"Size of SBVP: "<< sharp_bone_vPool.size()<<"\n";
        cout<<"BVP = SBVP: "<< num123<<"\n";
        //cout<<"Size of InputSet: "<< theSetOfInputPoint.size()<<"\n";

        theSetOfInputPoint.clear();
        tmp_PointSet.clear();

    }
}


/* Operation on Object Surface */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

GLfloat distancePnM(vertex tV1, vertex tV2)
{
    float distance;

    float vx = fabs(tV1.x-tV2.x);
    float vy = fabs(tV1.y-tV2.y);

    distance = sqrt(pow(vx, 2.0)+pow(vy, 2.0));

    return distance;
}

void draw()
{
    theSetOfNotedVertex.clear();
    if( draw_done )
    {

    int jumpAg = 2;
    jumpAg = theSet2OfMouse.size()/75 + 1 + more;

    //theSetOfInputPoint.clear();
    /*
    for(int i=0;i<theSetOfMouse.size();i=i+jumpAg){
            theSetOfInputPoint.push_back(theSetOfMouse[i]);
    }
    */
    evendistance2();
    for(int i=0;i<theSet2OfMouse.size();i=i+jumpAg){
        tmp_PointSet.push_back(theSet2OfMouse[i]);
    }

    //draw
    for(int i=0;i<tmp_PointSet.size();i++)
    {
        int record_j = 0;
        int record_v = 0;
        GLfloat distance_P_and_M = 100.0;
        //edge tmp_sE;

        for(int j=0; j < meshPool.size(); j++)
        {
            if( distancePnM( tmp_PointSet[i], meshPool[j].v1 ) < distance_P_and_M ){
                distance_P_and_M = distancePnM( tmp_PointSet[i], meshPool[j].v1 );
                record_j = j;
                record_v = 1;
            }
            if( distancePnM( tmp_PointSet[i], meshPool[j].v2 ) < distance_P_and_M ){
                distance_P_and_M = distancePnM( tmp_PointSet[i], meshPool[j].v2 );
                record_j = j;
                record_v = 2;
            }
            if( distancePnM( tmp_PointSet[i], meshPool[j].v3 ) < distance_P_and_M ){
                distance_P_and_M = distancePnM( tmp_PointSet[i], meshPool[j].v3 );
                record_j = j;
                record_v = 3;
            }
        }

        if(record_v==1){
            theSetOfNotedVertex.push_back( meshPool[record_j].v1 );
        }
        if(record_v==2){
            theSetOfNotedVertex.push_back( meshPool[record_j].v2 );
        }
        if(record_v==3){
            theSetOfNotedVertex.push_back( meshPool[record_j].v3 );
        }

    }
    cout<<"Noted size"<<theSetOfNotedVertex.size()<<"\n";

    theSet2OfMouse.clear();
    tmp_PointSet.clear();

    /* Limb Start */
    for(int i=0; i<theSetOfNotedVertex.size(); i++){
        tmp_PointSet_2.push_back(theSetOfNotedVertex[i]);
    }
    int count = tmp_PointSet_2.size();
    float x1, y1, z1;
    float x2, y2, z2;
    x1 = tmp_PointSet_2[0].x - tmp_PointSet_2[count/3].x;
    y1 = tmp_PointSet_2[0].y - tmp_PointSet_2[count/3].y;
    z1 = tmp_PointSet_2[0].z - tmp_PointSet_2[count/3].z;

    x2 = tmp_PointSet_2[count/3].x - tmp_PointSet_2[2*count/3].x;
    y2 = tmp_PointSet_2[count/3].y - tmp_PointSet_2[2*count/3].y;
    z2 = tmp_PointSet_2[count/3].z - tmp_PointSet_2[2*count/3].z;

    float normal_x, normal_y, normal_z;
    normal_x = y1*z2 - z1*y2;
    normal_y = z1*x2 - x1*z2;
    normal_z = x1*y2 - y1*x2;

    if(normal_z < 0.0){
        normal_x = -normal_x;
        normal_y = -normal_y;
        normal_z = -normal_z;
    }

    float limit = 5.3;
    normal_x = normal_x / limit;
    normal_y = normal_y / limit;
    normal_z = normal_z / limit;

    float rate = 0.12;
    float rate_stay = 0.5;
    float num = 1.01;
    vertex center = centerOfCircumscribedCircle( tmp_PointSet_2[0], tmp_PointSet_2[count/3], tmp_PointSet_2[2*count/3] );
    vertex tmp_vertex;
    triangle tmp_triangle;

    for(int i=0;i< 6/*tmp_PointSet.size()*/;i++)
    {
        int size = count;
/*
        if(i==4){

            for(int j=0; j< size; j++){
                tmp_vertex.x = tmp_PointSet_2[j].x + (center.x - tmp_PointSet_2[j].x) * rate + normal_x* rate_stay;
                tmp_vertex.y = tmp_PointSet_2[j].y + (center.y - tmp_PointSet_2[j].y) * rate + normal_y* rate_stay;
                tmp_vertex.z = tmp_PointSet_2[j].z + (center.z - tmp_PointSet_2[j].z) * rate + normal_z* rate_stay;
                tmp_PointSet_3.push_back(tmp_vertex);
            }

            center = centerOfCircumscribedCircle( tmp_PointSet_3[0], tmp_PointSet_3[count/3], tmp_PointSet_3[2*count/3] );

            //center.x = center.x + normal_x ;//* rate_stay * rate_stay;
            //center.y = center.y + normal_y ;//* rate_stay * rate_stay;
            //center.z = center.z + normal_z ;//* rate_stay * rate_stay;

            for(int j=0; j< size; j++){
                tmp_triangle.v1 = tmp_PointSet_2[j%size];
                tmp_triangle.v2 = tmp_PointSet_2[(j+1)%size];
                tmp_triangle.v3 = center;
                tmp_meshPool.push_back(tmp_triangle);
            }

            goto dd2049;
        }*/

        if((i%2) == 0){

            for(int j=0; j< size; j++){
                tmp_vertex.x = tmp_PointSet_2[j].x + (center.x - tmp_PointSet_2[j].x) * rate + normal_x* rate_stay;
                tmp_vertex.y = tmp_PointSet_2[j].y + (center.y - tmp_PointSet_2[j].y) * rate + normal_y* rate_stay;
                tmp_vertex.z = tmp_PointSet_2[j].z + (center.z - tmp_PointSet_2[j].z) * rate + normal_z* rate_stay;
                tmp_PointSet_3.push_back(tmp_vertex);
            }
/*
            center.x = center.x + normal_x;
            center.y = center.y + normal_y;
            center.z = center.z + normal_z;
*/
            for(int j=0; j< size; j++){
                tmp_triangle.v1 = tmp_PointSet_2[j%size];
                tmp_triangle.v2 = tmp_PointSet_2[(j+1)%size];
                tmp_triangle.v3 = tmp_PointSet_3[j%size];
                tmp_meshPool.push_back(tmp_triangle);

                tmp_triangle.v1 = tmp_PointSet_2[(j+1)%size];
                tmp_triangle.v2 = tmp_PointSet_3[(j+1)%size];
                tmp_triangle.v3 = tmp_PointSet_3[j%size];
                tmp_meshPool.push_back(tmp_triangle);
            }

            tmp_PointSet_2.clear();
            center = centerOfCircumscribedCircle( tmp_PointSet_3[0], tmp_PointSet_3[count/3], tmp_PointSet_3[2*count/3] );

        }

        if((i%2) == 1){
        cout<<"000111"<<"\n";

            for(int j=0; j< size; j++){
                tmp_vertex.x = tmp_PointSet_3[j].x + (center.x - tmp_PointSet_3[j].x) * rate + normal_x* rate_stay;
                tmp_vertex.y = tmp_PointSet_3[j].y + (center.y - tmp_PointSet_3[j].y) * rate + normal_y* rate_stay;
                tmp_vertex.z = tmp_PointSet_3[j].z + (center.z - tmp_PointSet_3[j].z) * rate + normal_z* rate_stay;
                tmp_PointSet_2.push_back(tmp_vertex);
            }
/*
            center.x = center.x + normal_x;
            center.y = center.y + normal_y;
            center.z = center.z + normal_z;
*/
            for(int j=0; j< size; j++){
                tmp_triangle.v1 = tmp_PointSet_3[j%size];
                tmp_triangle.v2 = tmp_PointSet_3[(j+1)%size];
                tmp_triangle.v3 = tmp_PointSet_2[j%size];
                tmp_meshPool.push_back(tmp_triangle);

                tmp_triangle.v1 = tmp_PointSet_3[(j+1)%size];
                tmp_triangle.v2 = tmp_PointSet_2[(j+1)%size];
                tmp_triangle.v3 = tmp_PointSet_2[j%size];
                tmp_meshPool.push_back(tmp_triangle);
            }
            tmp_PointSet_3.clear();
            center = centerOfCircumscribedCircle( tmp_PointSet_2[0], tmp_PointSet_2[count/3], tmp_PointSet_2[2*count/3] );
        }

        num = num * num ;
        rate = rate * num;

        cout<<center.z<<"\n";
        cout<<rate<<"\n";

    }

    dd2049:

    tmp_PointSet_2.clear();
    tmp_PointSet_3.clear();

/*
        //deform
        for(int i=0; i<bone_vertex_pool.size(); i++)
        {
            for(int j=0; j < edge_sharp_Pool.size(); j++)
            {
                if( boneV_near_sharpE( bone_vertex_pool[i], edge_sharp_Pool[j] ) ){
                    sharp_bone_vPool.push_back(bone_vertex_pool[i]);
                }
            }
        }

        meshPool.clear();
        trianglePool.clear();

        bone_edgePool.clear();
        bone_vertex_pool.clear();
        bone_uper_vPool.clear();
        bone_lower_vPool.clear();

        meshBeenMade=false;
        generateDelaunayTriangle();
        generateBoneLine();
         generateMesh();
*/

    }

}

/*deform*/
void change_curvature()
{
    //for(){}
    if(0/*something*/)
    {
        //draw
        for(int i=0;i<theSetOfNotedVertex.size();i++)
        {
            int record_j = 0;
            int record_v = 0;
            /*
            GLfloat distance_P_and_M = 100.0;
            //edge tmp_sE;
            */
            for(int j=0; j < meshPool.size(); j++)
            {
                if( isSameVertex2( theSetOfNotedVertex[i], meshPool[j].v1 ) ){
                    record_j = j;
                    record_v = 1;
                }
                if( isSameVertex2( theSetOfNotedVertex[i], meshPool[j].v1 ) ){
                    record_j = j;
                    record_v = 2;
                }
                if( isSameVertex2( theSetOfNotedVertex[i], meshPool[j].v1 ) ){
                    record_j = j;
                    record_v = 3;
                }
            }

            if(record_v==1){
                meshPool[record_j].v1.x = theSetOfNotedVertex[i].x;
                meshPool[record_j].v1.y = theSetOfNotedVertex[i].y;
                meshPool[record_j].v1.z = theSetOfNotedVertex[i].z;
            }
            if(record_v==2){
                meshPool[record_j].v2.x = theSetOfNotedVertex[i].x;
                meshPool[record_j].v2.y = theSetOfNotedVertex[i].y;
                meshPool[record_j].v2.z = theSetOfNotedVertex[i].z;
            }
            if(record_v==3){
                meshPool[record_j].v3.x = theSetOfNotedVertex[i].x;
                meshPool[record_j].v3.y = theSetOfNotedVertex[i].y;
                meshPool[record_j].v3.z = theSetOfNotedVertex[i].z;
            }

        }
        /*
        //deform
        for(int i=0; i<bone_vertex_pool.size(); i++)
        {
            for(int j=0; j < edge_sharp_Pool.size(); j++)
            {
                if( boneV_near_sharpE( bone_vertex_pool[i], edge_sharp_Pool[j] ) ){
                    sharp_bone_vPool.push_back(bone_vertex_pool[i]);
                }
            }
        }

        meshPool.clear();
        trianglePool.clear();

        bone_edgePool.clear();
        bone_vertex_pool.clear();
        bone_uper_vPool.clear();
        bone_lower_vPool.clear();

        meshBeenMade=false;
        generateDelaunayTriangle();
        generateBoneLine();
        generateMesh();
*/
        theSet2OfMouse.clear();
        tmp_PointSet.clear();

    }

}

/* Generate a new Limb on surface. */
void limb()
{
    ;
}


/* Control buttoms */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

//Mouse draw
void mousebutton( int button, int state, int x, int y )
{
    if(button==GLUT_LEFT_BUTTON) {
        switch(state) {

			case GLUT_DOWN:
				if(mode_2_on==false&&mode_3_on==false){
					drawn = false;
                    meshDrawn=false;
				}
				if(mode_2_on==true&&mode_3_on==false){
					mark_done = false;
					//mark_sharp_edge();
				}
                if(mode_3_on==true&&mode_2_on==false){
                    draw_done = false;
                    //mark_sharp_edge();
                }
				break;

			case GLUT_UP:
				if(mode_2_on==false&&mode_3_on==false){//mode_1_on
                    //evendistance();
					drawn = true;
					cout<<"Size of Stroke: "<<theSetOfMouse.size()<<"\n";
                	cout<<"\n";
                	meshBeenMade = false;
                	teddy_test();
                }
                if(mode_2_on==true&&mode_3_on==false){
                	mark_done = true;
					mark_sharp_edge();
                    //teddy_test();
				}
                if(mode_3_on==true&&mode_2_on==false){
                    draw_done = true;
                    draw();
                    draw_done = false;

                    //teddy_test();
                }
				break;
		}
	}
	if(button==GLUT_RIGHT_BUTTON) {
        if(mode_2_on==false&&mode_3_on==false){//mode_1_on
        	theSetOfMouse.clear();
        	theSetOfInputPoint.clear();
        	trianglePool.clear();
        	edgePool.clear();
        	bone_edgePool.clear();
            bone_prime_vPool.clear();
        	meshPool.clear();
        	prev_mouse_X=0;
        	prev_mouse_Y=0;
        	is_first_time=true;
        	meshBeenMade = false;
            yRotated=0.0;
            zRotated=0.0;
        }
        if(mode_2_on==true&&mode_3_on==false){
			mark_done = false;
			theSet2OfMouse.clear();
			edge_sharp_Pool.clear();
            sharp_bone_vPool.clear();
            mark_done = true;
            mark_sharp_edge();
			//sharp_bone_vPool.clear();
		}
        if(mode_3_on==true&&mode_2_on==false){
            draw_done = false;
            theSet2OfMouse.clear();
            tmp_PointSet.clear();
            theSetOfNotedVertex.clear();
            tmp_meshPool.clear();
            //draw_done = true;
            //draw();
            //sharp_bone_vPool.clear();
        }

	}
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case 'r':
            meshBeenMade=false;
            test=test+1;
            test=test%16;
            if(test%16==0)
                test=5;
            teddy_test();
            break;

        case 'e':
            meshBeenMade=false;
            test=test-1;
            test=test%16;
            if(test%16<0)
                test=15;
            teddy_test();
            break;


        case 'a':
            meshBeenMade=false;
            test=15;
            break;

        case 'j':
            /*  */
            //latiRotate-=0.1;
            yRotated-=1;//0.01;
            yRotated=yRotated%360;
            //if(latiRotate<0)latiRotate=160+latiRotate;
            break;
        case 'k':
            /* return */
            //latiRotate=0;
            //longRotate=0;
            yRotated=0.0;
            zRotated=0.0;
            break;
        case 'l':
            /*  */
            //latiRotate+=0.1;
            yRotated+=1;//0.01;
            yRotated=yRotated%360;
            //if(latiRotate>=160)latiRotate=latiRotate%160;
            break;
        case 'i':
            /*  */
            //longRotate-=0.1;
            zRotated-=1;
            zRotated=zRotated%360;
            //if(longRotate<0)longRotate=160+longRotate;
            break;
        case ',':
            /*  */
            //longRotate+=0.1;
            zRotated+=1;
            zRotated=zRotated%360;
            //if(longRotate>=160)longRotate=longRotate%160;
            break;

        case '2':
        	if(mode_2_on == true)
        		mode_2_on = false;
        	else
        		mode_2_on = true;
        	break;

        case '3':
            if(mode_3_on == true)
                mode_3_on = false;
            else
                mode_3_on = true;
            break;

        case 'p':
            teddy_test();
            break;

        case '-':
            more = more + 1;

            mark_done = true;
            mark_sharp_edge();
            break;

        case '+':
            more = more - 1;
            if(more<0)more=0;

            mark_done = true;
            mark_sharp_edge();
            break;

        case '9':
            if(on7 == true)
                on7 = false;
            else
                on7 = true;
            break;

        case '7':
            if(on8 == true)
                on8 = false;
            else
                on8 = true;
            break;

        case '8':
            if(on == true)
                on = false;
            else
                on = true;
            break;
    }

    glutPostRedisplay();
}


/* Lighting */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

static void idle(void)
{
	//_sleep(1000);
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


/* main */
//-------------------------------------------------------------------------------------------------------------------------------------------------//

int main(int argc, char *argv[])
{
    teddy_test();//drawFlatTriangleBase() all, not only FlatTriangleBase
    rout_bone_line();

    glutInit(&argc, argv);
    glutInitWindowSize(Width,Height);
    glutInitWindowPosition(500,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GRA_Final_104753038");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    glutMouseFunc( mousebutton );           // when mouse button pressed
    glutMotionFunc( recordMousePos );       // when mouse drags around

    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
