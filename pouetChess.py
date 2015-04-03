# Some code is Copyright (C) 2006  Tobi Vollebregt

import re, os, sys
import SCons.Util
from SCons.Variables import Variables


def exists(env):
	return True


def platform():
	if sys.platform == 'linux2' or sys.platform == 'linux-i386':
		print "Detected platform : linux"
		detected_platform='linux'
	elif sys.platform[:7] == 'freebsd':
		print "Detected platform : freebsd (%s)" % sys.platform
		print "WARNING: support is incomplete"
		detected_platform='freebsd'
	else:
		print "Platform not supported yet, please edit SConstruct file"
		detected_platform='unknown'
	return detected_platform


# Set gcc_3_4 to false for versions of gcc below 3.4.
def processor(gcc_3_4=True):
	print "Detecting processor..."

	try:
		f = open('/proc/cpuinfo', 'r')
	except IOError:
		import platform
		bits = platform.architecture()[0]
		if bits == '32bit':
			# default to i686 on 32 bit platforms without /proc/cpuinfo
			print "  couldn't detect; assuming Pentium Pro or better"
			archflags=['-march=i686']
		else:
			print "  couldn't detect"
			archflags=[]
		return archflags

	str = f.readline()
	family=""
	model=-1
	vendor=""
	archflags=[]
	while str:
		if str.startswith("vendor_id"):
			if str.find("GenuineTMx86") != -1:
				# Transmeta
				vendor="GenuineTMx86"
			elif str.find("GenuineIntel") != -1:
				# Intel
				vendor="GenuineIntel"
			elif str.find("AuthenticAMD") != -1:
				# AMD
				vendor="AuthenticAMD"
		elif str.startswith("cpu"):
			# first re does not do anything more than family = str.strip() in my case
			family = re.sub('^[^0-9A-Za-z]*', '', str).strip()
			# this is more useful, finds the first block of numbers
			family_number = re.findall('[0-9]+', str)[0]
		elif str.startswith("model") and str.find("name") == -1:
			model = re.sub('^[^0-9]*', '', str).strip()
		if vendor == "GenuineTMx86":
			if str.startswith("model name"):
				if str.find("Transmeta Efficeon") != -1:
					# Automatically reorders/realigns when
					# converting to VLIW, so no alignment
					# saves space without sacrificing speed
					print "  found Transmeta Efficeon"
					archflags=['-march=i686']
					if gcc_3_4: archflags+=['-mtune=pentium3']
					else: archflags+=['-mcpu=pentium3']
					archflags+=['-msse2', '-mfpmath=sse', '-falign-functions=0', '-falign-jumps=0', '-falign-loops=0']
		elif vendor == "GenuineIntel":
			if str.startswith("model name"):
				if str.find("Intel(R) Pentium(R) 4 CPU") != -1:
					print "  found Intel Pentium 4"
					archflags=['-march=pentium4']
				elif str.find("Coppermine") != -1:
					print "  found Intel Celeron (Coppermine)"
					archflags=['-march=pentium3']
				elif str.find("Pentium III") != -1:
					print "  found Intel Pentium III"
					archflags=['-march=pentium3']
				elif str.find("Pentium II") != -1:
					print "  found Intel Pentium II"
					archflags=['-march=pentium2']
				elif str.find("Intel(R) Celeron(R) CPU") != -1:
					print "  found Intel Celeron (Willamette)"
					archflags=['-march=pentium4']
				elif str.find("Celeron") != -1:
					print "  found Intel Celeron 1"
					archflags=['-march=pentium2']
				elif str.find("Intel(R) Pentium(R) M") != -1:
					print "  found Intel Pentium-M"
					if gcc_3_4: archflags=['-march=pentium-m']
					else:
						print "WARNING: gcc versions below 3.4 don't support -march=pentium-m, using -march=pentium4 instead"
						archflags=['-march=pentium4']
				elif str.find("Intel(R) Xeon(TM) CPU") != -1:
					print "  found Intel Xeon w/EM64T"
					archflags=['-march=nocona', '-mmmx', '-msse3']
		elif vendor == "AuthenticAMD":
			if str.startswith("model name"):
				if str.find("Duron") != -1:
					if model == 7:
						print "  found AMD Duron Morgan"
						archflags=['-march=athlon-xp']
					elif model == 3:
						print "  found AMD Mobile Duron"
						archflags=['-march=athlon-tbird']
					else:
						print "  found AMD Duron"
						archflags=['-march=athlon-tbird']
				elif str.find("Sempron") != -1:
					print "  found AMD Sempron 64"
					if gcc_3_4: archflags=['-march=athlon64']
					else:
						print "WARNING: gcc versions below 3.4 don't support -march=athlon64, using -march=athlon-4 -msse2 -mfpmath=sse instead"
						archflags=['-march=athlon', '-msse2', '-mfpmath=sse']
				elif str.find("Athlon") != -1:
					if str.find("64") != -1:
						print "  found AMD Athlon 64"
						if gcc_3_4: archflags=['-march=athlon64']
						else:
							print "WARNING: gcc versions below 3.4 don't support -march=athlon64, using -march=athlon-4 -msse2 -mfpmath=sse instead"
							archflags=['-march=athlon', '-msse2', '-mfpmath=sse']
					# maybe this should use int(family_number) rather than family
					# i have no way of checking
					elif family == 6 and model == 8:
						print "  found AMD Athlon Thunderbird XP"
						archflags=['-march=athlon-xp']
					else:
						print "  found AMD Athlon"
						archflags=['-march=athlon']
				elif str.find("Opteron") != -1:
					print "  found AMD Opteron"
					if gcc_3_4: archflags=['-march=opteron']
					else:
						print "WARNING: gcc versions below 3.4 don't support -march=opteron, using -march=athlon-4 -msse2 -mfpmath=sse instead"
						archflags=['-march=athlon-4', '-msse2', '-mfpmath=sse']
		else:
			if str.find("Nehemiah") != -1:
				print "  found VIA Nehemiah"
				archflags=['-march=c3-2']
			elif str.find("Eden") != -1:
				print "  found VIA Eden C3"
				archflags=['-march=c3']
			elif str.find("Ezra") != -1:
				print "  found VIA Ezra"
				archflags=['-march=c3']

		str = f.readline()
	if len(archflags) == 0:
		if vendor == "":
			if family != -1: # has to be checked...
				if family_number in ('970'):
					print "  found PowerPC 970 (G5)"
					archflags=['-mtune=G5', '-maltivec', '-mabi=altivec']
				#i think these are all the same as far as gcc cares
				elif family_number in ('7450','7455','7445','7447','7457'):
					print "  found PowerPC 7450 (G4 v2)"
					archflags=['-mtune=7450', '-maltivec', '-mabi=altivec']
				elif family_number in ('7400','7410'):
					print "  found PowerPC 7400 (G4)"
					archflags=['-mtune=7400', '-maltivec', '-mabi=altivec']
				elif family_number in ('750'):
					print "  found PowerPC 750 (G3)"
					archflags=['-mtune=750']
				elif family_number in ('740'):
					print "  found PowerPC 740 (G3)"
					archflags=['-mtune=740']
				elif family_number in ('604'):
					print "  found PowerPC 604"
					archflags=['-mtune=604']
				elif family_number in ('603'):
					print "  found PowerPC 603"
					archflags=['-mtune=603']



	f.close()
	if len(archflags) == 0:
		import platform
		bits = platform.architecture()[0]
		if bits == '32bit':
			print "Unrecognized or couldn't detect CPU, guessing i686 compatible.."
			archflags=['-march=i686']
		else:
			print "Unrecognized or couldn't detect CPU, guessing generic.."
			archflags=['-mtune=generic']
	return archflags




