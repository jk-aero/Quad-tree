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
	bool intersects(REC range )//wer are testing if both the rectangle intersect
	{
		return(
			range.x - range.w > x + w ||
			range.y + range.y > y + h ||
			range.x - range.w < x + w ||
			range.x + range.w > x + w
			);
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

	void query(REC range, std::vector<Point>&found)// here we pass a rectangle and see what all quad trees are intersecting with it
	{
		
		if (!boundary.intersects(range)) { return ; }// see if that range intersects with other quadTree
		
		else {									// if the boundary intersects
												//then we are insert  all the points 
			for (auto P : pointsInQt)			// from the QT to this found array 
			{	
				if (range.contains(P)) { found.push_back(P); }
			}
			
			if(isDivided)
			{
				 QTlist[0].query(range,found);
				 QTlist[1].query(range,found);
				 QTlist[2].query(range,found);
				 QTlist[3].query(range,found);
			


				//QTlist[0].query(range);

			}
			

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





std::vector<Point> points;

int main() {
	const int screenW = 800, screenH = 800;
	REC boundary{ screenW / 2,screenH / 2, screenW / 2,screenH / 2, };
	QuadTree QT(boundary);

	for (int i = 0; i < 300; i++)
	{
		Point p{ (int)std::rand() % screenW,(int)std::rand() % screenW };
		points.push_back(p);
		QT.insertPoint(p);
		std::cout << "n:" << i << " " << points[i].x << "  :x  " << points[i].y << ":y\n";

	}

	//QT.PrintInfo(); for debugging purposes
	
	//std::cout << "total number of points inside:" << pointsInRange.size()<<"\n\n\n";
	
	InitWindow(screenW, screenH, "raylib [core] example - basic window");

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	 while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);
		QT.DrawRec();
		
		std::vector<Point> pointsInRange;
		REC range{ (float)GetMouseX(), (float)GetMouseY(), 111, 156 }; std::vector<Point>points;
		QT.query(range, pointsInRange);
		DrawRectangleLines(range.x-range.w, range.y-range.h, range.w*2, range.h*2, GREEN);
		for(auto p: pointsInRange)
		{
			DrawCircle(p.x, p.y, 4, GREEN);
		}
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // C
	
	
	return 0;
}
