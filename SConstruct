###########################################################################
#                                                                         #
#   Copyright (C) 2006 by Frederic MARTIN                                 #
#   martin-frederic@users.sourceforge.net                                 #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
#   This program is distributed in the hope that it will be useful,       #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#   GNU General Public License for more details.                          #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with this program; if not, write to the                         #
#   Free Software Foundation, Inc.,                                       #
#   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
#                                                                         #
###########################################################################



import os,glob



# create build environment

env = Environment(tools = ['default', 'pouetChess'], toolpath = ['.'])
env.AppendUnique(CXXFLAGS = ['-Wall'])

# gather a list of source files
SOURCES =  glob.glob('src/*.cpp')
SOURCES += glob.glob('src/sxmlgui/*.cpp')
SOURCES += glob.glob('src/faile/*.c')

# build target
pouetChess = env.Program(target = 'bin/pouetChess', source = SOURCES)

# Strip the executable
if env['strip']:
	env.AddPostAction(pouetChess, Action([['strip','$TARGET']]))
	


if not env['noinstall']:

	# Install the program
	inst=Install(os.path.join(env['prefix'], 'bin'), pouetChess)
	Alias('install', inst)
	
		
	##  Install the data  ##
	
	# Install the opering book for FAILE
	pc_data =  ['data/faile.obk']
	inst = Install(os.path.join(env.subst(env['datadir'])),pc_data)
	Alias('install', inst)
	
	# Install the 3D Models
	pc_data = glob.glob('data/3DModels/*.lwo')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'3DModels'),f)
		Alias('install', inst)
	
	
	# Install the GUI xml description files
	pc_data = glob.glob('data/gui/*.xml')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'gui'),f)
		Alias('install', inst)
		
	pc_data = glob.glob('data/gui/english/*.xml')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'gui/english'),f)
		Alias('install', inst)
		
	pc_data = glob.glob('data/gui/french/*.xml')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'gui/french'),f)
		Alias('install', inst)
	
	
	# Install the icons
	pc_data = glob.glob('data/icons/*.bmp')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'icons'),f)
		Alias('install', inst)
		
	pc_data = glob.glob('data/icons/*.png')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'icons'),f)
		Alias('install', inst)	
		
		
	# Install the textures
	pc_data = glob.glob('data/textures/*.jpg')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'textures'),f)
		Alias('install', inst)
	
	pc_data = glob.glob('data/textures/*.png')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'textures'),f)
		Alias('install', inst)
	
	
	pc_data = glob.glob('data/textures/skybox/*.jpg')
	for f in pc_data:
		# Use subst() to substitute $prefix in datadir.
		inst = Install(os.path.join(env.subst(env['datadir']),'textures/skybox'),f)
		Alias('install', inst)
	
	


