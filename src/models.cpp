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
 
 
#include <math.h>
#include "globalInfos.h"
#include "data.h"
#include "models.h"
#include "CLightWaveObject.h"


void draw_lightWave(const CLightWaveObject& model);
void draw_board();
void draw_square();
void draw_skybox_0();
void draw_skybox_1();
void draw_skybox_2();
void draw_skybox_3();
void draw_skybox_4();
void draw_skybox_5();

CLightWaveObject objRook;
CLightWaveObject objKnight;
CLightWaveObject objBishop;
CLightWaveObject objQueen;
CLightWaveObject objKing;
CLightWaveObject objPawn;
CLightWaveObject objChessboard;
CLightWaveObject objArrow;



bool load3DModels()
{
	if(!objRook.LoadModel(PC_DATA_MODEL_ROOK))
	{
		printf("Loading of "PC_DATA_MODEL_ROOK" failed\n");
		return false;
	}
	
	if(!objKnight.LoadModel(PC_DATA_MODEL_KNIGHT)) 	
	{
		printf("Loading of "PC_DATA_MODEL_KNIGHT" failed\n");
		return false;
	}
	
	if(!objBishop.LoadModel(PC_DATA_MODEL_BISHOP)) 	
	{
		printf("Loading of "PC_DATA_MODEL_BISHOP" failed\n");
		return false;
	}
	
	if(!objQueen.LoadModel(PC_DATA_MODEL_QUEEN)) 	
	{
		printf("Loading of "PC_DATA_MODEL_QUEEN" failed\n");
		return false;
	}
	
	if(!objKing.LoadModel(PC_DATA_MODEL_KING)) 	
	{
		printf("Loading of "PC_DATA_MODEL_KING" failed\n");
		return false;
	}
	
	if(!objPawn.LoadModel(PC_DATA_MODEL_PAWN)) 	
	{
		printf("Loading of "PC_DATA_MODEL_PAWN" failed\n");
		return false;
	}
	
	if(!objChessboard.LoadModel(PC_DATA_MODEL_BOARD)) 	
	{
		printf("Loading of "PC_DATA_MODEL_BOARD" failed\n");
		return false;
	}
	
	if(!objArrow.LoadModel(PC_DATA_MODEL_ARROW)) 	
	{
		printf("Loading of "PC_DATA_MODEL_ARROW" failed\n");
		return false;
	}
	
	return true;
}

void unload3DModels()
{
	objRook.Shutdown();
	objKnight.Shutdown();
	objBishop.Shutdown();
	objQueen.Shutdown();
	objKing.Shutdown();
	objPawn.Shutdown();
	objChessboard.Shutdown();
	objArrow.Shutdown();
}


void createModelList()
{
	GLuint mlist=glGenLists(TOTAL_NB_MODELS);
	pGlobalInfos->SetModelsList(mlist);
  
	glNewList(mlist+MODEL_KING, GL_COMPILE);
	draw_lightWave(objKing);
	glEndList();

	glNewList(mlist+MODEL_QUEEN, GL_COMPILE);
	draw_lightWave(objQueen);
	glEndList();

	glNewList(mlist+MODEL_BISHOP, GL_COMPILE);
	draw_lightWave(objBishop);
	glEndList();

	glNewList(mlist+MODEL_KNIGHT, GL_COMPILE);
	draw_lightWave(objKnight);
	glEndList();

	glNewList(mlist+MODEL_ROOK, GL_COMPILE);
	draw_lightWave(objRook);
	glEndList();

	glNewList(mlist+MODEL_PAWN, GL_COMPILE);
	draw_lightWave(objPawn);
	glEndList();
	
	glNewList(mlist+MODEL_ARROW, GL_COMPILE);
	draw_lightWave(objArrow);
	glEndList();
  
	glNewList(mlist+MODEL_BOARD,GL_COMPILE);
	draw_board();
	glEndList();
  
	glNewList(mlist+MODEL_SQUARE,GL_COMPILE);
	draw_square();
	glEndList();
	
	glNewList(mlist+MODEL_CHESSBOARD,GL_COMPILE);
	draw_lightWave(objChessboard);
	glEndList();
	
	glNewList(mlist+MODEL_SKYBOX_0,GL_COMPILE);
	draw_skybox_0();
	glEndList();
	
	glNewList(mlist+MODEL_SKYBOX_1,GL_COMPILE);
	draw_skybox_1();
	glEndList();
	
	glNewList(mlist+MODEL_SKYBOX_2,GL_COMPILE);
	draw_skybox_2();
	glEndList();
	
	glNewList(mlist+MODEL_SKYBOX_3,GL_COMPILE);
	draw_skybox_3();
	glEndList();
	
	glNewList(mlist+MODEL_SKYBOX_4,GL_COMPILE);
	draw_skybox_4();
	glEndList();
	
	glNewList(mlist+MODEL_SKYBOX_5,GL_COMPILE);
	draw_skybox_5();
	glEndList();
	
}

