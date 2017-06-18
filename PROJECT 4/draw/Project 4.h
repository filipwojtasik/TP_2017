#pragma once

#include "resource.h"
#include <string.h>
#include <math.h>
const double g = 9.80665;
int time = 20;
int radius=12;
int loss = 1;
double absol(const double a)
{
	if (a > 0)return a;
	else return -a;
}
class object
{
public:

	 object(int Xo, int Yo, int mass_, bool egs)
	{
		show = egs;
		X = Xo, Y = Yo;
		mass = mass_;
		set_coords();
		speed_X = 0;
		speed_Y = 0;
	}


	bool catch_ball(Point end)
	{
		if ((dist(end) < 2 * radius) && (mass <= 5))
		{
			X = end.X;
			Y = end.Y;
			set_coords();
			return true;			
		}
		else return false;
	}


	void freefall()
	{
		speed_Y = speed_Y + g*time / 1000.;
		if (!show) 
		{
			Y = Y + speed_Y;
			X = X + speed_X;
		}
		recoil();
		set_coords();

	}
	void draw_object(HDC hdc)
	{
		Graphics graphics(hdc);			
		Pen pen(Color(255, 0, 0, 0), 5);
		graphics.DrawEllipse(&pen, shape);
		TextOutW(hdc, X, Y, 0,3);
	}
	int get_mass()
	{
		return mass;
	}
	Rect get_ball_loc()
	{
		return shape;
	}
	bool get_egs()
	{
		return show;
	}
	void set_egs(bool egs)
	{
		show = egs;
	}
	void stop()
	{
		speed_Y = 0;
	}
private:
	bool show;
	int X, Y;
	double speed_Y, speed_X;
	int mass;
	Rect shape;
	void set_coords()
	{
		shape.X = X;
		shape.Y = Y;
		shape.Width = 2*radius;
		shape.Height = 2*radius;
	}
	void recoil()
	{
		if (Y > 500) {
			
			if (absol(speed_Y) < loss*2)speed_Y = 0;
			else speed_Y = -speed_Y + loss;
			}
		if (X < 400)speed_X = -speed_X + loss;
		if (X > 1200)speed_X = -speed_X + loss;
		
	}
	double dist(Point end)
	{
		return sqrt((end.X - X)*(end.X - X) + (end.Y - Y)*(end.Y - Y));
	}
};