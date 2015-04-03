/***************************************************************************
 *   Copyright (C) 2006 by Frederic MARTIN                                 *
 *   martin-frederic@users.sourceforge.net                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "SDL/SDL_opengl.h"
#include <math.h>
#include "raytracing.h"

#ifndef pi
#define pi 3.14159265358979
#endif


const double radian = (double) (pi / 180);
const double epsilon = (double) (0.05);


typedef double SCALAR;

class VECTOR
{
	public:
		VECTOR(double sx = 0, double sy = 0, double sz = 0);
		VECTOR(const VECTOR& Vector);
		~VECTOR();

		double GetMagnitude();
		GLvoid Normalize();
		GLvoid Reset();
		GLvoid Set(double sx, double sy, double sz) {x = sx, y = sy, z = sz;}
		GLvoid CrossVector(VECTOR vect);
		double DotProduct(VECTOR vect);

        //equal within an error e
		const bool nearlyEquals( const VECTOR& v, const SCALAR e ) const
		{
			return fabs(x-v.x)<e && fabs(y-v.y)<e && fabs(z-v.z)<e;
		}

        //cross product
		const VECTOR cross( const VECTOR& v ) const
		{
            //Davis, Snider, "Introduction to Vector Analysis", p. 44
			return VECTOR( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x );
		}

        //scalar dot product
		const SCALAR dot( const VECTOR& v ) const
		{
			return x*v.x + y*v.y + z*v.z;
		}

        //length
		const SCALAR length() const
		{
			return (SCALAR)sqrt( (double)this->dot(*this) );
		}

        //unit vector
		const VECTOR unit() const
		{
			return (*this) / length();
		}

        //make this a unit vector
		void normalize()
		{
			(*this) /= length();
		}

        //Members
		double x;
		double y;
		double z;

        //index a component
        //NOTE: returning a reference allows
        //you to assign the indexed element
		SCALAR& operator [] ( const long i )
		{
			return *((&x) + i);
		}

        //compare
		const bool operator == ( const VECTOR& v ) const
		{
			return (v.x==x && v.y==y && v.z==z);
		}

		const bool operator != ( const VECTOR& v ) const
		{
			return !(v == *this);
		}

        //negate
		const VECTOR operator - () const
		{
			return VECTOR( -x, -y, -z );
		}

        //assign
		const VECTOR& operator = ( const VECTOR& v )
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

        //increment
		const VECTOR& operator += ( const VECTOR& v )
		{
			x+=v.x;
			y+=v.y;
			z+=v.z;
			return *this;
		}

        //decrement
		const VECTOR& operator -= ( const VECTOR& v )
		{
			x-=v.x;
			y-=v.y;
			z-=v.z;
			return *this;
		}

        //self-multiply
		const VECTOR& operator *= ( const SCALAR& s )
		{
			x*=s;
			y*=s;
			z*=s;
			return *this;
		}

        //self-divide
		const VECTOR& operator /= ( const SCALAR& s )
		{
			const SCALAR r = 1 / s;
			x *= r;
			y *= r;
			z *= r;
			return *this;
		}

        //add
		const VECTOR operator + ( const VECTOR& v ) const
		{
			return VECTOR(x + v.x, y + v.y, z + v.z);
		}

        //subtract
		const VECTOR operator - ( const VECTOR& v ) const
		{
			return VECTOR(x - v.x, y - v.y, z - v.z);
		}

        //post-multiply by a scalar
		const VECTOR operator * ( const SCALAR& s ) const
		{
			return VECTOR( x*s, y*s, z*s );
		}

        //pre-multiply by a scalar
		friend inline const VECTOR operator * ( const SCALAR& s, const VECTOR& v )
		{
			return v * s;
		}

        //divide
		const VECTOR operator / (SCALAR s) const
		{
			s = 1/s;
			return VECTOR( s*x, s*y, s*z );
		}
};



class VERTEX
{
	public:
		VERTEX(double x = 0, double y = 0, double z = 0, double nx = 0, double ny = 0, double nz = 0);
		~VERTEX();

		VECTOR coords;
		VECTOR normal;
		double u;
		double v;

        // operator overloading
		VERTEX& operator = (const VERTEX& Vertex)
		{
			coords = Vertex.coords;
			normal = Vertex.normal;
			u = Vertex.u;
			v = Vertex.v;
			return *this; 
		}
};


class QUAD
{
	public:
		QUAD();
		~QUAD();

		VECTOR GetNormal();
		GLvoid SetNormal();

		VERTEX Vertex[4];
		int numVertices;
};


class QUAT
{
	public:
		QUAT(double sx = 0, double sy = 0, double sz = 0, double sw = 1);
		~QUAT();

		GLvoid Reset();
		GLvoid CopyQuat(QUAT q);
		GLvoid Set(double sx, double sy, double sz, double sw) {x = sx, y = sy, z = sz, w = sw;}
		GLvoid AxisAngleToQuat(VECTOR axis, double theta);
		GLvoid EulerToQuat(double pitch, double yaw, double roll);
		GLvoid NormaliseQuat();
		double MagnitudeQuat();
		GLvoid MultQuat(QUAT q);                

		double x;
		double y;
		double z;
		double w;
};




class MATRIX
{
	public:
		MATRIX();
		~MATRIX();

		GLvoid LoadIdentity();
		GLvoid CopyMatrix(double m[16]);
		GLvoid MultMatrix(double m[16]);
		GLvoid MatrixInverse(); 
		GLvoid MatrixFromAxisAngle(VECTOR axis, double theta);
		GLvoid QuatToMatrix(QUAT quat); 

		double Element[16];
};





class OBJECT
{
	public:
		OBJECT();
		~OBJECT();
                
		GLvoid Reset();
		GLvoid Rotate();
		GLvoid Draw();
		GLvoid UpdatePosition();
		GLvoid UpdatePosition(double x, double y, double z); 
		GLvoid MoveX(); 
		GLvoid MoveY(); 
		GLvoid MoveZ(); 
		GLvoid MoveX(double x); 
		GLvoid MoveY(double y); 
		GLvoid MoveZ(double z); 
		VECTOR GetXUnit();                
		VECTOR GetYUnit();                
		VECTOR GetZUnit();

		QUAT Orientation;
		VECTOR Position;
		double Delta_x;   //Rotation deltas  
		double Delta_y;
		double Delta_z;
		double Movement_x;    //Movement displacements
		double Movement_y;
		double Movement_z;
		double Multiplier;
};


class CAMERA : public OBJECT
{
	public:
		CAMERA();
		~CAMERA();
                
		GLvoid Reset();
		GLvoid Update();
		GLvoid Apply();

		MATRIX Matrix;
};








VECTOR::VECTOR(double sx, double sy, double sz)
:
	x(sx),
	y(sy),
	z(sz)
{
}

VECTOR::VECTOR(const VECTOR& Vector)
{
	x = Vector.x;
	y = Vector.y;
	z = Vector.z;
}

VECTOR::~VECTOR()
{
}

GLvoid VECTOR::Reset()
{
	x = 0;
	y = 0;
	z = 0;
}

double VECTOR::DotProduct(VECTOR vect)
{
  	double dot;
  	dot = vect.x * x + vect.y * y + vect.z * z;
  	return dot;
}

GLvoid VECTOR::CrossVector(VECTOR vect)
{
  	VECTOR temp = *this;
  	x = vect.y * temp.z - vect.z * temp.y;
  	y = vect.z * temp.x - vect.x * temp.z;
  	z = vect.x * temp.y - vect.y * temp.x;
}

double VECTOR::GetMagnitude()
{
	double presque_rien=0.0000000000001f;
    double magnitude = sqrt(x * x + y * y + z * z);
    if (magnitude > presque_rien)
        return magnitude;
    else
        return  presque_rien;
}

GLvoid VECTOR::Normalize()
{
    double magnitude = this->GetMagnitude();
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}



VERTEX::VERTEX(double sx, double sy, double sz, double snx, double sny, double snz)
{
	coords.x = sx;
	coords.y = sy;
	coords.z = sz;
	normal.x = snx;
	normal.y = sny;
	normal.z = snz;
}

VERTEX::~VERTEX()
{
}


QUAD::QUAD()
{
	numVertices=4;
}

QUAD::~QUAD()
{
}

VECTOR QUAD::GetNormal()
{
        VECTOR temp; 
    	double ux;
    	double uy;
    	double uz;
    	double vx;
    	double vy;
    	double vz;
      	ux = Vertex[1].coords.x - Vertex[0].coords.x;
      	uy = Vertex[1].coords.y - Vertex[0].coords.y;
      	uz = Vertex[1].coords.z - Vertex[0].coords.z;
      	vx = Vertex[2].coords.x - Vertex[0].coords.x;
      	vy = Vertex[2].coords.y - Vertex[0].coords.y;
      	vz = Vertex[2].coords.z - Vertex[0].coords.z;
      	temp.x = (uy*vz)-(vy*uz);
      	temp.y = (uz*vx)-(vz*ux);
      	temp.z = (ux*vy)-(vx*uy);
        return temp;
}

GLvoid QUAD::SetNormal()
{
    	double ux;
    	double uy;
    	double uz;
    	double vx;
    	double vy;
    	double vz;
      	ux = Vertex[1].coords.x - Vertex[0].coords.x;
      	uy = Vertex[1].coords.y - Vertex[0].coords.y;
      	uz = Vertex[1].coords.z - Vertex[0].coords.z;
      	vx = Vertex[2].coords.x - Vertex[0].coords.x;
      	vy = Vertex[2].coords.y - Vertex[0].coords.y;
      	vz = Vertex[2].coords.z - Vertex[0].coords.z;
      	Vertex[0].normal.x = (uy*vz)-(vy*uz);
      	Vertex[0].normal.y = (uz*vx)-(vz*ux);
      	Vertex[0].normal.z = (ux*vy)-(vx*uy);
        Vertex[1].normal = Vertex[0].normal;
      	Vertex[2].normal = Vertex[0].normal;
		Vertex[3].normal = Vertex[0].normal;
}



MATRIX::MATRIX()
{
	LoadIdentity();
}

MATRIX::~MATRIX()
{
}

GLvoid MATRIX::LoadIdentity()
{
	Element[0]=1.0f;
    	Element[1]=0.0f;
    	Element[2]=0.0f;
    	Element[3]=0.0f;

    	Element[4]=0.0f;
    	Element[5]=1.0f;
    	Element[6]=0.0f;
    	Element[7]=0.0f;

    	Element[8]=0.0f;
    	Element[9]=0.0f;
    	Element[10]=1.0f;
    	Element[11]=0.0f;

    	Element[12]=0.0f;
    	Element[13]=0.0f;
    	Element[14]=0.0f;
    	Element[15]=1.0f;
}

GLvoid MATRIX::CopyMatrix(double m[16])
{
    	Element[0 ] = m[0 ];
    	Element[1 ] = m[1 ];
    	Element[2 ] = m[2 ];
    	Element[3 ] = m[3 ];
    	Element[4 ] = m[4 ];
    	Element[5 ] = m[5 ];
    	Element[6 ] = m[6 ];
    	Element[7 ] = m[7 ];
    	Element[8 ] = m[8 ];
    	Element[9 ] = m[9 ];
    	Element[10] = m[10];
    	Element[11] = m[11];
    	Element[12] = m[12];
    	Element[13] = m[13];
    	Element[14] = m[14];
    	Element[15] = m[15];
}

GLvoid MATRIX::MultMatrix(double m[])
{
    	MATRIX temp;

    	temp.CopyMatrix(this->Element);

  	Element[0] = temp.Element[0 ]*m[0 ]
        	+ temp.Element[4 ]*m[1 ]
        	+ temp.Element[8 ]*m[2 ]
        	+ temp.Element[12]*m[3 ];

  	Element[1] = temp.Element[1 ]*m[0 ]
        	+ temp.Element[5 ]*m[1 ]
        	+ temp.Element[9 ]*m[2 ]
        	+ temp.Element[13]*m[3 ];

  	Element[2] = temp.Element[2 ]*m[0 ]
       		+ temp.Element[6 ]*m[1 ]
       		+ temp.Element[10]*m[2 ]
       		+ temp.Element[14]*m[3 ];

	Element[3] = temp.Element[3 ]*m[0 ]
       		+ temp.Element[7 ]*m[1 ]
       		+ temp.Element[11]*m[2 ]
       		+ temp.Element[15]*m[3 ];

  	Element[4] = temp.Element[0 ]*m[4 ]
       		+ temp.Element[4 ]*m[5 ]
       		+ temp.Element[8 ]*m[6 ]
       		+ temp.Element[12]*m[7 ];

  	Element[5] = temp.Element[1 ]*m[4 ]
       		+ temp.Element[5 ]*m[5 ]
       		+ temp.Element[9 ]*m[6 ]
       		+ temp.Element[13]*m[7 ];

  	Element[6] = temp.Element[2 ]*m[4 ]
       		+ temp.Element[6 ]*m[5 ]
       		+ temp.Element[10]*m[6 ]
       		+ temp.Element[14]*m[7 ];

  	Element[7] = temp.Element[3 ]*m[4 ]
       		+ temp.Element[7 ]*m[5 ]
       		+ temp.Element[11]*m[6 ]
       		+ temp.Element[15]*m[7 ];

  	Element[8] = temp.Element[0 ]*m[8 ]
       		+ temp.Element[4 ]*m[9 ]
       		+ temp.Element[8 ]*m[10]
       		+ temp.Element[12]*m[11];

  	Element[9] = temp.Element[1 ]*m[8 ]
       		+ temp.Element[5 ]*m[9 ]
       		+ temp.Element[9 ]*m[10]
       		+ temp.Element[13]*m[11];

  	Element[10]= temp.Element[2 ]*m[8 ]
       		+ temp.Element[6 ]*m[9 ]
       		+ temp.Element[10]*m[10]
       		+ temp.Element[14]*m[11];

  	Element[11]= temp.Element[3 ]*m[8 ]
       		+ temp.Element[7 ]*m[9 ]
       		+ temp.Element[11]*m[10]
       		+ temp.Element[15]*m[11];

  	Element[12]= temp.Element[0 ]*m[12]
       		+ temp.Element[4 ]*m[13]
       		+ temp.Element[8 ]*m[14]
       		+ temp.Element[12]*m[15];

  	Element[13]= temp.Element[1 ]*m[12]
       		+ temp.Element[5 ]*m[13]
       		+ temp.Element[9 ]*m[14]
       		+ temp.Element[13]*m[15];

  	Element[14]= temp.Element[2 ]*m[12]
       		+ temp.Element[6 ]*m[13]
       		+ temp.Element[10]*m[14]
       		+ temp.Element[14]*m[15];

  	Element[15]= temp.Element[3 ]*m[12]
       		+ temp.Element[7 ]*m[13]
       		+ temp.Element[11]*m[14]
       		+ temp.Element[15]*m[15];
}

GLvoid MATRIX::MatrixInverse()
 {
  	MATRIX temp;

  	temp.CopyMatrix(this->Element);

  	Element[0 ] = temp.Element[0 ];
  	Element[1 ] = temp.Element[4 ];
  	Element[2 ] = temp.Element[8 ];

  	Element[4 ] = temp.Element[1 ];
  	Element[5 ] = temp.Element[5 ];
  	Element[6 ] = temp.Element[9 ];

  	Element[8 ] = temp.Element[2 ];
  	Element[9 ] = temp.Element[6 ];
  	Element[10] = temp.Element[10];

  	Element[12] *= -1.0f;
  	Element[13] *= -1.0f;
  	Element[14] *= -1.0f;
}

GLvoid MATRIX::MatrixFromAxisAngle(VECTOR axis, double theta)
{
        QUAT q;
        double halfTheta =  theta * 0.5;
        double cosHalfTheta =  (cos(halfTheta));
        double sinHalfTheta =  sin(halfTheta);
        double xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
        q.x = axis.x * sinHalfTheta;
        q.y = axis.y * sinHalfTheta;
        q.z = axis.z * sinHalfTheta;
        q.w = cosHalfTheta;
        xs = q.x * 2;  ys = q.y * 2;  zs = q.z * 2;
        wx = q.w * xs; wy = q.w * ys; wz = q.w * zs;
        xx = q.x * xs; xy = q.x * ys; xz = q.x * zs;
        yy = q.y * ys; yz = q.y * zs; zz = q.z * zs;
        Element[0] = 1 - (yy + zz);
        Element[1] = xy - wz;
        Element[2] = xz + wy;
        Element[4] = xy + wz;
        Element[5] = 1 - (xx + zz);
        Element[6] = yz - wx;
        Element[8] = xz - wy;
        Element[9] = yz + wx;
        Element[10] = 1 - (xx + yy);
        Element[12] = Element[13] = Element[14] = Element[3] = Element[7] = Element[11] = 0;
        Element[15] = 1;
}

GLvoid MATRIX::QuatToMatrix(QUAT quat)
{
  	double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
  	// calculate coefficients
  	x2 = quat.x + quat.x;
  	y2 = quat.y + quat.y;
  	z2 = quat.z + quat.z;
  	xx = quat.x * x2;
  	xy = quat.x * y2;
  	xz = quat.x * z2;
  	yy = quat.y * y2;
  	yz = quat.y * z2;
  	zz = quat.z * z2;
  	wx = quat.w * x2;
  	wy = quat.w * y2;
  	wz = quat.w * z2;
  	Element[0] = (double) (1.0 - (yy + zz));
  	Element[1] = (double) (xy - wz);
  	Element[2] = (double) (xz + wy);
  	Element[3] = 0.0;
  	Element[4] = xy + wz;
  	Element[5] = (double) (1.0 - (xx + zz));
  	Element[6] = yz - wx;
  	Element[7] = 0.0;
  	Element[8] = xz - wy;
  	Element[9] = yz + wx;
  	Element[10] = (double) (1.0 - (xx + yy));
  	Element[11] = 0.0;
  	Element[12] = 0;
  	Element[13] = 0;
  	Element[14] = 0;
  	Element[15] = 1;
}






QUAT::QUAT(double sx, double sy, double sz, double sw)
:
	x(sx),
	y(sy),
	z(sz),
    w(sw)
{
}

QUAT::~QUAT()
{
}

GLvoid QUAT::Reset()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 1.0;
}

GLvoid QUAT::CopyQuat(QUAT q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

GLvoid QUAT::AxisAngleToQuat(VECTOR axis, double theta)
{
        double halfTheta = (double) (theta * 0.5);
        double cosHalfTheta = (double) (cos(halfTheta));
        double sinHalfTheta = (double) (sin(halfTheta));
        x = axis.x * sinHalfTheta;
        y = axis.y * sinHalfTheta;
        z = axis.z * sinHalfTheta;
        w = cosHalfTheta;
}

GLvoid QUAT::EulerToQuat(double roll, double pitch, double yaw)
{
    	double cr, cp, cy, sr, sp, sy, cpcy, spsy;  // calculate trig identities
    	cr = (double) (cos(roll/2));
    	cp = (double) (cos(pitch/2));
    	cy = (double) (cos(yaw/2));
    	sr = (double) (sin(roll/2));
    	sp = (double) (sin(pitch/2));
    	sy = (double) (sin(yaw/2));
    	cpcy = cp * cy;
    	spsy = sp * sy;
    	w = cr * cpcy + sr * spsy;
    	x = sr * cpcy - cr * spsy;
    	y = cr * sp * cy + sr * cp * sy;
    	z = cr * cp * sy - sr * sp * cy;
}

double QUAT::MagnitudeQuat()
{
  	return( (double) (sqrt(w*w+x*x+y*y+z*z)));
}

GLvoid QUAT::NormaliseQuat()
{
  	double Mag;
  	Mag = MagnitudeQuat();
  	w = w/Mag;
  	x = x/Mag;
  	y = y/Mag;
  	z = z/Mag;
}

GLvoid QUAT::MultQuat(QUAT q)
{
  	QUAT q3;
  	VECTOR vectorq1;
  	VECTOR vectorq2;
  	vectorq1.x = x;
  	vectorq1.y = y;
  	vectorq1.z = z;
  	vectorq2.x = q.x;
  	vectorq2.y = q.y;
  	vectorq2.z = q.z;

  	VECTOR tempvec1;
  	VECTOR tempvec2;
  	VECTOR tempvec3;
  	tempvec1 = vectorq1;
  	q3.w = (w*q.w) - tempvec1.DotProduct(vectorq2);
  	tempvec1.CrossVector(vectorq2);
  	tempvec2.x = w * q.x;
  	tempvec2.y = w * q.y;
  	tempvec2.z = w * q.z;
  	tempvec3.x = q.w * x;
  	tempvec3.y = q.w * y;
  	tempvec3.z = q.w * z;
  	q3.x = tempvec1.x + tempvec2.x + tempvec3.x;
  	q3.y = tempvec1.y + tempvec2.y + tempvec3.y;
  	q3.z = tempvec1.z + tempvec2.z + tempvec3.z;
  	CopyQuat(q3);
}



OBJECT::OBJECT()
{
}

OBJECT::~OBJECT()
{
}

GLvoid OBJECT::Reset()
{
	Orientation.Reset();
	Position.Reset();
        Delta_x = 0.0;
        Delta_y = 0.0;
        Delta_z = 0.0;
        Movement_x = 0.0;
        Movement_y = 0.0;
        Movement_z = 0.0;
}

GLvoid OBJECT::Rotate()
{
	QUAT temp_quat;
	temp_quat.EulerToQuat(Delta_x * radian * Multiplier, Delta_y * radian * Multiplier, Delta_z * radian * Multiplier);
	Orientation.MultQuat(temp_quat);
}

GLvoid OBJECT::Draw()
{
  // Should probably be a pure virtual 
}

GLvoid OBJECT::UpdatePosition()
{
    if (Movement_x != 0)
        MoveX();
    if (Movement_y != 0)
        MoveY();
    if (Movement_z != 0)
        MoveZ();

    Movement_x = 0.0;
    Movement_y = 0.0;
    Movement_z = 0.0;
}

GLvoid OBJECT::UpdatePosition(double x, double y, double z)
{
    if (x != 0)
        MoveX(x);
    if (y != 0)
        MoveY(y);
    if (z != 0)
        MoveZ(z);
}

GLvoid OBJECT::MoveX()
{
    double DirX;
    double DirY;
    double DirZ;
    double W;
    double X;
    double Y;
    double Z;
    QUAT TempQuat;
    QUAT TempQuat2;
    TempQuat = Orientation;
    TempQuat2.EulerToQuat(0.0,(double) (-90.0*(pi/180)), 0.0);
    TempQuat.MultQuat(TempQuat2);
    W = TempQuat.w;
    X = TempQuat.x;
    Y = TempQuat.y;
    Z = TempQuat.z;
    DirX = (double)(2.0 * ( X * Z - W * Y ));
    DirY = (double)(2.0 * ( Y * Z + W * X ));
    DirZ = (double)(1.0 - 2.0 * ( X * X + Y * Y ));
    Position.x += DirX * Movement_x * Multiplier;
    Position.y += DirY * Movement_x * Multiplier;
    Position.z += DirZ * Movement_x * Multiplier;
}

GLvoid OBJECT::MoveY()
{
    double DirX;
    double DirY;
    double DirZ;
    double W;
    double X;
    double Y;
    double Z;
    QUAT TempQuat;
    QUAT TempQuat2;
    TempQuat = Orientation;
    TempQuat2.EulerToQuat((double)(90.0*(pi/180)), 0.0, 0.0);
    TempQuat.MultQuat(TempQuat2);
    W = TempQuat.w;
    X = TempQuat.x;
    Y = TempQuat.y;
    Z = TempQuat.z;
    DirX = (double)(2.0 * ( X * Z - W * Y ));
    DirY = (double)(2.0 * ( Y * Z + W * X ));
    DirZ = (double)(1.0 - 2.0 * ( X * X + Y * Y ));
    Position.x += DirX * Movement_y * Multiplier;
    Position.y += DirY * Movement_y * Multiplier;
    Position.z += DirZ * Movement_y * Multiplier;
}

GLvoid OBJECT::MoveZ()
{
    double DirX;
    double DirY;
    double DirZ;
    double W = Orientation.w;
    double X = Orientation.x;
    double Y = Orientation.y;
    double Z = Orientation.z;
    DirX = 2.0 * ( X * Z - W * Y );
    DirY = 2.0 * ( Y * Z + W * X );
    DirZ = 1.0 - 2.0 * ( X * X + Y * Y );
    Position.x += DirX * Movement_z * Multiplier;
    Position.y += DirY * Movement_z * Multiplier;
    Position.z += DirZ * Movement_z * Multiplier;
}

GLvoid OBJECT::MoveX(double x)
{
    double DirX;
    double DirY;
    double DirZ;
    double W;
    double X;
    double Y;
    double Z;
    QUAT TempQuat;
    QUAT TempQuat2;
    TempQuat = Orientation;
    TempQuat2.EulerToQuat(0.0, -90.0*(pi/180), 0.0);
    TempQuat.MultQuat(TempQuat2);
    W = TempQuat.w;
    X = TempQuat.x;
    Y = TempQuat.y;
    Z = TempQuat.z;
    DirX = 2.0 * ( X * Z - W * Y );
    DirY = 2.0 * ( Y * Z + W * X );
    DirZ = 1.0 - 2.0 * ( X * X + Y * Y );
    Position.x += DirX * x;
    Position.y += DirY * x;
    Position.z += DirZ * x;
}

GLvoid OBJECT::MoveY(double y)
{
    double DirX;
    double DirY;
    double DirZ;
    double W;
    double X;
    double Y;
    double Z;
    QUAT TempQuat;
    QUAT TempQuat2;
    TempQuat = Orientation;
    TempQuat2.EulerToQuat(90.0*(pi/180), 0.0, 0.0);
    TempQuat.MultQuat(TempQuat2);
    W = TempQuat.w;
    X = TempQuat.x;
    Y = TempQuat.y;
    Z = TempQuat.z;
    DirX = 2.0 * ( X * Z - W * Y );
    DirY = 2.0 * ( Y * Z + W * X );
    DirZ = 1.0 - 2.0 * ( X * X + Y * Y );
    Position.x += DirX * y;
    Position.y += DirY * y;
    Position.z += DirZ * y;
}

GLvoid OBJECT::MoveZ(double z)
{
    double DirX;
    double DirY;
    double DirZ;
    double W = Orientation.w;
    double X = Orientation.x;
    double Y = Orientation.y;
    double Z = Orientation.z;
    DirX = 2.0 * ( X * Z - W * Y );
    DirY = 2.0 * ( Y * Z + W * X );
    DirZ = 1.0 - 2.0 * ( X * X + Y * Y );
    Position.x += DirX * z;
    Position.y += DirY * z;
    Position.z += DirZ * z;
}

VECTOR OBJECT::GetXUnit()
{
    double DirX;
    double DirY;
    double DirZ;
    double W;
    double X;
    double Y;
    double Z;
    QUAT TempQuat;
    QUAT TempQuat2;
    TempQuat = Orientation;
    TempQuat2.EulerToQuat(0.0, -90.0*(pi/180), 0.0);
    TempQuat.MultQuat(TempQuat2);
    W = TempQuat.w;
    X = TempQuat.x;
    Y = TempQuat.y;
    Z = TempQuat.z;
    DirX = 2.0 * ( X * Z - W * Y );
    DirY = 2.0 * ( Y * Z + W * X );
    DirZ = 1.0 - 2.0 * ( X * X + Y * Y );
    VECTOR Unit;
    Unit.x += DirX * 1;
    Unit.y += DirY * 1;
    Unit.z += DirZ * 1;
    return Unit;
}

VECTOR OBJECT::GetYUnit()
{
    double DirX;
    double DirY;
    double DirZ;
    double W;
    double X;
    double Y;
    double Z;
    QUAT TempQuat;
    QUAT TempQuat2;
    TempQuat = Orientation;
    TempQuat2.EulerToQuat(90.0*(pi/180), 0.0, 0.0);
    TempQuat.MultQuat(TempQuat2);
    W = TempQuat.w;
    X = TempQuat.x;
    Y = TempQuat.y;
    Z = TempQuat.z;
    DirX = 2.0 * ( X * Z - W * Y );
    DirY = 2.0 * ( Y * Z + W * X );
    DirZ = 1.0 - 2.0 * ( X * X + Y * Y );
    VECTOR Unit;
    Unit.x += DirX * 1;
    Unit.y += DirY * 1;
    Unit.z += DirZ * 1;
    return Unit;
}

VECTOR OBJECT::GetZUnit()
{
    double DirX;
    double DirY;
    double DirZ;
    double W = Orientation.w;
    double X = Orientation.x;
    double Y = Orientation.y;
    double Z = Orientation.z;
    DirX = 2.0 * ( X * Z - W * Y );
    DirY = 2.0 * ( Y * Z + W * X );
    DirZ = 1.0 - 2.0 * ( X * X + Y * Y );
    VECTOR Unit;
    Unit.x += DirX * 1;
    Unit.y += DirY * 1;
    Unit.z += DirZ * 1;
    return Unit;
}



CAMERA::CAMERA()
{
}

CAMERA::~CAMERA()
{
}

GLvoid CAMERA::Reset()
{
	Orientation.Reset();
	Position.Reset();
        Delta_x = 0.0;
        Delta_y = 0.0;
        Delta_z = 0.0;
        Matrix.LoadIdentity();
}

GLvoid CAMERA::Update()
{
    Rotate();

    UpdatePosition();
}

GLvoid CAMERA::Apply()
{
    Matrix.QuatToMatrix(Orientation);
    Matrix.MatrixInverse();

    glLoadMatrixd(Matrix.Element);
    glTranslated(-Position.x,-Position.y,-Position.z);
}



GLvoid LoadIdentity(double m[])
{
    	m[0]=1.0f;
    	m[1]=0.0f;
    	m[2]=0.0f;
    	m[3]=0.0f;

    	m[4]=0.0f;
    	m[5]=1.0f;
    	m[6]=0.0f;
    	m[7]=0.0f;

    	m[8]=0.0f;
    	m[9]=0.0f;
    	m[10]=1.0f;
    	m[11]=0.0f;

    	m[12]=0.0f;
    	m[13]=0.0f;
    	m[14]=0.0f;
    	m[15]=1.0f;
}

GLvoid CopyMatrix(double m[], double n[])
{
    	m[0 ] = n[0 ];
    	m[1 ] = n[1 ];
    	m[2 ] = n[2 ];
    	m[3 ] = n[3 ];
    	m[4 ] = n[4 ];
    	m[5 ] = n[5 ];
    	m[6 ] = n[6 ];
    	m[7 ] = n[7 ];
    	m[8 ] = n[8 ];
    	m[9 ] = n[9 ];
    	m[10] = n[10];
    	m[11] = n[11];
    	m[12] = n[12];
    	m[13] = n[13];
    	m[14] = n[14];
    	m[15] = n[15];
}

GLvoid MultMatrix(double m[], double n[])
{
    	double temp[16];

    	CopyMatrix(temp, m);
  	m[0] = temp[0 ]*n[0 ]
       		+ temp[4 ]*n[1 ]
       		+ temp[8 ]*n[2 ]
       		+ temp[12]*n[3 ];

  	m[1] = temp[1 ]*n[0 ]
       		+ temp[5 ]*n[1 ]
       		+ temp[9 ]*n[2 ]
       		+ temp[13]*n[3 ];

  	m[2] = temp[2 ]*n[0 ]
       		+ temp[6 ]*n[1 ]
       		+ temp[10]*n[2 ]
       		+ temp[14]*n[3 ];

  	m[3] = temp[3 ]*n[0 ]
       		+ temp[7 ]*n[1 ]
       		+ temp[11]*n[2 ]
       		+ temp[15]*n[3 ];

  	m[4] = temp[0 ]*n[4 ]
       		+ temp[4 ]*n[5 ]
       		+ temp[8 ]*n[6 ]
       		+ temp[12]*n[7 ];

  	m[5] = temp[1 ]*n[4 ]
       		+ temp[5 ]*n[5 ]
       		+ temp[9 ]*n[6 ]
       		+ temp[13]*n[7 ];

  	m[6] = temp[2 ]*n[4 ]
       		+ temp[6 ]*n[5 ]
       		+ temp[10]*n[6 ]
       		+ temp[14]*n[7 ];

  	m[7] = temp[3 ]*n[4 ]
       		+ temp[7 ]*n[5 ]
       		+ temp[11]*n[6 ]
       		+ temp[15]*n[7 ];

  	m[8] = temp[0 ]*n[8 ]
       		+ temp[4 ]*n[9 ]
       		+ temp[8 ]*n[10]
       		+ temp[12]*n[11];

  	m[9] = temp[1 ]*n[8 ]
       		+ temp[5 ]*n[9 ]
       		+ temp[9 ]*n[10]
       		+ temp[13]*n[11];

  	m[10]= temp[2 ]*n[8 ]
       		+ temp[6 ]*n[9 ]
       		+ temp[10]*n[10]
       		+ temp[14]*n[11];

  	m[11]= temp[3 ]*n[8 ]
       		+ temp[7 ]*n[9 ]
       		+ temp[11]*n[10]
       		+ temp[15]*n[11];

  	m[12]= temp[0 ]*n[12]
       		+ temp[4 ]*n[13]
       		+ temp[8 ]*n[14]
       		+ temp[12]*n[15];

  	m[13]= temp[1 ]*n[12]
       		+ temp[5 ]*n[13]
       		+ temp[9 ]*n[14]
       		+ temp[13]*n[15];

  	m[14]= temp[2 ]*n[12]
       		+ temp[6 ]*n[13]
       		+ temp[10]*n[14]
       		+ temp[14]*n[15];

  	m[15]= temp[3 ]*n[12]
       		+ temp[7 ]*n[13]
       		+ temp[11]*n[14]
       		+ temp[15]*n[15];
}

GLvoid MatrixInverse(double m[])
{
	double n[16];

  	CopyMatrix(n, m);
  	m[0 ] = n[0 ];
  	m[1 ] = n[4 ];
  	m[2 ] = n[8 ];

  	m[4 ] = n[1 ];
  	m[5 ] = n[5 ];
  	m[6 ] = n[9 ];

  	m[8 ] = n[2 ];
  	m[9 ] = n[6 ];
  	m[10] = n[10];

  	m[12] *= -1.0f;
  	m[13] *= -1.0f;
  	m[14] *= -1.0f;
}

    /* The following routine converts an angle and a unit axis vector
        * to a matrix, returning the corresponding unit quaternion at no
        * extra cost. It is written in such a way as to allow both fixed
        * point and doubleing point versions to be created by appropriate
        * definitions of FPOINT, ANGLE, VECTOR, QUAT, MATRIX, MUL, HALF,
        * TWICE, COS, SIN, ONE, and ZERO.
        * The following is an example of doubleing point definitions.*/