def check_gcc_version(env, conf = None):
	# The GCC version is needed by the detect.processor() function to recognize
	# versions below 3.4 and versions equal to or greater than 3.4.
	# As opposed to the other functions in this file, which are called from the
	# configure() function below, this one is called directly from rts.py while
	# parsing `optimize=' configure argument.
	print env.subst("Checking $CC version..."),
	try:
		f = os.popen(env.subst('$CC --version'))
		version = f.read()
	finally:
		f.close()
	if version:
		version = re.search('[0-9]\.[0-9]\.[0-9]', version).group()
		print version
		return re.split('\.', version)
	else:
		print env.subst("$CC not found")
		env.Exit(1)




def check_sdl(env, conf):
	print "Checking for SDL..."
	print "  Checking for sdl-config...",
	sdlcfg = env.WhereIs("sdl-config")
	if not sdlcfg: # for FreeBSD
		sdlcfg = env.WhereIs("sdl11-config")
	if sdlcfg:
		print sdlcfg
		print "  Checking for LibSDL >= 1.2.8...",
		sdlobj = os.popen(sdlcfg+" --version")
		sdlver = sdlobj.read()
		sdlerr = sdlobj.close()
		print sdlver,
		if sdlver.split('.') >= [1,2,8]:
			env.ParseConfig(sdlcfg+" --cflags --libs")
		else:
			print "You need LibSDL version 1.2.8 or greater for this program"
			env.Exit(1)
	else:
		print "not found"
		if env['platform'] == 'freebsd':
			guess_include_path(env, conf, 'SDL', 'SDL11')
		else:
			guess_include_path(env, conf, 'SDL', 'SDL')








