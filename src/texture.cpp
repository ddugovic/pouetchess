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

#include "texture.h"
#include <SDL/SDL_image.h>
#include <string.h>


CBasicTexture::CBasicTexture()
{

}

CBasicTexture::~CBasicTexture()
{
	unload();
}



// Formats : BMP, PNM, XPM, LBM, PCX, GIF, JPEG, PNG, TGA.
bool CBasicTexture::load(const char *file, bool mipmap )	
{
	SDL_Surface *surface;
	int pixel;

	surface = IMG_Load(file);
	if (surface == NULL)
		return false;
		
	pixel=surface->format->BitsPerPixel;
	width=surface->w;
	height=surface->h;
	mipmapping=mipmap;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);


	if (mipmap)
	{
		if ( pixel == 24)
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D,4,width, height, GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

		SetTexParameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		SetTexParameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		if ( pixel == 24)
			glTexImage2D(GL_TEXTURE_2D,0,3,width, height,0, GL_RGB,GL_UNSIGNED_BYTE,surface->pixels);
		else
			glTexImage2D(GL_TEXTURE_2D,0,4,width, height,0, GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

		SetTexParameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		SetTexParameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	SDL_FreeSurface(surface);
	surface = NULL;
	
	return true;
}



void CBasicTexture::bind()
{
	glBindTexture ( GL_TEXTURE_2D, id);
}

void CBasicTexture::SetTexParameter (GLenum type, GLint mode)
{
	bind();

	glTexParameteri(GL_TEXTURE_2D,
					type,
					mode);
}


void CBasicTexture::Set_Blending (GLint mode, GLfloat *env_color)
{
	bind();
	if (mode == GL_BLEND)
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLfloat) mode);
		glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,(GLfloat*)env_color);
	}
	else
	{
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, (GLfloat) mode);
	}
}

void CBasicTexture::Set_Blending (GLint mode)
{
	bind();
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, (GLfloat) mode);
}

void CBasicTexture::unload()
{
	glDeleteTextures(1,&id);
}

