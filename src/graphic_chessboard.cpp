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


#include "graphic_chessboard.h"
#include "models.h"
#include "data.h"
#include "utils.h"
#include "raytracing.h"
#include "globalInfos.h"
#include <string>
#include <math.h>


#ifndef PI
#define PI 3.141592f
#endif

#ifndef _2__PI_
#define _2__PI_ 6.28319f
#endif

// --------------------------------------------------------------------- //
// ----------------------- CGraphicChessboard -------------------------- //
// --------------------------------------------------------------------- //


CGraphicChessboard::CGraphicChessboard()
{
	from=dest=99;
}

CGraphicChessboard::~CGraphicChessboard()
{

}

void CGraphicChessboard::Move_piece(int cfrom, int cdest)
{
	from=cfrom;
	dest=cdest;
	t=0.0f;
}








const GLfloat material_white_Ka[] = {0.2f,0.2f,0.2f,1.0f};
const GLfloat material_white_Kd[] = {0.9f,0.85f,0.55f,1.0f};
const GLfloat material_white_Ks[] = {0.7f,0.7f,0.7f,1.0f};
const GLfloat material_white_Ke[] = {0.0f,0.0f,0.0f,1.0f};

const GLfloat material_black_Ka[] = {0.05f,0.05f,0.05f,1.0f};
const GLfloat material_black_Kd[] = {0.1f,0.1f,0.1f,1.0f};
const GLfloat material_black_Ks[] = {1.0f,1.0f,1.0f,1.0f};
const GLfloat material_black_Ke[] = {0.0f,0.0f,0.0f,1.0f};

// "gold" material found at http://devernay.free.fr/cours/opengl/materials.html
const GLfloat material_chessboard_Ka[] = {0.24725f,0.1995f,0.0745f,1.0f};
const GLfloat material_chessboard_Kd[] = {0.75164f,0.60648f,0.22648f,1.0f};
const GLfloat material_chessboard_Ks[] = {0.628281f,0.555802f,0.366065f,1.0f};
const GLfloat material_chessboard_Ke[] = {0.0f,0.0f,0.0f,1.0f};

// arrow material
const GLfloat material_arrow_Ka[] = {0.0f,1.0f,0.0f,1.0f};
const GLfloat material_arrow_Kd[] = {0.2f,1.0f,0.2f,1.0f};
const GLfloat material_arrow_Ks[] = {0.0f,1.0f,0.0f,1.0f};
const GLfloat material_arrow_Ke[] = {0.0f,0.0f,0.0f,1.0f};

// --------------------------------------------------------------------- //
// ----------------------- C3DGraphicChessboard ------------------------ //
// --------------------------------------------------------------------- //

C3DGraphicChessboard::C3DGraphicChessboard()
{
	texture_board			= NULL;
	for(int i=0;i<6;i++)
		texture_skybox[i]	= NULL;
}


C3DGraphicChessboard::~C3DGraphicChessboard()
{
	
}

bool C3DGraphicChessboard::load()
{
	
	models_list=pGlobalInfos->GetModelsList();
	
	texture_board = new CBasicTexture;
	if (false==texture_board->load(PC_DATA_TEXTURE_BOARD,false))
	{
		Logger::writeErrorLog("Loading of :"PC_DATA_TEXTURE_BOARD" failed");
		return false;
	}

	texture_board->SetTexParameter(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	texture_board->SetTexParameter(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	for(int i=0;i<6;i++)
	{
		char texture_filename[50];
		sprintf(texture_filename,PC_DATA_TEXTURE_SKYBOX,i),
		texture_skybox[i] = new CBasicTexture;
		if (false==texture_skybox[i]->load(texture_filename,false))
		{
			printf("Loading of :"PC_DATA_TEXTURE_SKYBOX" failed\n",i);
			return false;
		}
		
		texture_skybox[i]->SetTexParameter(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); 
		texture_skybox[i]->SetTexParameter(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE); 
	}	
	
	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	const GLfloat lightPosition0    [] = { 7.0f, 10.0f, 7.0f, 0.0f };	
	const GLfloat lightColorAmbient [] = { 0.3f, 0.3f, 0.3f, 1.0f };	
	const GLfloat lightColorDiffuse [] = { 1.0f, 1.0f, 1.0f, 1.0f };	
	const GLfloat lightColorSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };	
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightColorAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColorDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColorSpecular);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT,  lightColorAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightColorDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColorSpecular);
	
	angleCameraZ	= angleCameraZ_default	= 1.05f  ;
	cameraDist		= cameraDist_default	= 23.0f  ;
	angleCameraX	= angleCameraX_default	= -1.57f ;
	
	default_camera_position=true;
	
	mouseX=pGlobalInfos->MouseX();
	mouseY=pGlobalInfos->MouseY();
		
	return true;

}

