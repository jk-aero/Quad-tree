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