def config_h_build(target, source, env):

	config_h_defines = {
		# this is where you put all of your custom configuration values
		"install_prefix": env.subst(env['datadir'])
	}


	config_h = file(str(target), "w")
	config_h_in = file(str(source), "r")
	config_h.write(config_h_in.read() % config_h_defines)
	config_h_in.close()
	config_h.close()






def generate(env):
	
	
	usrcachefile = 'usropts.py'
	intcachefile = 'intopts.py'
	
	usropts = Variables(usrcachefile)
	intopts = Variables(intcachefile)
	
	#user visible options
	usropts.AddVariables(
		('platform',          'Set to linux or freebsd', None),
		('debug',             'Set to yes to produce a binary with debug information', 0),
		('optimize',          'Enable processor optimizations during compilation', 1),
		('profile',           'Set to yes to produce a binary with profiling information', False),
		('prefix',            'Install prefix', '/usr/local'),
		('datadir',           'Data directory', '$prefix/games/pouetChess'),
		('noinstall',         'You can run pouetChess directly from the bin directory. Useful for developers.', False),
		('strip',             'Discard symbols from the executable (only when neither debugging nor profiling)', True))
		
	#internal options
	intopts.AddVariables(
		('LINKFLAGS',     'linker flags'),
		('LIBPATH',       'library path'),
		('LIBS',          'libraries'),
		('CCFLAGS',       'c compiler flags'),
		('CXXFLAGS',      'c++ compiler flags'),
		('CPPDEFINES',    'c preprocessor defines'),
		('CPPPATH',       'c preprocessor include path'),
		('is_configured', 'configuration version stamp'))
		
	usropts.Update(env)
	intopts.Update(env)
	
	env.Help(usropts.GenerateHelpText(env))
	
	# Use this to avoid an error message 'how to make target configure ?'
	env.Alias('configure', None)
	
	if not 'configure' in sys.argv and not ((env.has_key('is_configured') and env['is_configured'] == 1) or env.GetOption('clean')):
		print "Not configured or configure script updated.  Run `scons configure' first."
		print "Use `scons --help' to show available configure options to `scons configure'."
		env.Exit(1)
		
		
	if 'configure' in sys.argv:
		
		
		# Important : unset existing variables
		for key in ['platform', 'debug', 'optimize', 'profile', 'prefix', 'datadir', 'noinstall', 'cachedir', 'strip', 'LINKFLAGS', 'LIBPATH', 'LIBS', 'CCFLAGS', 'CXXFLAGS', 'CPPDEFINES', 'CPPPATH', 'is_configured']:
			if env.has_key(key): env.__delitem__(key)
		
		print "\nNow configuring.  If something fails, consult `config.log' for details.\n"
		
		#parse cmdline
		def makeHashTable(args):
			table = { }
			for arg in args:
				if len(arg) > 1:
					lst = arg.split('=')
					if len(lst) < 2: continue
					key = lst[0]
					value = lst[1]
					if len(key) > 0 and len(value) > 0: table[key] = value
			return table

		args = makeHashTable(sys.argv)
		
		
			
		env['is_configured'] = 1
		
		if args.has_key('platform'): env['platform'] = args['platform']
		else: env['platform'] = platform()
	
		gcc_version = check_gcc_version(env)
		
		
		
		# Declare some helper functions for boolean and string options.
		def bool_opt(key, default):
			if args.has_key(key):
				if args[key] == 'no' or args[key] == 'false' or args[key] == '0':
					env[key] = False
				elif args[key] == 'yes' or args[key] == 'true' or args[key] == '1':
					env[key] = True
				else:
					print "\ninvalid", key, "option, must be one of: yes, true, no, false, 0, 1."
					env.Exit(1)
			else: env[key] = default

		def string_opt(key, default):
			if args.has_key(key):
				env[key] = args[key]
			else: env[key] = default		
		
		
	
		# start with empty FLAGS, in case everything is disabled.
		env['CCFLAGS'] = []
		
		# profile?
		bool_opt('profile', False)
		if env['profile']:
			print "profiling enabled,",
			env.AppendUnique(CCFLAGS=['-pg'], LINKFLAGS=['-pg'])
		else:
			print "profiling NOT enabled,",
		
		# debug?
		if args.has_key('debug'):
			level = args['debug']
			if level == 'no' or level == 'false': level = '0'
			elif level == 'yes' or level == 'true': level = '3'
		else:
			level = '0'
		if int(level) == 0:
			print "debugging NOT enabled,",
			env['debug'] = 0
		elif int(level) >= 1 and int(level) <= 3:
			print "level", level, "debugging enabled,"
			env['debug'] = level
			env.AppendUnique(CCFLAGS=['-ggdb'+level], CPPDEFINES=['DEBUG', '_DEBUG'])
		else:
			print "\ninvalid debug option, must be one of: yes, true, no, false, 0, 1, 2, 3."
			env.Exit(1)
		
		# optimize?
		if args.has_key('optimize'):
			level = args['optimize']
			if level == 'no' or level == 'false': level = '0'
			elif level == 'yes' or level == 'true': level = '1'
		else:
			if env['debug']: level = '0'
			else: level = '1'
		if level == 'noarch':
		 	print "noarch optimizing"
			env['optimize'] = 'noarch'
			env.AppendUnique(CCFLAGS=['-O1', '-pipe'])	
		elif int(level) >= 1 and int(level) <= 3:
			print "level", level, "optimizing enabled"
			env['optimize'] = level
			archflags = processor(gcc_version >= ['3','4','0'])
			env.AppendUnique(CCFLAGS=['-O'+level, '-pipe']+archflags)
		elif int(level) == 0:
			print "optimizing NOT enabled,"
			env['optimize'] = 0
		else:
			print "\ninvalid optimize option, must be one of: yes, true, no, false, 0, 1, 2, 3, noarch."
			env.Exit(1)



		bool_opt('strip', True)
		bool_opt('noinstall', False)
		string_opt('prefix', '/usr/local')
		string_opt('datadir', '$prefix/games/pouetChess')
		string_opt('cachedir', None)
		

		conf = env.Configure()
		check_sdl(env, conf)
		
		## Check for SDL
		if not conf.CheckCHeader('SDL.h') and not conf.CheckCHeader('SDL/SDL.h') and not conf.CheckCHeader('SDL11/SDL.h'):
			print 'LibSDL headers are required for this program'
			env.Exit(1)
			
		if not conf.CheckLib('SDL') and not conf.CheckLib('SDL-1.1'):
			print 'LibSDL is required for this program'
			env.Exit(1)
		
		
		## Check for SDL_image
		if not conf.CheckCHeader('SDL_image.h') and not conf.CheckCHeader('SDL/SDL_image.h') and not conf.CheckCHeader('SDL11/SDL_image.h'):
			print "SDL_image is required for this program"
			env.Exit(1)
			
		if not conf.CheckLib('SDL_image'):
			print 'LibSDL_image is required for this program'
			env.Exit(1)
			
			
		## Check for OpenGL	
		if not conf.CheckLib('GL'):
			print "You need an OpenGL development library for this program"
			env.Exit(1)
		if not conf.CheckLib('GLU'):
			print "You need the OpenGL Utility (GLU) library for this program"
			env.Exit(1)
			

		env = conf.Finish()
		
		if env['noinstall']:
			env['datadir']="../data"
			print "========================================================="
			print "pouetChess will not be installed (option noinstall)"
		else:
			print "========================================================="
			print "Program will be installed in : %s" % env['prefix']
			print "Data will be intalled in : %s" % env.subst(env['datadir'])
		
		print "========================================================="
		print "\nEverything seems OK.  Run `scons' now to build.\n"
		print "========================================================="
		
		
		# fall back to environment variables if neither debug nor optimize options are present
		if not args.has_key('debug') and not args.has_key('optimize'):
			if os.environ.has_key('CFLAGS'):
				print "using CFLAGS:", os.environ['CFLAGS']
				env['CCFLAGS'] = SCons.Util.CLVar(os.environ['CFLAGS'])
			if os.environ.has_key('CXXFLAGS'):
				print "using CXXFLAGS:", os.environ['CXXFLAGS']
				env['CXXFLAGS'] = SCons.Util.CLVar(os.environ['CXXFLAGS'])
			else:
				env['CXXFLAGS'] = env['CCFLAGS']
		
		usropts.Save(usrcachefile, env)
		intopts.Save(intcachefile, env)
		
		config_h_build("src/config.h","config.h.in",env)

	#Should we strip the exe?
	if env.has_key('strip') and env['strip'] and not env['debug'] and not env['profile'] and not env.GetOption('clean'):
		env['strip'] = True
	else:
		env['strip'] = False


	