void C3DGraphicChessboard::unload()
{
	SAFE_DELETE(texture_board);
	for(int i=0;i<6;i++)
		SAFE_DELETE(texture_skybox[i]);
}


void C3DGraphicChessboard::display(int* a_board)
{
	// First update camera position and orientation
	update_camera();
			
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
		
	glColor3f(1.0f,1.0f,1.0f);
	texture_board->bind();
	glCallList(models_list+MODEL_BOARD);
	
	DisplaySkyBox();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	
	const GLfloat lightPosition1    [] = { 7.0f, 10.0f, 7.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	
	// display chessboard
	glMaterialfv(GL_FRONT,GL_AMBIENT,material_chessboard_Ka);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,material_chessboard_Kd);
	glMaterialfv(GL_FRONT,GL_SPECULAR,material_chessboard_Ks);
	glMaterialfv(GL_FRONT,GL_EMISSION,material_chessboard_Ke);
	glMaterialf(GL_FRONT,GL_SHININESS,51.2f);//51.2f = 128.0f*0.4f
		
	glPushMatrix();
	glTranslatef(7.0f,-0.1f,7.0f);
	glCallList(models_list+MODEL_CHESSBOARD);
	glPopMatrix();

	glEnable(GL_LIGHT1);
	glEnable(GL_BLEND);
	
	// diplay all the pieces
	int a,b,c;
	
	for (a = 1; a <= 8; a++) 
	{
		for (b = 0; b <= 11; b++) 
		{
			c = 24 + a*12 -b;
			
			switch(a_board[c])
			{
				case 1:  DisplayPiece(models_list+MODEL_PAWN,b-3,a-1,WHITE);	break;
				case 2:  DisplayPiece(models_list+MODEL_PAWN,b-3,a-1,BLACK);	break;
				case 3:  DisplayPiece(models_list+MODEL_KNIGHT,b-3,a-1,WHITE);	break;
				case 4:  DisplayPiece(models_list+MODEL_KNIGHT,b-3,a-1,BLACK);	break;
				case 5:  DisplayPiece(models_list+MODEL_KING,b-3,a-1,WHITE);	break;
				case 6:  DisplayPiece(models_list+MODEL_KING,b-3,a-1,BLACK);	break;
				case 7:  DisplayPiece(models_list+MODEL_ROOK,b-3,a-1,WHITE);	break;
				case 8:  DisplayPiece(models_list+MODEL_ROOK,b-3,a-1,BLACK);	break;
				case 9:  DisplayPiece(models_list+MODEL_QUEEN,b-3,a-1,WHITE);	break;
				case 10: DisplayPiece(models_list+MODEL_QUEEN,b-3,a-1,BLACK);	break;
				case 11: DisplayPiece(models_list+MODEL_BISHOP,b-3,a-1,WHITE);	break;
				case 12: DisplayPiece(models_list+MODEL_BISHOP,b-3,a-1,BLACK);	break;	
				default: break;	
			}
		}
	}
}



void C3DGraphicChessboard::DisplayPiece(GLuint piece, int posX, int posZ, Piece_color color)
{
	glPushMatrix();
	
	// if the piece to display is going from 'from' 
	// to 'dest' : let's animate it ! 
	if (posX+8*posZ==dest)
	{
		
		if (t<=1540.0f)
			t+=pGlobalInfos->ElapsedTime();
		else
			dest=99;// disable animation, any value above 64 works
		
		GLfloat pax=posX*2.0f;
		GLfloat paz=posZ*2.0f;
		GLfloat px=from%8*2.0f;
		GLfloat py=0.02f;
		GLfloat pz=from/8*2.0f;
		
		if (piece==(models_list+MODEL_KNIGHT))
			glTranslatef(sin(t/1000.0f)*(pax-px)+px,py+5.0f*sin(t/500.0f),sin(t/1000.0f)*(paz-pz)+pz);
		else
			glTranslatef(sin(t/1000.0f)*(pax-px)+px,py,sin(t/1000.0f)*(paz-pz)+pz);
	}
	else
	{
		glTranslatef(posX*2.0f,0.02f,posZ*2.0f);
	}
	
	if (color==WHITE)
	{
		glMaterialfv(GL_FRONT,GL_AMBIENT,material_white_Ka);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,material_white_Kd);
		glMaterialfv(GL_FRONT,GL_SPECULAR,material_white_Ks);
		glMaterialfv(GL_FRONT,GL_EMISSION,material_white_Ke);
		glMateriali(GL_FRONT,GL_SHININESS,128);
	}
	else if (color==BLACK)
	{
			
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material_black_Ka);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material_black_Kd);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material_black_Ks);
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material_black_Ke);
		glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,15);
			
		glRotatef(180.0f,0.0f,1.0f,0.0f); // turn around little piece :)
	}


	glCallList(piece);
	
	
	glPopMatrix();
	
}