void deleteModelsList()
{
	glDeleteLists(pGlobalInfos->GetModelsList(),TOTAL_NB_MODELS);
}



void draw_lightWave(const CLightWaveObject& model)
{
	// Pointers to the surface, polygon list, and vertex point.  These will be used
	// to draw the object.
	CVector4 *points;
	stSurface *surface;
	stPolygon *polygons = model.m_polygons;

	for(int i = 0; i < model.m_totalPolygons; i++)
	{
		if(polygons->totalPoints == 3) glBegin(GL_TRIANGLES);
		else if(polygons->totalPoints == 4) glBegin(GL_QUADS);
		else glBegin(GL_POLYGON);

		// Get this polygon's surface information.
		surface = &model.m_surfaces[polygons->surfaceIndex];

         // Set this poly's color and normal.
		glColor3f(surface->red, surface->green, surface->blue);
		//glNormal3f(-polygons->normal.x, -polygons->normal.y, -polygons->normal.z);

         // Here we draw a surface.  Get each point that makes up this shape and draw them.
		for(int j = 0; j < polygons->totalPoints; j++)
		{
			points = &model.m_normals[polygons->pointIndex[j]];
			glNormal3f(-points->x, -points->y, -points->z);
			points = &model.m_vertices[polygons->pointIndex[j]];
			glVertex3f(points->x, points->y, points->z);
		}

         // Move to the next polygon.
		polygons++;

		glEnd();
	}
}



void draw_board()
{
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(15.0f, 0.0f, -1.0f);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(15.0f, 0.0f, 15.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, 15.0f);
	
	glEnd();
}


void draw_square()
{
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 0.0f, 2.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 2.0f);
	
	
	glEnd();
}



void draw_skybox_0()
{
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-500.0f, -500.0f, -500.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(500.0f, -500.0f, -500.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(500.0f, -500.0f, 500.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-500.0f, -500.0f, 500.0f);
	glEnd();
}


void draw_skybox_1()
{
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-500.0f, 500.0f, -500.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(500.0f, 500.0f, -500.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(500.0f, 500.0f, 500.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-500.0f, 500.0f, 500.0f);
	glEnd();
}

void draw_skybox_2()
{
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-500.0f, -500.0f, -500.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-500.0f, -500.0f, 500.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-500.0f, 500.0f, 500.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-500.0f, 500.0f, -500.0f);
	glEnd();
}

void draw_skybox_3()
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(500.0f, -500.0f, -500.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(500.0f, -500.0f, 500.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(500.0f, 500.0f, 500.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(500.0f, 500.0f, -500.0f);
	glEnd();
}


void draw_skybox_4()
{
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-500.0f, -500.0f, -500.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(500.0f, -500.0f, -500.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(500.0f, 500.0f, -500.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-500.0f, 500.0f, -500.0f);
	glEnd();
}

void draw_skybox_5()
{
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-500.0f, -500.0f, 500.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(500.0f, -500.0f, 500.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(500.0f, 500.0f, 500.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-500.0f, 500.0f, 500.0f);
	glEnd();
}





