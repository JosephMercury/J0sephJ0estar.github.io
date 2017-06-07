#include<windows.h>
#include<GL/glut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<math.h>
#include<iostream>
using namespace std;
//Esc
#define ESCAPE 27

const int num = 18;

class point//define 3-d point(vector)
{
public:
	long double x, y, z;
	int number;
	point (long double xx, long double yy, long double zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}
	point(){}
	void setPoint (long double xx, long double yy, long double zz)
	{
		x = xx;
		y = yy;
		z = zz;
	}
};

struct material//define material
{
	int type, f;
	point i_amb, i_diff, i_spec;//ambient,diffuse,specular
	point p_amb, p_diff, p_spec, p_ref;
};

class polygon//define polygon
{
public:
	long double A, B, C, D;
	point mas_obj[4], normal;//4 vertices and norm vector
	material mat;//material type
	char _type;
	polygon(){}

	//4 points
	polygon (long double x1,long double y1,long double z1,long double x2,long double y2,long double z2,
		long double x3,long double y3,long double z3,long double x4,long double y4,
		long double z4,char ch,int style)
	{
		point p1(x1,y1,z1), p2(x2,y2,z2);
		point p3(x3,y3,z3), p4(x4,y4,z4);
		mas_obj[0] = p1; mas_obj[1] = p2; mas_obj[2] = p3; mas_obj[3] = p4;
		_type = ch;
        //compute norm vector
		A = p1.y*(p2.z-p3.z) + p2.y*(p3.z-p1.z) + p3.y*(p1.z-p2.z);
		B = p1.z*(p2.x-p3.x) + p2.z*(p3.x-p1.x) + p3.z*(p1.x - p2.x);
		C = p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y - p2.y);
		D = -(p1.x*(p2.y*p3.z-p3.y*p2.z) + p2.x*(p3.y*p1.z-p1.y*p3.z) + p3.x*(p1.y*p2.z-p2.y*p1.z));
		normal.setPoint(A,B,C);
		normal = Normalize(normal);//normalize
		mat.type = style;//
	}
	//3 points, almost same as the function above
	polygon (long double x1,long double y1,long double z1,long double x2,long double y2,long double z2,
		long double x3,long double y3,long double z3,char ch,int style)
	{
		point p1(x1,y1,z1), p2(x2,y2,z2);
		point p3(x3,y3,z3);
		mas_obj[0] = p1; mas_obj[1] = p2; mas_obj[2] = p3;
		_type = ch;
		A = p1.y*(p2.z-p3.z) + p2.y*(p3.z-p1.z) + p3.y*(p1.z-p2.z);
		B = p1.z*(p2.x-p3.x) + p2.z*(p3.x-p1.x) + p3.z*(p1.x - p2.x);
		C = p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y - p2.y);
		D = -(p1.x*(p2.y*p3.z-p3.y*p2.z) + p2.x*(p3.y*p1.z-p1.y*p3.z) + p3.x*(p1.y*p2.z-p2.y*p1.z));
		normal.setPoint(A,B,C);
		normal = Normalize(normal);
		mat.type=style;
	}

	//normalize vector function
	point Normalize(point x)
	{
		long double y=0;

		y = pow(x.x,2)+pow(x.y,2)+pow(x.z,2);
		y = sqrt(y);
		x.x = x.x/y; x.y = x.y/y; x.z = x.z/y;

		return x;
	}
};

polygon objects[num];//store polygons of objects

class ray//define ray(light)
{
public:
	point dir, pixel;//direction vector&&start position
	polygon p;
	ray(){}

	ray (point pixelll, point camera)
	{
		pixel = pixelll;
		dir.setPoint(pixel.x-camera.x,pixel.y-camera.y,pixel.z-camera.z);//compute the direction from ray to camera
		dir = p.Normalize(dir);
	}