void C3DGraphicChessboard::DisplaySkyBox()
{
	glDisable(GL_LIGHTING);
	
	// ground
	texture_skybox[1]->bind();
	glCallList(models_list+MODEL_SKYBOX_0);
	
	// sky
	texture_skybox[4]->bind();
	glCallList(models_list+MODEL_SKYBOX_1);
	
	// left
	texture_skybox[0]->bind();
	glCallList(models_list+MODEL_SKYBOX_2);
	
	// right
	texture_skybox[3]->bind();
	glCallList(models_list+MODEL_SKYBOX_3);
	
	// front
	texture_skybox[5]->bind();
	glCallList(models_list+MODEL_SKYBOX_4);
	
	// back
	texture_skybox[2]->bind();
	glCallList(models_list+MODEL_SKYBOX_5);
	
	glEnable(GL_LIGHTING);
		
}


void C3DGraphicChessboard::update_camera()
{
	Uint32 dt=pGlobalInfos->ElapsedTime();
	
	if (angleCameraX>_2__PI_)
		angleCameraX-=_2__PI_;
	if (angleCameraX<-_2__PI_)
		angleCameraX+=_2__PI_;
	
	if (default_camera_position)
	{
		const GLfloat epsilon1=0.07f;
		const GLfloat epsilon2=0.02f;
		GLfloat Xspeed=600.0f;
		// prevent a camera bug if the fps is too low
		if (dt>20)
			dt=20;
	
		if ((angleCameraX_default-epsilon1)<angleCameraX && angleCameraX<(angleCameraX_default+epsilon1)) 
			Xspeed=1500.0f;
		
		if (angleCameraX>angleCameraX_default+PI)
			angleCameraX+=dt/Xspeed;
		else if (angleCameraX>angleCameraX_default)
			angleCameraX-=dt/Xspeed;
		
		if (angleCameraX<angleCameraX_default-PI)
			angleCameraX-=dt/Xspeed;
		else if (angleCameraX<angleCameraX_default)
			angleCameraX+=dt/Xspeed;
			
		if (angleCameraZ<angleCameraZ_default)
			angleCameraZ+=dt/600.0f;
		if (angleCameraZ>angleCameraZ_default)
			angleCameraZ-=dt/600.0f;
		
		if (cameraDist<cameraDist_default)
			cameraDist+=dt/50.0f;
		if (cameraDist>cameraDist_default)
			cameraDist-=dt/50.0f;
		
		if (angleCameraX>angleCameraX_default+_2__PI_)
			angleCameraX-=_2__PI_;
		if (angleCameraX<-angleCameraX_default-_2__PI_)
			angleCameraX+=_2__PI_;
	
		
		
		if ((angleCameraX_default-epsilon2)<angleCameraX &&
				   angleCameraX<(angleCameraX_default+epsilon2) && 
				   (angleCameraZ_default-epsilon1)<angleCameraZ &&
				   angleCameraZ<(angleCameraZ_default+epsilon1) &&
				   (cameraDist_default-0.5f)<cameraDist &&
				   cameraDist<(cameraDist_default+0.5f) )
		{
			default_camera_position=false;
			angleCameraZ=angleCameraZ_default;
			angleCameraX=angleCameraX_default;
			cameraDist=cameraDist_default;			
		}
	}
	else
	{
		if (pGlobalInfos->KeyDown(SDLK_DOWN))
			angleCameraZ-=0.001f*dt;
		if (pGlobalInfos->KeyDown(SDLK_UP))
			angleCameraZ+=0.001f*dt;
		
		if (pGlobalInfos->KeyDown(SDLK_RIGHT))
			angleCameraX-=0.001f*dt;
		if (pGlobalInfos->KeyDown(SDLK_LEFT))
			angleCameraX+=0.001f*dt;
		
		if (pGlobalInfos->KeyDown(SDLK_PAGEUP))
			cameraDist-=dt/50.0f;
		if (pGlobalInfos->KeyDown(SDLK_PAGEDOWN))
			cameraDist+=dt/50.0f;
		
		
		int mX=pGlobalInfos->MouseX();
		int mY=pGlobalInfos->MouseY();

		// right mouse click is used to rotate the chessboard
		if (pGlobalInfos->IsRightMouseButtonDown())
		{		
			angleCameraX+=0.001f*dt*(mX-mouseX)/5.0f;
			angleCameraZ+=0.001f*dt*(mY-mouseY)/5.0f;
		}
		
		mouseX=mX;
		mouseY=mY;
		
		if (pGlobalInfos->OnMouseWheelUp())
			cameraDist-=dt/6.0f;
		if (pGlobalInfos->OnMouseWheelDown())
			cameraDist+=dt/6.0f;
		
		if (angleCameraZ>1.57f) // a little bit less than Pi/2
			angleCameraZ=1.57f; // a little bit less than Pi/2
		if (angleCameraZ<-0.7f)
			angleCameraZ=-0.7f;
		if (cameraDist>80.0f)
			cameraDist=80.0f;
		if (cameraDist<22.0f)
			cameraDist=22.0f;
	
	}

	
	cameraX=cos(angleCameraX)*cos(angleCameraZ)*cameraDist+7.0f;
	cameraY=sin(angleCameraZ)*cameraDist;
	cameraZ=sin(angleCameraX)*cos(angleCameraZ)*cameraDist+7.0f;
	
	
	// Positionne la caméra
	gluLookAt(cameraX, cameraY, cameraZ,	// camera position
			  7.0f, 0.0f, 7.0f,				// center of the chessboard
			  0.0f, 1.0f, 0.0f);			// Camera orientation : Y axis
}




