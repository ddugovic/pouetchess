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

#ifndef CLIGHT_WAVE_OBJECT_H
#define CLIGHT_WAVE_OBJECT_H

#include <SDL/SDL.h>
#include "CVector.h"


struct stPolygon
{
	int *pointIndex;                                            // List of point indexes.
	int totalPoints;                                            // Total points of this poly.
	int surfaceIndex;                                           // Index into surface array.
    CVector4 normal;                                            // This poly's normal.
};


struct stSurface
{
	float red, green, blue;                                     // Color of surface.
	char name[40];                                              // Name of surface.
};


class CLightWaveObject
{
	public:
      CLightWaveObject();                                      // Contructor.
      ~CLightWaveObject();                                     // Destructor.

      bool LoadModel(char *filename);                          // Load a model into this class.
      void Shutdown();                                         // Release all resources.

   private:
      bool ReadShortFromFile(FILE *fp, unsigned short &value); // Read a short from a file.
      bool ReadIntFromFile(FILE *fp, int &value);              // Read a int from a file.
      bool ReadFloatFromFile(FILE *fp, float &value);          // Read a float from a file.
      bool ReadSubChunkFromFile(FILE *fp, char *buffer,
                                unsigned short &val);          // Read a sub chunk from file.

      bool LoadTags(FILE *fp);                                 // Load all tags.
	   bool LoadSurfaces(FILE *fp);                             // Load all surfaces.
      int AddPolygon(FILE * fp);                               // Add a polygon to the list.
	   bool FindChunk(FILE *fp, const char *chunk);             // Searches for a chunk.
	   bool FindNextChunk(FILE *fp, const char *chunk);         // Searches for a chunk.

      void CalculateNormals();                                 // Calculate all polygon normals.

   public:
	   CVector4 *m_vertices;                                    // List of vertex points.
	   CVector4 *m_normals;										// List of normals
	   stPolygon *m_polygons;                                   // List of polygons.
	   stSurface *m_surfaces;                                   // List of surfaces.

      int m_totalPoints;                                       // Total num of vertex points.
      int m_totalPolygons;                                     // Total num of polygons.
      int m_totalSurfaces;                                     // Total num of surfaces.
};

#endif