QUAT AxisAngleToMatrix(VECTOR axis, double theta, double m[16])
{
        QUAT q;
        double halfTheta = theta * 0.5;
        double cosHalfTheta = cos(halfTheta);
        double sinHalfTheta = sin(halfTheta);
        double xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
        q.x = axis.x * sinHalfTheta;
        q.y = axis.y * sinHalfTheta;
        q.z = axis.z * sinHalfTheta;
        q.w = cosHalfTheta;
        xs = q.x * 2;  ys = q.y * 2;  zs = q.z * 2;
        wx = q.w * xs; wy = q.w * ys; wz = q.w * zs;
        xx = q.x * xs; xy = q.x * ys; xz = q.x * zs;
        yy = q.y * ys; yz = q.y * zs; zz = q.z * zs;
        m[0] = 1 - (yy + zz);
        m[1] = xy - wz;
        m[2] = xz + wy;
        m[4] = xy + wz;
        m[5] = 1 - (xx + zz);
        m[6] = yz - wx;
        m[8] = xz - wy;
        m[9] = yz + wx;
        m[10] = 1 - (xx + yy);
        /* Fill in remainder of 4x4 homogeneous transform matrix. */
        m[12] = m[13] = m[14] = m[3] = m[7] = m[11] = 0;
        m[15] = 1;
        return (q);
}

