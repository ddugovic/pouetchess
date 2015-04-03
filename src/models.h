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

#ifndef ___MODELS_H___
#define ___MODELS_H___



bool load3DModels();
void unload3DModels();
void createModelList();
void deleteModelsList();

#define TOTAL_NB_MODELS		16

#define MODEL_PAWN			0
#define MODEL_ROOK			1
#define MODEL_KNIGHT		2
#define MODEL_BISHOP		3
#define MODEL_QUEEN			4
#define MODEL_KING			5
#define MODEL_ARROW			6
#define MODEL_BOARD			7
#define MODEL_SQUARE		8
#define MODEL_CHESSBOARD	9
#define MODEL_SKYBOX_0		10
#define MODEL_SKYBOX_1		11
#define MODEL_SKYBOX_2		12
#define MODEL_SKYBOX_3		13
#define MODEL_SKYBOX_4		14
#define MODEL_SKYBOX_5		15




#endif

