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
	void DrawRec() 
	{
		DrawRectangleLines( boundary.x - boundary.w, boundary.y - boundary.h , boundary.w * 2, boundary.h * 2, WHITE);
		for(auto p:pointsInQt)
		{
			DrawCircle(p.x, p.y, 4, RED);
		}
		if (isDivided) 
		{
			QTlist[0].DrawRec();//0->north West
			QTlist[1].DrawRec();//1->north East
			QTlist[2].DrawRec();//2->south West
			QTlist[3].DrawRec();//3->south East

		}

		
	}
	void PrintInfo()
	{
		
		std::cout <<"x: " << boundary.x << "  ";
		std::cout <<"y: " << boundary.y << "  ";
		std::cout <<"w: " << boundary.w << "  ";
		std::cout <<"h: " << boundary.h << "  ";
		std::cout << "isdivded: " << isDivided << " \n"; int i = 0;
		std::cout << "point inside the QT\n\n";
		for(auto p :pointsInQt)
		{
			std::cout << "n:" << ++i << " " << p.x << "  :x  " << p.y << ":y\n";
		}
		if(isDivided)
		{
			std::cout << "\n***************north West**********\n";
			QTlist[0].PrintInfo();//0->north West
			std::cout << "\n***************north EAST**********\n";
			QTlist[1].PrintInfo();//1->north East
			std::cout << "\n***************south West**********\n";
			QTlist[2].PrintInfo();//2->south West
			std::cout << "\n***************South East**********\n";
			QTlist[3].PrintInfo();//3->south East
			std::cout << "\n\n\n";
		}

	

	}

};