double DotProduct(VECTOR vec1, VECTOR vec2)
{
    /*
    Dot Product of two Vectors.

    U = (Ux, Uy, Uz)
    V = (Vx, Vy, Vz)
    U*V = UxVx + UyVy + UzVz
    U*V = |U||V|cos(t) (where t is the angle theta between the two vectors)
    */
  	double dot;
  	dot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
  	return dot;
}

VECTOR CrossVector(VECTOR vec1, VECTOR vec2)
{
    /*
    Cross Product of Two Vectors.

    a �b = ( a.y * b.z - a.z * b.y,

    a.z * b.x - a.x * b.z,

    a.x * b.y - a.y * b.x )

    | a �b | = |a| * |b| * sin()
    */
  	VECTOR vec3;
  	vec3.x = vec1.y * vec2.z - vec1.z * vec2.y;
  	vec3.y = vec1.z * vec2.x - vec1.x * vec2.z;
  	vec3.z = vec1.x * vec2.y - vec1.y * vec2.x;
  	return vec3;
}

void EulerToQuat(double roll, double pitch, double yaw, QUAT * quat)
{
    /*
    Euler Angle to Quarternion.

    q = qyaw qpitch qroll where:

    qyaw = [cos(f /2), (0, 0, sin(f /2)]
    qpitch = [cos (q/2), (0, sin(q/2), 0)]
    qroll = [cos (y/2), (sin(y/2), 0, 0)]
    */
	double cr, cp, cy, sr, sp, sy, cpcy, spsy;  // calculate trig identities
    	cr = cos(roll/2);
	cp = cos(pitch/2);
    	cy = cos(yaw/2);
    	sr = sin(roll/2);
	sp = sin(pitch/2);
    	sy = sin(yaw/2);
    	cpcy = cp * cy;
    	spsy = sp * sy;
	quat->w = cr * cpcy + sr * spsy;
    	quat->x = sr * cpcy - cr * spsy;
	quat->y = cr * sp * cy + sr * cp * sy;
    	quat->z = cr * cp * sy - sr * sp * cy;
}

