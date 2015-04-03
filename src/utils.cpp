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

#include "utils.h"

/**************************************************************************************/
void go_2D()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0f, 1024.0f, 768.0f, 0.0f, 0.0f, 1.0f );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
}


/**************************************************************************************/
void exit_2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}


/**************************************************************************************/
void draw_rectangle(int X, int Y,int width,int height)
{
	glBegin(GL_LINE_LOOP);
		glVertex2i(X, Y);
		glVertex2i(X, Y+height);
		glVertex2i(X+width,Y+height);
		glVertex2i(X+width,Y);
		glVertex2i(X, Y);
	glEnd();
}

/**************************************************************************************/
void draw_rectangle_2(int x1, int y1,int x2,int y2)
{
	glBegin(GL_LINE_LOOP);
		glVertex2i(x1, y1);
		glVertex2i(x1, y2);
		glVertex2i(x2, y2);
		glVertex2i(x2, y1);
		glVertex2i(x1, y1);
	glEnd();
}

/**************************************************************************************/
void draw_filled_rectangle(int x1, int y1,int x2,int y2)
{
	glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x1, y2);
		glVertex2i(x2, y2);
		glVertex2i(x2, y1);
	glEnd();
}


/**************************************************************************************/
void draw_image(int X, int Y,int width,int height,CBasicTexture* image)
{
	glEnable(GL_TEXTURE_2D);
	image->bind();
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);		glVertex2i(X, Y);
		glTexCoord2f(0.0f, 1.0f);		glVertex2i(X, Y+height);
		glTexCoord2f(1.0f, 1.0f);		glVertex2i(X+width,Y+height);
		glTexCoord2f(1.0f, 0.0f);		glVertex2i(X+width,Y);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}



/**************************************************************************************/
void clearScreen()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapBuffers();
}



/**************************************************************************************/
/**************************************************************************************/
GLfloat alpha_fade;


void init_fade_in()
{
	alpha_fade=0.0f;
}

void init_fade_out()
{
	alpha_fade=1.0f;
}


/**************************************************************************************/
bool gl_fade_in(GLfloat speed)
{
	if (alpha_fade<1.0f)
		alpha_fade+=speed;
	else
	{
		alpha_fade=0.0f;
		return false;
	}
	
	glColor4f(0.0f,0.0f,0.0f,alpha_fade);
	
	glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, 768);
		glVertex2i(1024, 768);
		glVertex2i(1024, 0);
	glEnd();
	
	return true;
}


/**************************************************************************************/
bool gl_fade_out(GLfloat speed)
{
	if (alpha_fade>0.0f)
		alpha_fade-=speed;
	else
	{
		alpha_fade=0.0f;
		return false;
	}
	
	glColor4f(0.0f,0.0f,0.0f,alpha_fade);
	
	glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, 768);
		glVertex2i(1024, 768);
		glVertex2i(1024, 0);
	glEnd();
	
	return true;	
}

/**************************************************************************************/
/**************************************************************************************/