	point tracing (int number, int n, point ray, point camera, point light)//ray tracing
	{
		long double crossfire, check_crossfire;//chenk ifCrossFire
 		long double lambert, phong;
		int count;
		bool flag;
		point p_cam;//camera position
		point light_p;//light position
		point dir_light;
		point find, find2;
		point cross_mas[num];
		point ref;
		point ref_int;
		point finish_color; //final color
		finish_color.x = 0.0; finish_color.y = 0.0; finish_color.z = 0.0;//pre-define to 0,0,0

		if(number >= 3)
			return finish_color;

		count = 0;

		//for each object, check
		for (int q = 0; q < num; q++)
		{
			if(q != n)
			{
				check_crossfire = objects[q].A*ray.x + objects[q].B*ray.y + objects[q].C*ray.z;
				if (check_crossfire != 0)//if cross
				{
					crossfire =-(objects[q].A*camera.x + objects[q].B*camera.y + objects[q].C*camera.z + objects[q].D)/check_crossfire;
					find.setPoint(camera.x+ray.x*crossfire,camera.y+ray.y*crossfire,camera.z+ray.z*crossfire);
					if (Hit(q,find) != 0)//if hit, then store and plus count
					{
						cross_mas[count].setPoint(find.x,find.y,find.z);
						cross_mas[count].number = q;
						count++;
					}
				}
			}
		}

		if(count != 0)
		{
			flag = false;
			find2 = Nearest(count,cross_mas);//find nearest point
			dir_light.setPoint(light.x-find2.x,light.y-find2.y,light.z-find2.z);//compute direction
			dir_light = p.Normalize(dir_light);//normalize

		//for each object, check
		for (int q = 0; q < num; q++)
		{
			if(q != find2.number)
			{
				check_crossfire = objects[q].A*dir_light.x + objects[q].B*dir_light.y + objects[q].C*dir_light.z;
				if (check_crossfire != 0)
				{
					crossfire =-(objects[q].A*find2.x + objects[q].B*find2.y + objects[q].C*find2.z + objects[q].D)/check_crossfire;
					find.setPoint(find2.x+dir_light.x*crossfire,find2.y+dir_light.y*crossfire,find2.z+dir_light.z*crossfire);
					if (Hit(q,find) != 0)
					{
						if(point_in_segment (find, find2, light))
						{
                            flag = true;
                            break;
						}
					}
				}
			}
		}

		Material(objects, find2.number); //assign colors according to materials

		lambert = Cosine(dir_light, objects[find2.number].normal); //compute diffuse intensity

		p_cam.setPoint(camera.x-find2.x,camera.y-find2.y,camera.z-find2.z);
		p_cam = p.Normalize(p_cam);
		light_p.setPoint(find2.x-light.x,find2.y-light.y,find2.z-light.z);
		light_p = p.Normalize(light_p);
		ref = Reflected(ray, objects[find2.number].normal);
		ref = p.Normalize(ref);
		phong = Cosine(ref, p_cam);//compute reflected intensity

		if (flag == true)
		{
			finish_color.x = objects[find2.number].mat.i_amb.x * objects[find2.number].mat.p_amb.x;
			finish_color.y = objects[find2.number].mat.i_amb.y * objects[find2.number].mat.p_amb.y;
			finish_color.z = objects[find2.number].mat.i_amb.z * objects[find2.number].mat.p_amb.z;
		}
		else
		{
			finish_color.x = objects[find2.number].mat.i_amb.x * objects[find2.number].mat.p_amb.x + objects[find2.number].mat.i_diff.x * objects[find2.number].mat.p_diff.x*lambert + objects[find2.number].mat.i_spec.x * objects[find2.number].mat.p_spec.x * (pow(phong,objects[find2.number].mat.f));
			finish_color.y = objects[find2.number].mat.i_amb.y * objects[find2.number].mat.p_amb.y + objects[find2.number].mat.i_diff.y * objects[find2.number].mat.p_diff.y*lambert + objects[find2.number].mat.i_spec.y * objects[find2.number].mat.p_spec.y * (pow(phong,objects[find2.number].mat.f));
			finish_color.z = objects[find2.number].mat.i_amb.z * objects[find2.number].mat.p_amb.z + objects[find2.number].mat.i_diff.z * objects[find2.number].mat.p_diff.z*lambert + objects[find2.number].mat.i_spec.z * objects[find2.number].mat.p_spec.z * (pow(phong,objects[find2.number].mat.f));
		}

		ref_int = tracing (number+1, find2.number, ref, find2, light);

		finish_color.x += ref_int.x * objects[find2.number].mat.p_ref.x;
		finish_color.y += ref_int.y * objects[find2.number].mat.p_ref.y;
		finish_color.z += ref_int.z * objects[find2.number].mat.p_ref.z;

		return finish_color;
	}
	else
		{
			finish_color.x = 0.0;
			finish_color.y = 0.0;
			finish_color.z = 0.0;

			return finish_color;
		}
	}