double MagnitudeQuat(QUAT q1)
{
  	return( sqrt(q1.w*q1.w+q1.x*q1.x+q1.y*q1.y+q1.z*q1.z));
}

QUAT NormaliseQuat(QUAT q1)
{
  	QUAT q2;
  	double Mag;
  	Mag = MagnitudeQuat(q1);
  	q2.w = q1.w/Mag;
  	q2.x = q1.x/Mag;
  	q2.y = q1.y/Mag;
  	q2.z = q1.z/Mag;
  	return q2;
}

GLvoid QuatToMatrix(QUAT quat, double m[16])
{
  	double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
  	// calculate coefficients
  	x2 = quat.x + quat.x;
  	y2 = quat.y + quat.y;
  	z2 = quat.z + quat.z;
  	xx = quat.x * x2;
  	xy = quat.x * y2;
  	xz = quat.x * z2;
  	yy = quat.y * y2;
  	yz = quat.y * z2;
  	zz = quat.z * z2;
  	wx = quat.w * x2;
  	wy = quat.w * y2;
  	wz = quat.w * z2;
  	m[0] = 1.0 - (yy + zz);
  	m[1] = xy - wz;
  	m[2] = xz + wy;
  	m[3] = 0.0;
  	m[4] = xy + wz;
  	m[5] = 1.0 - (xx + zz);
  	m[6] = yz - wx;
  	m[7] = 0.0;
  	m[8] = xz - wy;
  	m[9] = yz + wx;
  	m[10] = 1.0 - (xx + yy);
  	m[11] = 0.0;
  	m[12] = 0;
  	m[13] = 0;
  	m[14] = 0;
  	m[15] = 1;
}

