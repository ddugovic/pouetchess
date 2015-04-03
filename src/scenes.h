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


#ifndef __SCENES_H__
#define __SCENES_H__




enum Game_scene
{
	GAME_SCENE_INIT,
	GAME_SCENE_INTRO,
	GAME_SCENE_MAIN_MENU,
	GAME_SCENE_MAIN_GAME,
};


class Chess_Scene
{
	public:
		Chess_Scene();
		virtual ~Chess_Scene();
		virtual bool load()=0;
		virtual void unload()=0;
		virtual void update()=0;
	protected:
		bool already_loaded;
		void finish_loading();
	
};



#endif