	//compute reflected vector
	point Reflected (point s, point n)
	{
		point r;
		long double l;

		l = pow(n.x,2)+pow(n.y,2)+pow(n.z,2);

		r.x = n.x * 2 * (n.x * s.x + n.y * s.y + n.z * s.z)/l  - s.x;
		r.y = n.y * 2 * (n.x * s.x + n.y * s.y + n.z * s.z)/l  - s.y;
		r.z = n.z * 2 * (n.x * s.x + n.y * s.y + n.z * s.z)/l  - s.z;

		return r;
	}

	//check if a point in a segment
	bool point_in_segment (point t, point p1, point p2)
	{
		if (t.x == p1.x && t.y == p1.y && t.z == p1.z)
			return false;

		long double a, b;

		a = sqrt(pow(p2.x-t.x,2) + pow(p2.y-t.y,2) + pow(p2.z-t.z,2));
		b = sqrt(pow(p2.x-p1.x,2) + pow(p2.y-p1.y,2) + pow(p2.z-p1.z,2));

		if(a > b)
			return false;
		else
			return true;
	}

	//signal
	int Sign(long double sign)
	{
		if (sign == 0)
			return 0;

		if (sign < 0)
			return 1;
		else
			return -1;
	}

	//find nearest object
	point Nearest (int counter, point mas[num])
	{
		int min = 0;

		for(int i = 1; i < counter; i++)
		{
			if(mas[i].z > mas[min].z)
				min = i;
		}

		return mas[min];
	}