QUAT MultQuat(QUAT q1, QUAT q2)
{
    /*
    Multiplication of two Quarternions.

    qq = [ww - v  v, v x v + wv +wv]
    (  is vector dot product and x is vector cross product )
    */
  	QUAT q3;
  	VECTOR vectorq1;
  	VECTOR vectorq2;
  	vectorq1.x = q1.x;
  	vectorq1.y = q1.y;
  	vectorq1.z = q1.z;
  	vectorq2.x = q2.x;
  	vectorq2.y = q2.y;
  	vectorq2.z = q2.z;

  	VECTOR tempvec1;
  	VECTOR tempvec2;
  	VECTOR tempvec3;
  	q3.w = (q1.w*q2.w) - DotProduct(vectorq1, vectorq2);
  	tempvec1 = CrossVector(vectorq1, vectorq2);
  	tempvec2.x = q1.w * q2.x;
  	tempvec2.y = q1.w * q2.y;
  	tempvec2.z = q1.w * q2.z;
  	tempvec3.x = q2.w * q1.x;
  	tempvec3.y = q2.w * q1.y;
  	tempvec3.z = q2.w * q1.z;
  	q3.x = tempvec1.x + tempvec2.x + tempvec3.x;
  	q3.y = tempvec1.y + tempvec2.y + tempvec3.y;
  	q3.z = tempvec1.z + tempvec2.z + tempvec3.z;
  	return NormaliseQuat(q3);
}

