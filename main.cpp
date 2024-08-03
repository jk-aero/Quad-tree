#include<iostream>
#include<vector>
#include<random>
#include "raylib.h"

class Point {
public:
	int x, y;
	int size = 2;
	Point(int x, int y) :x(x), y(y)
	{
	}
};

class REC  {
public:
	float x;// we are using x and y as midpoint of the rectangle
	float y;// and the width and height is the with measured from center
	float w;// width will be half of actaul width
	float h;//
	
	REC(){}// default constructor

	REC(float x,float y, float w,float h):x(x),y(y),h(h),w(w)
	{
	}
	bool contains(Point p){
		return
		   (p.x >= x - w &&
			p.x < x + w &&
			p.y >= y - h &&
			p.y < y + h);
	}
	

};



class QuadTree {
public:
	REC boundary;
	int capacity=4;
	bool isDivided=0;
	std::vector<Point> pointsInQt;
	std::vector<QuadTree> QTlist;
	
	//0->north West
	//1->north East
	//2->south West
	//3->south East
	
		QuadTree(){}// default constructor

	QuadTree(REC boundary, int n=4):boundary(boundary),capacity(n){}

	void subDivide()
	{
		float x = this->boundary.x;
		float y = this->boundary.y;
		float w = this->boundary.w;
		float h = this->boundary.h;

		REC NW{x-w/2,y-h/2,w/2,h/2};
		REC NE{ x +w / 2,y - h / 2,w / 2,h / 2 };
		REC SW{ x - w / 2,y + h / 2,w / 2,h / 2 };
		REC SE{ x + w / 2,y + h / 2,w / 2,h / 2 };

		QuadTree NorthWest(NW);
		QuadTree NorthEast(NE);
		QuadTree SouthWest(SW);
		QuadTree SouthEast(SE);
	
		//we need to push these to the list of QuadTree
		QTlist.push_back(NorthWest);
		QTlist.push_back(NorthEast);
		QTlist.push_back(SouthWest);
		QTlist.push_back(SouthEast);

		isDivided = true;
		

	}

	void insertPoint(Point p)
	{	
		if (!boundary.contains(p)) { return; }
		if (pointsInQt.size() < capacity)//checks if the number of points exceeded or not
		{
			pointsInQt.push_back(p);

		}
		else 
		{
			if (!isDivided) // here we check if the tree is already divided
			{
				subDivide();
				
			}
			QTlist[0].insertPoint(p);//0->north West
			QTlist[1].insertPoint(p);//1->north East
			QTlist[2].insertPoint(p);//2->south West
			QTlist[3].insertPoint(p);//3->south East
			
		}

	}
}
