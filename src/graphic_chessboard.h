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


#ifndef ___GRAPHIC_CHESSBOARD_H___
#define ___GRAPHIC_CHESSBOARD_H___

#include <SDL/SDL_opengl.h>
#include "texture.h"


enum Piece_color
{
	BLACK,
	WHITE,
};



class CGraphicChessboard
{
	public:
		CGraphicChessboard();
		virtual ~CGraphicChessboard();
	
		void Move_piece(int cfrom, int cdest);
				
		virtual bool load()								= 0;
		virtual void unload()							= 0;
		virtual void display(int* a_board)				= 0;
		virtual void display_selection_arrow()			= 0;
		virtual void resetCameraPos()					= 0;
		virtual void changeSideToPlay(Piece_color side)	= 0;
		virtual bool clickOnChessBoard(int mouseX, 
									   int mouseY, 
									   int* squareX, 
									   int* squareY)	= 0;
		virtual void displayPossibleMoves(int squareX, 
										  int squareY,
						unsigned char* possible_moves)	= 0;
		
		
	protected:
		unsigned char from,dest;
		GLfloat t;
};


class C3DGraphicChessboard : public CGraphicChessboard
{
	public:
		
		C3DGraphicChessboard();
		virtual ~C3DGraphicChessboard();
	
		bool load();
		void unload();
		void display(int* a_board);
		void display_selection_arrow();
		void resetCameraPos()	{default_camera_position=true;}
		bool clickOnChessBoard(int mouseX, int mouseY, int* squareX, int* squareY);
		void displayPossibleMoves(int squareX, int squareY, unsigned char* possible_moves);
		void changeSideToPlay(Piece_color side);
		void init();
	
	private:
		CBasicTexture* texture_board;
		CBasicTexture* texture_skybox[6];
		GLuint models_list;
		
		int mouseX;
		int mouseY;
		
		GLfloat cameraX, cameraY, cameraZ; // camera position
		GLfloat angleCameraX, angleCameraZ, cameraDist;
		GLfloat angleCameraX_default,angleCameraZ_default,cameraDist_default;
		bool default_camera_position;
		
		void DisplayPiece(GLuint piece, int posX, int posZ, Piece_color color);
		void DisplaySkyBox();
		void update_camera();

};


#endif

