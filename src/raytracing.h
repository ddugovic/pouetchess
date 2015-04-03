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

#ifndef __RAY_TRACING__H__
#define __RAY_TRACING__H__


// SelectPolygon() creates a ray from the current camera coordinates to a point half a unit into the screen that is coincident
// with the mouse coordinates passed in and checks for an intersection along the line of this ray with the quad passed in.
// The 4 vertices of the quad MUST be in the same plane
// If there is an intersection then the function returns true.
// MouseX and MouseY are relative to the top-left corner of the window.
bool SelectPolygon(double cameraX, double cameraY, double cameraZ, 
				   double Vertices[12], double Intersec[3], 
				   int MouseX, int MouseY);


#endif