    //assign different lighting properties according to their different materials
	void Material (polygon mas[], int i)
	{
		if (mas[i].mat.type == 0)
		{
			mas[i].mat.f = 89.6;
			mas[i].mat.i_amb.x = 0.45; mas[i].mat.p_amb.x = 0.23125; mas[i].mat.i_diff.x = 0.96; mas[i].mat.p_diff.x = 0.2775; mas[i].mat.i_spec.x = 0.95; mas[i].mat.p_spec.x = 0.773911;  mas[i].mat.p_ref.x = 1.0;
			mas[i].mat.i_amb.y = 0.40; mas[i].mat.p_amb.y = 0.23125; mas[i].mat.i_diff.y = 0.98; mas[i].mat.p_diff.y = 0.2775; mas[i].mat.i_spec.y = 0.98; mas[i].mat.p_spec.y = 0.773911;  mas[i].mat.p_ref.y = 1.0;
			mas[i].mat.i_amb.z = 0.47; mas[i].mat.p_amb.z = 0.23125; mas[i].mat.i_diff.z = 0.67; mas[i].mat.p_diff.z = 0.2775; mas[i].mat.i_spec.z = 0.93; mas[i].mat.p_spec.z = 0.773911;  mas[i].mat.p_ref.z = 1.0;
		}

		if (mas[i].mat.type == 1)
		{
			mas[i].mat.f = 51.2;
			mas[i].mat.i_amb.x = 0.45; mas[i].mat.p_amb.x = 0.24725; mas[i].mat.i_diff.x = 0.96; mas[i].mat.p_diff.x = 0.75164; mas[i].mat.i_spec.x = 0.95; mas[i].mat.p_spec.x = 0.628281; mas[i].mat.p_ref.x = 0.0;
			mas[i].mat.i_amb.y = 0.40; mas[i].mat.p_amb.y = 0.1995;  mas[i].mat.i_diff.y = 0.98; mas[i].mat.p_diff.y = 0.60648; mas[i].mat.i_spec.y = 0.98; mas[i].mat.p_spec.y = 0.555802; mas[i].mat.p_ref.y = 0.0;
			mas[i].mat.i_amb.z = 0.47; mas[i].mat.p_amb.z = 0.0745;  mas[i].mat.i_diff.z = 0.67; mas[i].mat.p_diff.z = 0.22648; mas[i].mat.i_spec.z = 0.93; mas[i].mat.p_spec.z = 0.366065; mas[i].mat.p_ref.z = 0.0;
		}

		if (mas[i].mat.type == 2)
		{
			mas[i].mat.f = 27.8974;
			mas[i].mat.i_amb.x = 0.45; mas[i].mat.p_amb.x = 0.329412; mas[i].mat.i_diff.x = 0.96; mas[i].mat.p_diff.x = 0.780392; mas[i].mat.i_spec.x = 0.95; mas[i].mat.p_spec.x = 0.992157;  mas[i].mat.p_ref.x = 0.0;
			mas[i].mat.i_amb.y = 0.40; mas[i].mat.p_amb.y = 0.223529;  mas[i].mat.i_diff.y = 0.98; mas[i].mat.p_diff.y = 0.568627; mas[i].mat.i_spec.y = 0.98; mas[i].mat.p_spec.y = 0.941176;  mas[i].mat.p_ref.y =0.0;
			mas[i].mat.i_amb.z = 0.47; mas[i].mat.p_amb.z = 0.027451;  mas[i].mat.i_diff.z = 0.67; mas[i].mat.p_diff.z = 0.113725; mas[i].mat.i_spec.z = 0.93; mas[i].mat.p_spec.z = 0.807843;  mas[i].mat.p_ref.z = 0.0;
		}

		if (mas[i].mat.type == 3)
		{
			mas[i].mat.f = 12.8;
			mas[i].mat.i_amb.x = 0.45; mas[i].mat.p_amb.x = 0.19125; mas[i].mat.i_diff.x = 0.96; mas[i].mat.p_diff.x = 0.7038; mas[i].mat.i_spec.x = 0.95; mas[i].mat.p_spec.x = 0.256777;  mas[i].mat.p_ref.x = 0.0;
			mas[i].mat.i_amb.y = 0.40; mas[i].mat.p_amb.y = 0.0735;  mas[i].mat.i_diff.y = 0.98; mas[i].mat.p_diff.y = 0.27048; mas[i].mat.i_spec.y = 0.98; mas[i].mat.p_spec.y = 0.137622;  mas[i].mat.p_ref.y =0.0;
			mas[i].mat.i_amb.z = 0.47; mas[i].mat.p_amb.z = 0.0225;  mas[i].mat.i_diff.z = 0.67; mas[i].mat.p_diff.z = 0.0828; mas[i].mat.i_spec.z = 0.93; mas[i].mat.p_spec.z = 0.086014;  mas[i].mat.p_ref.z = 0.0;
		}

		if (mas[i].mat.type == 4)
		{
			mas[i].mat.f = 76.8;
			mas[i].mat.i_amb.x = 0.45; mas[i].mat.p_amb.x = 0.25; mas[i].mat.i_diff.x = 0.96; mas[i].mat.p_diff.x = 0.4; mas[i].mat.i_spec.x = 0.95; mas[i].mat.p_spec.x = 0.774597;  mas[i].mat.p_ref.x = 0.0;
			mas[i].mat.i_amb.y = 0.40; mas[i].mat.p_amb.y = 0.25;  mas[i].mat.i_diff.y = 0.98; mas[i].mat.p_diff.y = 0.4; mas[i].mat.i_spec.y = 0.98; mas[i].mat.p_spec.y = 0.774597;  mas[i].mat.p_ref.y =0.0;
			mas[i].mat.i_amb.z = 0.47; mas[i].mat.p_amb.z = 0.25;  mas[i].mat.i_diff.z = 0.67; mas[i].mat.p_diff.z = 0.4; mas[i].mat.i_spec.z = 0.93; mas[i].mat.p_spec.z = 0.774597;  mas[i].mat.p_ref.z = 0.0;
		}
	}

	//product of two vector
	long double Cosine(point dir_l, point norm)
	{
		long double product = 0;
		long double norm1 = 0;
		long double norm2 = 0;
		long double result = 0;

		product = norm.x * dir_l.x + norm.y * dir_l.y + norm.z * dir_l.z;
		norm1 = pow(dir_l.x,2) + pow(dir_l.y,2) + pow(dir_l.z,2);
		norm1 = sqrt(norm1);
		norm2 = pow(norm.x,2) + pow(norm.y,2) + pow(norm.z,2);
		norm2 = sqrt(norm2);
		result = product/(norm1 * norm2);
		result*=-1;

		return result;
	}