// returns the normal vector to a plane defined by three vertices
VECTOR GetNormal(VECTOR vertex1, VECTOR vertex2, VECTOR vertex3)
{
    	double ux, uy, uz, vx, vy, vz;
      	VECTOR temp_vertex;

      	ux = vertex1.x - vertex2.x;
      	uy = vertex1.y - vertex2.y;
      	uz = vertex1.z - vertex2.z;
      	vx = vertex3.x - vertex2.x;
      	vy = vertex3.y - vertex2.y;
      	vz = vertex3.z - vertex2.z;
      	temp_vertex.x = (uy*vz)-(vy*uz);
      	temp_vertex.y = (uz*vx)-(vz*ux);
      	temp_vertex.z = (ux*vy)-(vx*uy);
      	return temp_vertex;
}

VERTEX GetNorm(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3)
{
    	double ux;
    	double uy;
    	double uz;
    	double vx;
    	double vy;
    	double vz;
      	VERTEX temp_vertex;
      	ux = x1 - x2;
      	uy = y1 - y2;
      	uz = z1 - z2;
      	vx = x3 - x2;
      	vy = y3 - y2;
      	vz = z3 - z2;
      	temp_vertex.normal.x = (uy*vz)-(vy*uz);
      	temp_vertex.normal.y = (uz*vx)-(vz*ux);
      	temp_vertex.normal.z = (ux*vy)-(vx*uy);
      	return temp_vertex;
}

