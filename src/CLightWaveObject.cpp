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
 

#include "CLightWaveObject.h"
#include <string.h>
#include <math.h>


CLightWaveObject::CLightWaveObject()
{
	// Initialize all variables...
	m_vertices = 0;
	m_normals  = 0;
	m_polygons = 0;
	m_surfaces = 0;
	m_totalPoints = 0;
	m_totalPolygons = 0;
	m_totalSurfaces = 0;
}


CLightWaveObject::~CLightWaveObject()
{
   // Clear out everything this class used.
   Shutdown();
}


bool CLightWaveObject::LoadModel(char *filename)
{
	FILE *fp;
	int i;

	// Check to see if something has been passed in.
	if(filename == 0) return false;
	
	// Open the file with our model.
	fp = fopen(filename, "rb");
	
	// Error checking.
	if(!fp) return false;
	
	// Make sure all the variables have been cleared out.
	Shutdown();


	// Load all the points of our lightwave model into this object.
	// points is a pointer into our empty list of vertex points.  We will fill it in below.
	CVector4 *points;
	
	// length will hold the total amount of bytes there are in vertex points.
	int length = 0;
	
	// Move to the points section by looking for the PNTS chunk.
	if(!FindChunk(fp, "PNTS"))
      return false;
	
	// Read in the length of the chunk.  In other words we are reading the number of bytes
	// of all the points.
	if(!ReadIntFromFile(fp, length))
		return false;

	// If there was an error and we have no points then we return false.
	if(length <= 0)
	{
		   Shutdown();
		   fclose(fp);
		   return false;
	}
	
	// Calculate the number of points by the length in bytes / 12 (4 for each axis).
	m_totalPoints = length / 12;


	// Allocate the member variable m_vertices and normals.
	m_vertices = new CVector4[m_totalPoints];
	m_normals = new CVector4[m_totalPoints];

	// Error checking.
	if(!m_vertices) return false;
	if(!m_normals) return false;
	
	// Set our pointer to the newly allocated array.
	points = m_vertices;

	// Loop through and read in the point information for each axis of each point.
	for(i = 0; i < m_totalPoints; i++)
    {
         // Read in the x, y, z for this point.  If any errors occur then we return 0.
		 if(!ReadFloatFromFile(fp, points->x)) return false;
         if(!ReadFloatFromFile(fp, points->y)) return false;
		 if(!ReadFloatFromFile(fp, points->z)) return false;

		// We swich the signs of the z so we render the model correctly in OpenGL.
		points->z = -points->z;
		
		// Move to the next point.
		points++;
	}

	// Next we load all the polygon data into the lightwave object.  These are just
	// indexes into the points list.
	int result = 0;
	length = 0;
	char buffer[4];
	
	// Move to the section where the polygons are stored.
	if(!FindChunk(fp, "POLS")) return false;
	
	// Read in the length of the polygon list.
	if(!ReadIntFromFile(fp, length)) return false;
	
	// Check if 2 is evenly divided into length.
	if(length % 2) length++;
	
	m_totalPolygons = 0;
	
	// Read in the next tag.
	fread(buffer, 1, 4, fp);
	
	// It should be a face, if not we have a problem.
	if(strncmp(buffer, "FACE", 4) != 0) return false;
	
	// Loop through and load all the polygons in the model.
	for(i = 4; i < length;)
	{
		// Read in a polygon.
		result = AddPolygon(fp);
		
		// Error checking.
		if(result == 0) return false;
		
		// Next lets add to count so we can keep loading until we hit the length.
		i += result;
	}

	// Read in the polygon surface indices.
	if(!LoadTags(fp)) return false;
	
	// Again if we don't have any polygons loaded in then we have an error.
	if(m_totalPolygons == 0)
	{
		Shutdown();
		fclose(fp);
		return false;
	}

	// Next we load all the surface data of the model.  If none is provided we supply default
	// values for it.
	if(!LoadSurfaces(fp))
	{
		m_surfaces = new stSurface;
		m_totalSurfaces = 1;
		m_surfaces->red = 155;
		m_surfaces->green = 155; 
		m_surfaces->blue = 175;
		strcpy(m_surfaces->name, "Default");
	}

	// Calculate the normals for each polygon in the model.
	CalculateNormals();

	// Close the file because we are done loading.
	fclose(fp);

	return true;
}


