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
 
 
#ifndef __TEXTURE_H__
#define __TEXTURE_H__


#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


/**
 * @class CBasicTexture
 * @brief easy texture management for opengl with SDL_image as image loader
 *
 *
 * Use SDL_image to load an image to the system memory and 
 * opengl to load this image into the graphic card's memory
 * The image is removed from the system memory once in the 
 * graphic card's memory.
 *
 */ 
class CBasicTexture  
{
	public:
		CBasicTexture();
		virtual ~CBasicTexture();

		//! loads an image. Formats : BMP, PNM, XPM, LBM, PCX, GIF, JPEG, PNG, TGA. (Actually all SDL_image supported formats)
		bool load (const char *file, bool mipmap);
		//! Same as glBindTexture()
		void bind();
		void SetTexParameter (GLenum type, GLint mode);
		void Set_Blending  (GLint mode);
		void Set_Blending  (GLint mode , GLfloat *env_color);
		//! unload texture from graphic card memory
		void unload();
		//! @return texture's id
		inline GLuint GetId(){return id;}

	private:
		GLuint id;
		int width;
		int height;
		bool mipmapping;

};


#endif