void C3DGraphicChessboard::display_selection_arrow()
{
	static GLfloat y_rot=0.0f;
	y_rot+=pGlobalInfos->ElapsedTime()/10.0f;
	
	glEnable(GL_LIGHTING);
	
	glPushMatrix();
	
	glScalef(0.4f,0.4f,0.4f);
	glTranslatef(2.5f,13.5f+sin(SDL_GetTicks()/120.0f),2.5f);
	glRotatef(180.0f,1.0f,0.0f,0.0f);
	glRotatef(y_rot,0.0f,1.0f,0.0f);
	
	glMaterialfv(GL_FRONT,GL_AMBIENT,material_arrow_Ka);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,material_arrow_Kd);
	glMaterialfv(GL_FRONT,GL_SPECULAR,material_arrow_Ks);
	glMaterialfv(GL_FRONT,GL_EMISSION,material_arrow_Ke);
	glMateriali(GL_FRONT,GL_SHININESS,128);
	
	glCallList(models_list+MODEL_ARROW);
	glPopMatrix();
	
	glDisable(GL_LIGHTING);
	
}


bool C3DGraphicChessboard::clickOnChessBoard(int mouseX, int mouseY, int* squareX, int* squareY)
{
	if (angleCameraZ<0.02f)
		return false;
	
	double Vertices[12]={-1.0,0.0,-1.0,  15.0,0.0,-1.0,  15.0,0.0,15.0,  -1.0,0.0,15.0};// board coords
	double Intersec[3]={0};
	 
	if (SelectPolygon(cameraX, cameraY, cameraZ, Vertices, Intersec, mouseX, mouseY))
	{
		*squareX=(int)((Intersec[0]+1.0)/2.0);
		*squareY=(int)((Intersec[2]+1.0)/2.0);
		
		// should never happen but 4 tests cost nothing
		if (*squareX<0)
			*squareX=0;
		if (*squareX>7)
			*squareX=7;
		if (*squareY<0)
			*squareY=0;
		if (*squareY>7)
			*squareY=7;
		
		return true;
	}
	
	return false;
}


void C3DGraphicChessboard::displayPossibleMoves(int squareX, int squareY, unsigned char* possible_moves)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
		
		// draw the blue square
	glColor4f(0.0f,0.0f,0.9f,0.5f);
	glPushMatrix();
	glTranslatef(-1.0f+2.0f*squareX,0.01f,-1.0f+2.0f*squareY);
	glCallList(models_list+MODEL_SQUARE);
	if (pGlobalInfos->GetSelectionArrow())
		display_selection_arrow();
	glPopMatrix();

	// display move possibilities for the player
	int i=0;
	while (possible_moves[i]<65)
	{
		int PossibleXSquare=possible_moves[i]%8;
		int PossibleZSquare=possible_moves[i]/8;
			
		GLfloat alpha_value=0.4f+0.2*sin(SDL_GetTicks()/300.0f);
			
		glColor4f(0.0f,0.9f,0.0f,alpha_value);
		glPushMatrix();
		glTranslatef(-1.0f+2.0f*PossibleXSquare,0.01f,-1.0f+2.0f*PossibleZSquare);
		glCallList(models_list+MODEL_SQUARE);
		glPopMatrix();
			
		i++;
	}
	glEnable(GL_LIGHTING);
}


void C3DGraphicChessboard::changeSideToPlay(Piece_color side)
{
	if (side==WHITE)
		angleCameraX_default=-1.57f;
	else
		angleCameraX_default=1.57f;
						
	default_camera_position=true;// reset camera position
}