bool CLightWaveObject::ReadShortFromFile(FILE *fp, unsigned short &value)
{
	// This function will read a unsigned short value out of the file.
	// The byte is in mortorola order so we must swap the values after
	// reading it from a file.
	char buffer[2];
	char *pValue = (char*)&value;
	
	if(!fread(&buffer, 2, 1, fp))
		return false;
	
	pValue[0] = buffer[1];
	pValue[1] = buffer[0];
	
	return true;
}


bool CLightWaveObject::ReadIntFromFile(FILE *fp, int &value)
{
	// This function will read a int value out of the file.
	// The byte is in mortorola order so we must swap the values after
	// reading it from a file.
	char buffer[4];
	char *pValue = (char*)&value;
	
	if(!fread(&buffer, 4, 1, fp))
		return false;
	
	pValue[0] = buffer[3];
	pValue[1] = buffer[2];
	pValue[2] = buffer[1];
	pValue[3] = buffer[0];
	
	return true;
}


bool CLightWaveObject::ReadFloatFromFile(FILE *fp, float &value)
{
	// This function will read a float value out of the file.
	// The byte is in mortorola order so we must swap the values after
	// reading it from a file.
	char buffer[4];
	char *pValue = (char*)&value;
	
	if(!fread(&buffer, 4, 1, fp))
		return false;
	
	pValue[0] = buffer[3];
	pValue[1] = buffer[2];
	pValue[2] = buffer[1];
	pValue[3] = buffer[0];
	
	return true;
}


bool CLightWaveObject::ReadSubChunkFromFile(FILE *fp, char *buffer, unsigned short &val)
{
	// Read in the header of the sub chunk.
	if(!fread(buffer,4,1,fp)) return false;
	
	// Read in the length of the sub chunk.
	if(!ReadShortFromFile(fp,val)) return false;
	
	return true;
}


bool CLightWaveObject::LoadTags(FILE *fp)
{
	// Read in all tags in the file.
	int count;
	unsigned short polygonID;
	unsigned short surfaceIndex;
	char buffer[4];
	int length;
	
	// Look for the PTAG tag...
	if(!FindNextChunk(fp, "PTAG")) return false;
	
	// ...then read in its length.
	if(!ReadIntFromFile(fp, length)) return false;
	
	// Read in the next tag.
	fread(buffer, 4, 1, fp);
	
	// Check to see if this is a surface tag.
	if(strncmp(buffer, "SURF", 4) != 0)
	{
		fseek(fp, length, SEEK_CUR);
		return true;
	}
	
	count = 4;

	// Read in all the surface indexes.
	while(count < length)
	{
		if(!ReadShortFromFile(fp, polygonID)) return false;
		if(!ReadShortFromFile(fp, surfaceIndex)) return false;

		if(polygonID < m_totalPolygons)
			m_polygons[polygonID].surfaceIndex = surfaceIndex;

		count += 4;
	}

	return true;
}