double MagnitudeVector(VECTOR vec1)
{
  return(sqrt(vec1.x*vec1.x+vec1.y*vec1.y+vec1.z*vec1.z));
}

VECTOR GetUnitVector(VECTOR vector)
{
	// Reduces a normal vector specified as a set of three coordinates,
	// to a unit normal vector of length one.

	// Calculate the length of the vector		
	double length = (double) sqrt(( vector.x * vector.x) + 
						        ( vector.y * vector.y) +
						        ( vector.z * vector.z) );

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector.x /= length;
	vector.y /= length;
	vector.z /= length;
        return vector;
}

VECTOR GetEdgeVector(VECTOR point1, VECTOR point2)
{
  VECTOR temp_vector;
  temp_vector.x = point1.x - point2.x;
  temp_vector.y = point1.y - point2.y;
  temp_vector.z = point1.z - point2.z;
  return temp_vector;
}



bool CheckPointInTriangle(VECTOR point, VECTOR a, VECTOR b, VECTOR c) 
{
  
  double total_angles = 0.0f;
       
  // make the 3 vectors
  VECTOR TempVect;
  TempVect.x = point.x - a.x;
  TempVect.y = point.y - a.y;
  TempVect.z = point.z - a.z;
  VECTOR v1 = TempVect;
  TempVect.x = point.x - b.x;
  TempVect.y = point.y - b.y;
  TempVect.z = point.z - b.z;
  VECTOR v2 = TempVect;
  TempVect.x = point.x - c.x;
  TempVect.y = point.y - c.y;
  TempVect.z = point.z - c.z;
  VECTOR v3 = TempVect;
  
  v1 = GetUnitVector(v1);
  v2 = GetUnitVector(v2);
  v3 = GetUnitVector(v3);
  double Dot1 = DotProduct(v1,v2);
  if (Dot1 < -1)
    Dot1 = -1;
  if (Dot1 > 1)
    Dot1 = 1;
  total_angles += acos(Dot1);   
  double Dot2 = DotProduct(v2,v3);
  if (Dot2 < -1)
    Dot2 = -1;
  if (Dot2 > 1)
    Dot2 = 1;
  total_angles += acos(Dot2);
  double Dot3 = DotProduct(v3,v1);
  if (Dot3 < -1)
    Dot3 = -1;
  if (Dot3 > 1)
    Dot3 = 1;
  total_angles += acos(Dot3); 
     
  if (fabs(total_angles-2*pi) <= 0.005)
   return (true);
     
  return(false);
}

// Check if the point is in the quad
// All the vertices of the quad MUST be in the same plane
bool CheckPointInQuad(VECTOR* point, QUAD* quad)
{
	if (CheckPointInTriangle(*point,quad->Vertex[0].coords,quad->Vertex[1].coords,quad->Vertex[2].coords))
		return true;

	if (CheckPointInTriangle(*point,quad->Vertex[0].coords,quad->Vertex[3].coords,quad->Vertex[2].coords))
		return true;

	return false;
}



VECTOR line_plane_collision(VECTOR *a, VECTOR *b, QUAD *plane)
{
    double Distance = -(plane->Vertex[0].coords.x * plane->Vertex[0].normal.x + plane->Vertex[0].coords.y * plane->Vertex[0].normal.y + plane->Vertex[0].coords.z * plane->Vertex[0].normal.z);
    double final_x,final_y,final_z,final_t;
    double t,i;
    VECTOR temp;

    t=0; i=0;
    i+=(plane->Vertex[0].normal.x*b->x)+(plane->Vertex[0].normal.y*b->y)+(plane->Vertex[0].normal.z*b->z)+(Distance);
    t+=(plane->Vertex[0].normal.x*(b->x*-1))+(plane->Vertex[0].normal.y*(b->y*-1))+(plane->Vertex[0].normal.z*(b->z*-1));
    t+=(plane->Vertex[0].normal.x*a->x)+(plane->Vertex[0].normal.y*a->y)+(plane->Vertex[0].normal.z*a->z);

    // Be wary of possible divide-by-zeros here (i.e. if t==0)
    if (t)
		final_t = (-i)/t;
    else
		final_t = 0;
    // Vertical Line Segment
    if ((a->x == b->x)&&(a->z == b->z))
    { // vertical line segment

        temp.x = a->x;
        temp.y = (-((plane->Vertex[0].normal.x*a->x)+(plane->Vertex[0].normal.z*a->z)+(Distance)))/(plane->Vertex[0].normal.y);
        temp.z = a->z;

        return(temp);
    }

    final_x = (((a->x)*(final_t))+((b->x)*(1-final_t)));
    final_y = (((a->y)*(final_t))+((b->y)*(1-final_t)));
    final_z = (((a->z)*(final_t))+((b->z)*(1-final_t)));

    temp.x = final_x;
    temp.y = final_y;
    temp.z = final_z;

    return(temp);
}





