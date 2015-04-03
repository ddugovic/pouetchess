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

#ifndef CVECTOR_H
#define CVECTOR_H


#include <math.h>                                         // Math header file.


class CTexCoord
{
   public:
      CTexCoord()
         {
            tu = 0.0f; tv = 0.0f;
         }

      CTexCoord(float u, float v)
         {
            tu = u; tv = v;
         }

      float tu, tv;                                      // Set of texture coordinates.
};


class CVector4
{
   public:
      CVector4();                                        // Constructor.
      CVector4(float X, float Y, float Z);               // Overloaded constructor.
      CVector4(float X, float Y, float Z, float W);      // Overloaded constructor again.
      CVector4(const CVector4 &v);                       // Another overloaded constructor.

      void operator=(CVector4 v);                        // Overloaded = sign.
      CVector4 operator-(CVector4 v);                    // Overloaded - sign.
      CVector4 operator+(CVector4 v);                    // Overloaded + sign.
      CVector4 operator*(CVector4 v);                    // Overloaded * sign.
      CVector4 operator/(CVector4 v);                    // Overloaded / sign.
      CVector4 operator+(float f);                       // Overloaded + sign.
      CVector4 operator-(float f);                       // Overloaded - sign.
      CVector4 operator*(float f);                       // Overloaded * sign.
      CVector4 operator/(float f);                       // Overloaded / sign.

      void operator +=(CVector4 v);                      // Overloaded += sign.
      void operator -=(CVector4 v);                      // Overloaded -= sign.
      void operator *=(CVector4 v);                      // Overloaded *= sign.
      void operator /=(CVector4 v);                      // Overloaded /= sign.
      void operator +=(float f);                         // Overloaded += sign.
      void operator -=(float f);                         // Overloaded -= sign.
      void operator *=(float f);                         // Overloaded *= sign.
      void operator /=(float f);                         // Overloaded /= sign.

      bool operator ==(CVector4 v);                      // Overloaded == sign.
      bool operator !=(CVector4 v);                      // Overloaded != sign.

      void CrossProduct(CVector4 v1, CVector4 v2);       // Stores cross product of v1, v2.
      void CrossProduct3(CVector4 v1, CVector4 v2,
                         CVector4 v3);                   // Stores cross product of v1, v2, v3.
      float DotProduct3(CVector4 v1);                    // Dot 3 on v1 and this.
      float DotProduct4(CVector4 v1);                    // Returns dot product 4 of v1 and this.
      float GetLength();                                 // Return this objects length.
      void Normal();                                     // Normalize this vector.
      void Normalize(CVector4 Triangle[]);               // Find the normal of a triangle.
      void CalculateTangentVector(CVector4 Point1,
                                  CVector4 Point2,
                                  CVector4 Point3,
                                  CVector4 NormalOfA,
                                  CTexCoord P1Tex,
                                  CTexCoord P2Tex,
                                  CTexCoord P3Tex);      // Cacluate a tagent vector & store here.
      void ExtendVertexPos(CVector4 lightPos,            // Extend point based on light position
                           float Extend);                // and currend point, saves here.
      void ExtendVertexPos(CVector4 currentVertex,       // Takes current point and source and
                           CVector4 lightPos,            // extend and saves here.
                           float Extend);
      CVector4 GetRotatedX(double angle);                // Rotate along x.
      CVector4 GetRotatedY(double angle);                // Rotate alone y.
      CVector4 GetRotatedZ(double angle);                // Rotate along z.
      CVector4 GetRotatedAxis(double angle, CVector4 axis);// Rotate along an axis.
      void CalculateBinormalVector(CVector4 tangent,
                                   CVector4 normal);     // Calcluate a binormal.
      void ClampTo01();                                  // Clamp this vector between 0 and 1.


      float x, y, z, w;                                  // vertex's x, y, and z info.
};

#endif