bool CLightWaveObject::LoadSurfaces(FILE* fp)
{
	char buffer[4];
	int i = 0, length = 0, count = 0;
	unsigned short subLength, temp;
	stSurface *pTempSurface;

	while(1)
      {
		   if(!FindNextChunk(fp, "SURF")) return true;
		   
         // Create a new list to store all the surfaces plus this new one.
		   pTempSurface = new stSurface[m_totalSurfaces + 1];

		   if(m_surfaces)
            {
               // Copy the old list in the new array.
			      memcpy(pTempSurface, m_surfaces, m_totalSurfaces * sizeof(stSurface));
			      delete[] m_surfaces;
		      }

         // Point to the new array that has the entire list so far.
		   m_surfaces = pTempSurface;

         // Point to last surface so we can fill it in.
		   pTempSurface = &m_surfaces[m_totalSurfaces];
		   m_totalSurfaces++;

		   // Read in the length of the surface.
		   if(!ReadIntFromFile(fp, length)) return false;
         if(length % 2) length++;
		   
		   count = 0;
		   buffer[0] = 1;
		   
         // Read in the surface name.
         while(buffer[0])
            {
			      fread(buffer, 1, 1, fp);
			      pTempSurface->name[count++] = buffer[0];
		      }

         // Read in 1 byte.
		   if(count % 2)
            {
			      fread(buffer, 1, 1, fp); count++;
		      }

		   i = 0;
		   buffer[0] = 1;

         // Read in the parent name, we dont use this.
		   while(buffer[0])
            {
			      fread(buffer, 1, 1, fp); i++;
		      }

		   count += i;
         
         // Read in one byte.
         if(i % 2)
            {
			      fread(buffer, 1, 1, fp); count++;
		      }

		   // Load all subchunks.
		   while(count < length)
            {
               // Read in the length of the sub chunk.
			      if(!ReadSubChunkFromFile(fp, buffer, subLength)) return false;
			      if(subLength % 2) subLength++;
			      
               count += (subLength + 6);
			      
               // Make sure this chunk is a color chunk.
               if(strncmp(buffer, "COLR", 4) == 0)
                  {
				         // Read in the R, G, and B color components.
				         if(!ReadFloatFromFile(fp, pTempSurface->red)) return false;
                     if(!ReadFloatFromFile(fp, pTempSurface->green)) return false;
                     if(!ReadFloatFromFile(fp, pTempSurface->blue)) return false;
				         
                     // We don't support color envelopes.
                     if(!ReadShortFromFile(fp, temp)) return false;
				         if(temp != 0) return false;
				         
                     // Skip to the next loop iteration.
                     continue;
			         }
			      
               // Chunk was not found, skip to try the next
			      fseek(fp, subLength, SEEK_CUR);
		      }
	   }

	return true;
}


int CLightWaveObject::AddPolygon(FILE * fp)
{
	unsigned short index;
   unsigned short totalverts;
	stPolygon *polygons;

   // Read in the number of points in this polygon.
	if(!ReadShortFromFile(fp, totalverts)) return 0;
	
	// Initialize a list of polygons of the new size.
	polygons = new stPolygon[m_totalPolygons + 1];
	
   // Error checking.
   if(!polygons) return 0;

   polygons[m_totalPolygons].pointIndex = new int[totalverts];
	
   // Copy the old list of polygons into the new list.
   if(m_polygons)
      {
         memcpy(polygons, m_polygons, sizeof(stPolygon) * m_totalPolygons);
         
         for(int i = 0; i < m_totalPolygons; i++)
            {
               polygons[i].pointIndex = m_polygons[i].pointIndex;
               m_polygons[i].pointIndex = NULL;
            }

	      delete[] m_polygons;
      }

   // Next we take the pointer of our member variable and point it to the new list of polygons.
	m_polygons = polygons;

   // Point to the polygon at the end of the list.
	polygons = &polygons[m_totalPolygons];

   // Loop through and read each index for this polygon.
	for(int j = 0; j < totalverts; j++)
      {
         // Read in vertex index.
		   if(!ReadShortFromFile(fp, index)) return 0;
         
         // Set this point index.
         polygons->pointIndex[j] = (int)index;
	   }

   // Store the total points, init the surface index, and add to the total polygon count.		
   polygons->totalPoints = totalverts;
	polygons->surfaceIndex = 0;
	m_totalPolygons++;

	return (totalverts * 2) + 2;
}