	double Hit (int n, point finder)
	{
		if (objects[n]._type == 'p')
		{
		long double side1, side2, side3, side4, sides;
		if (abs(objects[n].normal.x) >= abs(objects[n].normal.y) && abs(objects[n].normal.x) >= abs(objects[n].normal.z))
		{
			//YZ
			side1 = (finder.z - objects[n].mas_obj[0].z)*(objects[n].mas_obj[1].y - objects[n].mas_obj[0].y) - (finder.y-objects[n].mas_obj[0].y)*(objects[n].mas_obj[1].z - objects[n].mas_obj[0].z);
			side2 = (finder.z - objects[n].mas_obj[1].z)*(objects[n].mas_obj[2].y - objects[n].mas_obj[1].y) - (finder.y-objects[n].mas_obj[1].y)*(objects[n].mas_obj[2].z - objects[n].mas_obj[1].z);
			side3 = (finder.z - objects[n].mas_obj[2].z)*(objects[n].mas_obj[3].y - objects[n].mas_obj[2].y) - (finder.y-objects[n].mas_obj[2].y)*(objects[n].mas_obj[3].z - objects[n].mas_obj[2].z);
			side4 = (finder.z - objects[n].mas_obj[3].z)*(objects[n].mas_obj[0].y - objects[n].mas_obj[3].y) - (finder.y-objects[n].mas_obj[3].y)*(objects[n].mas_obj[0].z - objects[n].mas_obj[3].z);
			sides = abs(Sign(side1) + Sign(side2) + Sign(side3) + Sign(side4));
			if (sides == 4) { return(1);}
			else return(0);
		}

		if (abs(objects[n].normal.y) >= abs(objects[n].normal.z) && abs(objects[n].normal.y) >= abs(objects[n].normal.x))
		{
			//XZ
			side1 = (finder.z - objects[n].mas_obj[0].z)*(objects[n].mas_obj[1].x - objects[n].mas_obj[0].x) - (finder.x-objects[n].mas_obj[0].x)*(objects[n].mas_obj[1].z - objects[n].mas_obj[0].z);
			side2 = (finder.z - objects[n].mas_obj[1].z)*(objects[n].mas_obj[2].x - objects[n].mas_obj[1].x) - (finder.x-objects[n].mas_obj[1].x)*(objects[n].mas_obj[2].z - objects[n].mas_obj[1].z);
			side3 = (finder.z - objects[n].mas_obj[2].z)*(objects[n].mas_obj[3].x - objects[n].mas_obj[2].x) - (finder.x-objects[n].mas_obj[2].x)*(objects[n].mas_obj[3].z - objects[n].mas_obj[2].z);
			side4 = (finder.z - objects[n].mas_obj[3].z)*(objects[n].mas_obj[0].x - objects[n].mas_obj[3].x) - (finder.x-objects[n].mas_obj[3].x)*(objects[n].mas_obj[0].z - objects[n].mas_obj[3].z);
			sides = abs(Sign(side1) + Sign(side2) + Sign(side3) + Sign(side4));
			if (sides == 4) { return(1);}
			else return(0);
		}

		 //XY
		side1 = (finder.y - objects[n].mas_obj[0].y)*(objects[n].mas_obj[1].x - objects[n].mas_obj[0].x) - (finder.x-objects[n].mas_obj[0].x)*(objects[n].mas_obj[1].y - objects[n].mas_obj[0].y);
		side2 = (finder.y - objects[n].mas_obj[1].y)*(objects[n].mas_obj[2].x - objects[n].mas_obj[1].x) - (finder.x-objects[n].mas_obj[1].x)*(objects[n].mas_obj[2].y - objects[n].mas_obj[1].y);
		side3 = (finder.y - objects[n].mas_obj[2].y)*(objects[n].mas_obj[3].x - objects[n].mas_obj[2].x) - (finder.x-objects[n].mas_obj[2].x)*(objects[n].mas_obj[3].y - objects[n].mas_obj[2].y);
		side4 = (finder.y - objects[n].mas_obj[3].y)*(objects[n].mas_obj[0].x - objects[n].mas_obj[3].x) - (finder.x-objects[n].mas_obj[3].x)*(objects[n].mas_obj[0].y - objects[n].mas_obj[3].y);
		sides = abs(Sign(side1) + Sign(side2) + Sign(side3) + Sign(side4));
		if (sides == 4) {return(1);}
		else return(0);
		}
		else
		{
			long double side1, side2, side3, sides;
		if (abs(objects[n].normal.x) >= abs(objects[n].normal.y) && abs(objects[n].normal.x) >= abs(objects[n].normal.z))
		{
			//  YZ
			side1 = (finder.z - objects[n].mas_obj[0].z)*(objects[n].mas_obj[1].y - objects[n].mas_obj[0].y) - (finder.y-objects[n].mas_obj[0].y)*(objects[n].mas_obj[1].z - objects[n].mas_obj[0].z);
			side2 = (finder.z - objects[n].mas_obj[1].z)*(objects[n].mas_obj[2].y - objects[n].mas_obj[1].y) - (finder.y-objects[n].mas_obj[1].y)*(objects[n].mas_obj[2].z - objects[n].mas_obj[1].z);
			side3 = (finder.z - objects[n].mas_obj[2].z)*(objects[n].mas_obj[0].y - objects[n].mas_obj[2].y) - (finder.y-objects[n].mas_obj[2].y)*(objects[n].mas_obj[0].z - objects[n].mas_obj[2].z);
			sides = abs(Sign(side1) + Sign(side2) + Sign(side3));
			if (sides == 3) { return(1);}
			else return(0);
		}

		if (abs(objects[n].normal.y) >= abs(objects[n].normal.z) && abs(objects[n].normal.y) >= abs(objects[n].normal.x))
		{
			//  XZ
			side1 = (finder.z - objects[n].mas_obj[0].z)*(objects[n].mas_obj[1].x - objects[n].mas_obj[0].x) - (finder.x-objects[n].mas_obj[0].x)*(objects[n].mas_obj[1].z - objects[n].mas_obj[0].z);
			side2 = (finder.z - objects[n].mas_obj[1].z)*(objects[n].mas_obj[2].x - objects[n].mas_obj[1].x) - (finder.x-objects[n].mas_obj[1].x)*(objects[n].mas_obj[2].z - objects[n].mas_obj[1].z);
			side3 = (finder.z - objects[n].mas_obj[2].z)*(objects[n].mas_obj[0].x - objects[n].mas_obj[2].x) - (finder.x-objects[n].mas_obj[2].x)*(objects[n].mas_obj[0].z - objects[n].mas_obj[2].z);
			sides = abs(Sign(side1) + Sign(side2) + Sign(side3));
			if (sides == 3) { return(1);}
			else return(0);
		}

		 // XY
		side1 = (finder.y - objects[n].mas_obj[0].y)*(objects[n].mas_obj[1].x - objects[n].mas_obj[0].x) - (finder.x-objects[n].mas_obj[0].x)*(objects[n].mas_obj[1].y - objects[n].mas_obj[0].y);
		side2 = (finder.y - objects[n].mas_obj[1].y)*(objects[n].mas_obj[2].x - objects[n].mas_obj[1].x) - (finder.x-objects[n].mas_obj[1].x)*(objects[n].mas_obj[2].y - objects[n].mas_obj[1].y);
		side3 = (finder.y - objects[n].mas_obj[2].y)*(objects[n].mas_obj[0].x - objects[n].mas_obj[2].x) - (finder.x-objects[n].mas_obj[2].x)*(objects[n].mas_obj[0].y - objects[n].mas_obj[2].y);
		sides = abs(Sign(side1) + Sign(side2) + Sign(side3));
		if (sides == 3) {return(1);}
		else return(0);
		}
	}
};

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	point light(550.0,600.0,400.0); //light position
	point camera(220.0,500.0,600.0); //camera position
	point color;

	//draw objects
	polygon floor(0.0, 100.0, 200.0, 0.0, 100.0, -500.0, 900.0, 100.0, -500.0, 900.0, 100.0, 200.0, 'p' ,0);   objects[0] = floor;
	polygon right_wall(900.0, 100.0, -500.0, 900.0, 600.0, -500.0, 900.0, 600.0, 200.0, 900.0, 100.0, 200.0, 'p' ,4);   objects[1] = right_wall;
	polygon left_wall(0.0, 100.0, 200.0, 0.0, 600.0, 200.0, 0.0, 600.0, -500.0, 0.0, 100.0, -500.0, 'p', 4);   objects[2] = left_wall;
	polygon back_wall(0.0, 100.0, -500.0, 0.0, 600.0, -500.0, 900.0, 600.0, -500.0, 900.0, 100.0, -500.0, 'p',4);   objects[3] = back_wall;


	polygon cube_front(100.0, 100.0, -100.0, 100.0, 300.0, -100.0, 250.0, 300.0, -100.0, 250.0, 100.0, -100.0, 'p', 1);   objects[4]=cube_front;
	polygon cube_back(150.0, 100.0, -250.0, 150.0, 300.0, -250.0, 300.0, 300.0, -250.0, 300.0, 100.0, -250.0, 'p', 1);   objects[5]=cube_back;
	polygon cube_left(100.0, 100.0, -100.0, 150.0, 100.0, -250.0, 150.0, 300.0, -250.0, 100.0, 300.0, -100.0, 'p', 1);   objects[6]=cube_left;
	polygon cube_right(250.0, 100.0, -100.0, 250.0, 300.0, -100.0, 300.0, 300.0, -250.0, 300.0, 100.0, -250.0, 'p', 1);   objects[7]=cube_right;
	polygon cube_roof(100.0, 300.0, -100.0, 150.0, 300.0, -250.0, 300.0, 300.0, -250.0, 250.0, 300.0, -100.0, 'p', 1);   objects[8]=cube_roof;


	polygon tr_front(400.0, 100.0, -80.0, 450.0, 400.0, -100.0, 550.0, 100.0, -100.0, 't' , 2);   objects[9]=tr_front;
	polygon tr_left(350.0, 100.0, -250.0, 450.0, 400.0, -100.0, 400.0, 100.0, -80.0, 't' , 2);   objects[10]=tr_left;
	polygon tr_right(500.0, 100.0, -270.0, 450.0, 400.0, -100.0, 550.0, 100.0, -100.0, 't' , 2);   objects[11]=tr_right;
	polygon tr_back(350.0, 100.0, -250.0, 500.0, 100.0, -270.0, 450.0, 400.0, -100.0, 't' , 2);   objects[12]=tr_back;


	polygon parallelepiped_front(590.0, 100.0, -100.0, 590.0, 370.0, -100.0, 800.0, 370.0, -100.0, 800.0, 100.0, -100.0, 'p', 3);   objects[13]=parallelepiped_front;
	polygon parallelepiped_back(590.0, 370.0, -250.0, 800.0, 370.0, -250.0, 800.0, 100.0, -250.0, 590.0, 100.0, -250.0, 'p', 3);   objects[14]=parallelepiped_back;
	polygon parallelepiped_left(590.0, 370.0, -100.0, 590.0, 100.0, -100.0, 590.0, 100.0, -250.0, 590.0, 370.0, -250.0, 'p', 3);   objects[15]=parallelepiped_left;
	polygon parallelepiped_right(800.0, 100.0, -100.0, 800.0, 370.0, -100.0, 800.0, 370.0, -250.0, 800.0, 100.0, -250.0, 'p', 3);   objects[16]=parallelepiped_right;
	polygon parallelepiped_roof(590.0, 370.0, -100.0, 590.0, 370.0, -250.0, 800.0, 370.0, -250.0, 800.0, 370.0, -100.0, 'p', 3);   objects[17]=parallelepiped_roof;

	//begin ray tracing, from right to lefe/up to down
	for (double weight = 800; weight > 0; weight--)
	{
		double height = 600;
		for (; height > 0; height--)
		{
			point pixell(weight, height, 0);
			ray r(pixell, camera);
			color = r.tracing(0, -1, r.dir, camera, light);

			glBegin(GL_POINTS);
			glColor3f(color.x, color.y, color.z);
			glVertex3f(pixell.x, pixell.y, pixell.z);
			glEnd();
		}
	 }
	 glutSwapBuffers();
 }

 //enable press 'esc' to quit
    int window;
    static void glutKeyboard(unsigned char key,int x,int y) {
        switch(key)
            case ESCAPE:
                glutDestroyWindow(window);
                //exit normally
                exit(0);
    }

 //main function
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	window=glutCreateWindow("ray_tracing");
	glOrtho(000.0,800.0,000.0,600.0,1.0,-1.0);
	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	return 0;
}