bool CheckClipPlanes(CAMERA Camera, VECTOR Vect)
{
  double ProjectionMatrix[16];
  double ModelViewMatrix[16];
  double A, B, C, Distance;
  int Counter = 0;
  glGetDoublev(GL_PROJECTION_MATRIX, ProjectionMatrix);
  glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMatrix);

  MultMatrix(ProjectionMatrix, ModelViewMatrix);

  //right clipping plane
  A = ProjectionMatrix[0] - ProjectionMatrix[3];
  B = ProjectionMatrix[4] - ProjectionMatrix[7];
  C = ProjectionMatrix[8] - ProjectionMatrix[11];

  Distance = -1 * (A * (-Camera.Position.x + Vect.x) + B * (-Camera.Position.y + Vect.y) + C * (-Camera.Position.z + Vect.z));
  if (Distance > 0)
    Counter++;

  //left clipping plane
  A = ProjectionMatrix[0] + ProjectionMatrix[3];
  B = ProjectionMatrix[4] + ProjectionMatrix[7];
  C = ProjectionMatrix[8] + ProjectionMatrix[11];

  Distance = A * (-Camera.Position.x + Vect.x) + B * (-Camera.Position.y + Vect.y) + C * (-Camera.Position.z + Vect.z);
  if (Distance > 0)
    Counter++;

  //top clipping plane
  A = ProjectionMatrix[1] - ProjectionMatrix[3];
  B = ProjectionMatrix[5] - ProjectionMatrix[7];
  C = ProjectionMatrix[9] - ProjectionMatrix[11];

  Distance = -1 * (A * (-Camera.Position.x + Vect.x) + B * (-Camera.Position.y + Vect.y) + C * (-Camera.Position.z + Vect.z));
  if (Distance > 0)
    Counter++;

  //bottom clipping plane
  A = ProjectionMatrix[1] + ProjectionMatrix[3];
  B = ProjectionMatrix[5] + ProjectionMatrix[7];
  C = ProjectionMatrix[9] + ProjectionMatrix[11];

  Distance = A * (-Camera.Position.x + Vect.x) + B * (-Camera.Position.y + Vect.y) + C * (-Camera.Position.z + Vect.z);
  if (Distance > 0)
    Counter++;

  // near clipping plane (might not be necessary when the near frustrum plane is close, but just in case)
  A = ProjectionMatrix[2] - ProjectionMatrix[3];
  B = ProjectionMatrix[6] - ProjectionMatrix[7];
  C = ProjectionMatrix[10] - ProjectionMatrix[11];

  Distance = A * (-Camera.Position.x + Vect.x) + B * (-Camera.Position.y + Vect.y) + C * (-Camera.Position.z + Vect.z);
  if (Distance > 0)
    Counter++;

  // far clipping plane (the equation didn't work for the far plane, so I'll just use a distance test)
  VECTOR Vect2;
  Vect2.x = Vect.x - Camera.Position.x;
  Vect2.y = Vect.y - Camera.Position.y;
  Vect2.z = Vect.z - Camera.Position.z;
  if (MagnitudeVector(Vect2) < 200)
    Counter++;

  if (Counter == 6)
    return 1;
  else
    return 0;
}





// SelectPolygon() creates a ray from the current camera coordinates to a point half a unit into the screen that is coincident
// with the mouse coordinates passed in and checks for an intersection along the line of this ray with the quad passed in.
// The 4 vertices of the quad MUST be in the same plane
// If there is an intersection then the function returns true.
// MouseX and MouseY are relative to the top-left corner of the window.
bool SelectPolygon(double cameraX, double cameraY, double cameraZ, 
				   double Vertices[12], double Intersec[3], 
				   int MouseX, int MouseY)
{
	VECTOR CollisionPoint;
	VECTOR WorldPos;
	VECTOR posCam=VECTOR(cameraX,cameraY,cameraZ);
	VECTOR* posCamera=&posCam;
	QUAD aquad;
	
	aquad.Vertex[0].coords.x=Vertices[0];
	aquad.Vertex[0].coords.y=Vertices[1];
	aquad.Vertex[0].coords.z=Vertices[2];
	
	aquad.Vertex[1].coords.x=Vertices[3];
	aquad.Vertex[1].coords.y=Vertices[4];
	aquad.Vertex[1].coords.z=Vertices[5];
	
	aquad.Vertex[2].coords.x=Vertices[6];
	aquad.Vertex[2].coords.y=Vertices[7];
	aquad.Vertex[2].coords.z=Vertices[8];
	
	aquad.Vertex[3].coords.x=Vertices[9];
	aquad.Vertex[3].coords.y=Vertices[10];
	aquad.Vertex[3].coords.z=Vertices[11];
	
	aquad.numVertices=4;
	aquad.SetNormal();
	QUAD* quad=&aquad;
			
	// Get the modelview and projection matrices 
	double WorldPosX, WorldPosY, WorldPosZ, MousePosX, MousePosY, MousePosZ;
	double ModelMatrix[16]; 
	glGetDoublev(GL_MODELVIEW_MATRIX, ModelMatrix);
	double ProjMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);

	// Get the current viewport
	int Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);

	if (MouseX >= Viewport[0] && MouseX <= Viewport[2] && MouseY >= Viewport[1] && MouseY <= Viewport[3])
	{
		// Set the end point of ray in windows coordinates
		MousePosX = MouseX;
		MousePosY = Viewport[3] - MouseY; // invert mouse Y coordinate
		MousePosZ = 1.0; // near clip plane depth

		// Get unprojected end point
		gluUnProject
		(
			MousePosX, 
			MousePosY, 
			MousePosZ, 
			ModelMatrix, 
			ProjMatrix,
			Viewport,
		    &WorldPosX,
		    &WorldPosY,
		    &WorldPosZ
		);
		WorldPos.x = WorldPosX;
		WorldPos.y = WorldPosY;
		WorldPos.z = WorldPosZ;

        CollisionPoint = line_plane_collision(posCamera, &WorldPos, quad);
		Intersec[0]=CollisionPoint.x;
		Intersec[1]=CollisionPoint.y;	
		Intersec[2]=CollisionPoint.z;
		
        if (CheckPointInQuad(&CollisionPoint, quad))
			return true;
		
	}


	return false;
}


