bool CLightWaveObject::FindChunk(FILE *fp, const char *chunk)
{
	char buffer[4];
	int length;
	
	// Move to the start then read in the name.
	fseek(fp, 12, SEEK_SET);
	fread(buffer, 4, 1, fp);

   // Compare it with the one we are looking for.
	while(strncmp(buffer, chunk, 4))
      {
		   // Read length.
		   if(!ReadIntFromFile(fp,length)) return false;
		   
         // Check if length is evenly divided into 2.
         if(length % 2) length++;
		   
         // Move to next position.
         if(fseek(fp, length, SEEK_CUR)) return false;
		   
         // Read data.
         fread(buffer, 4, 1, fp);
	   }

	return true;
}


bool CLightWaveObject::FindNextChunk(FILE *fp, const char *chunk)
{
	char buffer[4];
	int length;

   // Read in the chunk name.
	fread(buffer, 4, 1, fp);
	
   // Keep looping through until we find what we are looking for.
	while(strncmp(buffer, chunk, 4))
      {
		   // Read the length.
		   if(!ReadIntFromFile(fp, length)) return false;
		   
         // Check if length is evenly divided into 2.
         if(length % 2) length++;
		   
         // Move to the next chunk.
         if(fseek(fp, length, SEEK_CUR)) return false;

         // Read chunk.
		   fread(buffer, 4, 1, fp);
	   }

	return true;
}


void CLightWaveObject::CalculateNormals()
{
	int a = 0, b = 0, c = 0;      // Holds index into points list.
	stPolygon *polygons = m_polygons;  // A pointer to the list of polygons in this model.
	CVector4 normal;              // Temporarily stores the normal for each polygon.
	CVector4 triangle[3];         // Temporarily stores a triangle that makes up part of a polygon.
	
	// Here we loop through all the polygons in the object model and calculate the
	// normal for each of them.  We use the CVector4 function Normalize(CVector4 triangle[])
	// to do this for us.
	for(int i = 0; i < m_totalPolygons; i++)
	{
		// We grab the point index values from the polygon list for this polygon.
		a = polygons->pointIndex[0];
		b = polygons->pointIndex[1];
		c = polygons->pointIndex[2];
		
		// We use those values to get 3 points that make us this polygon.
		triangle[0] = m_vertices[a];
		triangle[1] = m_vertices[b];
		triangle[2] = m_vertices[c];
		
		// We call the CVector4 function normalize to normalise the triangle.
		normal.Normalize(triangle);
		
		// Lastly we store those normal values into this polygons normal.
		polygons->normal = normal;
		
		polygons++;
	}
	
	// now we have polygons normals we want per vertex normal
	// so for each vertex we compute the average of all
	// adjacent face normals
	int nbAdj=0;
	
	for(int i=0;i< m_totalPoints;i++)
	{
		polygons = m_polygons;
		for(int j=0; j < m_totalPolygons; j++)
		{
			// We grab the point index values from the polygon list for this polygon.
			a = polygons->pointIndex[0];
			b = polygons->pointIndex[1];
			c = polygons->pointIndex[2];
		
			if (i==a || i==b || i==c)
			{
				nbAdj++;
				m_normals[i]+=polygons->normal;
			}
			polygons++;
		}
		m_normals[i]/=nbAdj;
				
		m_normals[i].Normal();
		nbAdj=0;
		
	}
}


void CLightWaveObject::Shutdown()
{
	// Delete all resources and reset all variables.
	if(m_vertices)
	{
		delete[] m_vertices;
		m_vertices = NULL;
	}
	
	if (m_normals)
	{
		delete[] m_normals;
		m_normals = NULL;
	}
	
	if(m_polygons)
	{
		for(int i = 0; i < m_totalPolygons; i++)
		{
			if(m_polygons[i].pointIndex)
			{
				delete[] m_polygons[i].pointIndex;
				m_polygons[i].pointIndex = NULL;
			}
		}
		delete[] m_polygons;
		m_polygons = NULL;
	}
	
	if(m_surfaces)
	{
		delete[] m_surfaces;
		m_surfaces = NULL;
	}

	m_totalPoints = 0;
	m_totalPolygons = 0;
	m_totalSurfaces = 0;
}


