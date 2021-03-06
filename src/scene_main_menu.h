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


#ifndef __E_SCENE_MAIN_MENU_H__
#define __E_SCENE_MAIN_MENU_H__


#include "scenes.h"
#include "texture.h"
#include "globalInfos.h"


class E_scene_main_menu: public Chess_Scene,GUIEventListener
{
	public:
		E_scene_main_menu();
		virtual ~E_scene_main_menu();
		virtual bool load();
		virtual void unload();
		virtual void update();
		
	private:
		CBasicTexture* texture_background;
		
		unsigned short menu_to_display;
		
		
		void Initialize_scene(GUI_scene scene_to_init);
		void display_main_menu();
		void display_options();
		void display_about();
		
		bool fade_in, fade_out;
		Game_scene scene_to_go;
				
				
		/**
		 * GUI event management
		 * @param evt the current event to manage
		 */
		virtual void actionPerformed(GUIEvent &evt);
};



#endif




