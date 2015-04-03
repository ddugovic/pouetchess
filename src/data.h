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

#ifndef _POUETCHESS_DATA_H__
#define _POUETCHESS_DATA_H__


/*
 * On Windows, the data directory is always in 
 * the executable directory, so it will be
 * in the hard coded "data" directory.
 *
 * On Other OS the data directory is given
 * by scons when configure.
 */
#ifdef _WIN32
#define DATA_INSTALL_PREFIX "data"
#else
#include "config.h"
#endif


#define PC_DATA_GUI							DATA_INSTALL_PREFIX"/gui/"
#define PC_DATA_GUI_TEXTURES				DATA_INSTALL_PREFIX"/textures/"

#define PC_DATA_TEXTURE_BACKGROUND			DATA_INSTALL_PREFIX"/textures/background.jpg"
#define	PC_DATA_TEXTURE_BOARD				DATA_INSTALL_PREFIX"/textures/board.jpg"
#define PC_DATA_TEXTURE_SKYBOX				DATA_INSTALL_PREFIX"/textures/skybox/s%d.jpg"
#define PC_DATA_ICON_POUETCHESS				DATA_INSTALL_PREFIX"/icons/pouetChess.bmp"

#define PC_DATA_MODEL_ROOK					DATA_INSTALL_PREFIX"/3DModels/rook.lwo"
#define PC_DATA_MODEL_KNIGHT				DATA_INSTALL_PREFIX"/3DModels/knight.lwo"
#define PC_DATA_MODEL_BISHOP				DATA_INSTALL_PREFIX"/3DModels/bishop.lwo"
#define PC_DATA_MODEL_QUEEN					DATA_INSTALL_PREFIX"/3DModels/queen.lwo"
#define PC_DATA_MODEL_KING					DATA_INSTALL_PREFIX"/3DModels/king.lwo"
#define PC_DATA_MODEL_PAWN					DATA_INSTALL_PREFIX"/3DModels/pawn.lwo"
#define PC_DATA_MODEL_BOARD					DATA_INSTALL_PREFIX"/3DModels/board.lwo"
#define PC_DATA_MODEL_ARROW					DATA_INSTALL_PREFIX"/3DModels/arrow.lwo"

#define PC_DATA_FAILE_OBK					DATA_INSTALL_PREFIX"/faile.obk"



#endif


