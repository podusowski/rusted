#!/usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2005-2010; Arne Babenhauserheide

"""Waffle iron - creates waffles :)

- http://draketo.de/proj/waffles/

TODO: Differenciate sourcetree and packages: sourcetree as dir in wafdir, packages in subdir packages. 
"""

__license__ = """
This license only applies to the waffle part of the code,
which ends with
### Waffle Finished ###

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
"""

import os, sys, optparse, getpass, re, binascii
if sys.hexversion<0x204000f: raise ImportError("Waffle requires Python >= 2.4")
try:
	from hashlib import md5
except:
	from md5 import md5 # for python < 2.5

if 'PSYCOWAF' in os.environ:
	try:import psyco;psyco.full()
	except:pass

VERSION="0.1"
REVISION="6377b018e61adefaabfb0fc7f14eeeb1"
INSTALL=''
cwd = os.getcwd()
join = os.path.join
HOME = os.path.expanduser('~')

WAF='waffle' #: the default name of the executable
WAFFLE='waffle' #: the default name of the dir with the sources (prepended with s when unpacked)
WAFFLE_MAKER='waffle_maker.py'

def parse_cmdline_args():
	"""@return: opts, args; opts are parsed"""
	# parse commandline arguments.
	parser = optparse.OptionParser()
	parser.add_option("-o", "--filename", 
			  help="Set the output filename", default="waffle.py", metavar="OUTPUT_FILE")
	parser.add_option("-p", "--package", action="append", dest="packages",
		  help="Package folder to include (can be used multiple times)", metavar="PACKAGE_FOLDER")
	parser.add_option("-m", "--module", action="append", dest="modules", 
			  help="Python module to include (can be used multiple times)", metavar="module_to_include.py")
	parser.add_option("-s", "--script", 
			  help="Execute this script", default="run.py", metavar="script_to_run.py")
	parser.add_option("--unpack-only", action="store_true", 
			  help="only unpack the tar.bz2 data, but don't execute anything.", default=False)
	opts, args = parser.parse_args()
	if opts.modules is None:
		opts.modules = []
	if opts.packages is None:
		opts.packages = []
			      
	return opts, args

def b(x):
	return x

if sys.hexversion>0x300000f:
	WAF='waffle3'
	def b(x):
		return x.encode()

def err(m):
	print(('\033[91mError: %s\033[0m' % m))
	sys.exit(1)

def get_waffle_data():
	f = open(sys.argv[0],'r')
	c = "corrupted waf (%d)"
	while True:
		line = f.readline()
		if not line: err("no data")
		if line.startswith('#==>'):
			txt = f.readline()
			if not txt: err("wrong data: data-line missing")
			if not f.readline().startswith('#<=='): err("wrong data: closing line missing")
			return txt

def unpack_wafdir(txt, zip_type="bz2"):
	"""@param txt: The compressed data"""
	if not txt: err(c % 3)
	if sys.hexversion>0x300000f:
		txt = binascii.a2b_base64(eval("b'" + txt[1:-1] + r"\n'"))
	else: 
		txt = binascii.a2b_base64(txt[1:])

	# select the target folder
	import shutil, tarfile

	s = '.%s-%s-%s'
	if sys.platform == 'win32': s = s[1:]

	## Firstoff we we select some possible folders, to be tested one after the other (with the appropriate precautions).
	## For the sake of readability we first note the different options here.
	#: The home folder as the best option (if the user has a writeable home)
	dirhome = join(HOME, s % (WAF, VERSION, REVISION))
	# the scripts dir
	name = sys.argv[0]
	base = os.path.dirname(os.path.abspath(name))
	#: As second option use the folder where the script resides (if writeable by us - and not yet taken, which could be, if another user started the script). 
	dirbase = join(base, s % (WAF, VERSION, REVISION), getpass.getuser())
	#: tmp as last resort
	dirtmp = join("/tmp", getpass.getuser(), "%s-%s-%s" % (WAF, VERSION, REVISION))

	def prepare_dir(d):
		"""create the needed folder"""
		os.makedirs(join(d, WAFFLE))

	def check_base(d):
		"""Check the dir in which the script resides.

		Only use the dir, if it belongs to us. If we can’t trust the scripts dir, we’re fragged anyway (someone could just tamper directly with the script itself - or rather: could compromise anything we run)."""
		prepare_dir(d)
		return d

	def check_tmp(d):
		"""Check the tmp dir - always remove the dir before startup.

		This kills the caching advantage, but is necessary for security reasons (else someone could create a compromised dir in tmp and chmod it to us)."""
		# last resort: tmp
		if os.path.exists(d):
			try: shutil.rmtree(d)
			except OSError: err("Can't remove the previously existing version in /tmp - executing would endanger your system")
			try: 
				prepare_dir(d)
				return d
			except OSError: err("Cannot unpack waf lib into %s\nMove waf into a writeable directory" % dir)

	## Now check them. 
	# first check: home
	try:
		d = dirhome
		prepare_dir(d)
	except OSError:
		# second check: base
		if base.startswith(HOME) or sys.platform == 'win32':
			try:
				d = check_base(dirbase)
			except OSError:
				d = check_tmp(dirtmp)
		else: d = check_tmp(dirtmp)

	## Now unpack the tar.bz2 stream into the chosen dir. 
	os.chdir(d)
	if zip_type == 'bz2': 
		tmp = 't.tbz2'
	elif zip_type == 'gz':
		tmp = 't.gz'
	t = open(tmp,'wb')
	t.write(txt)
	t.close()

	try:
		t = tarfile.open(tmp)
	# watch out for python versions without bzip2
	except:
		try: 
			os.system('bunzip2 t.bz2')
			t = tarfile.open('t')
		except:
			# if it doesn’t work, go back and remove the garbage we created.
			try: 
				os.unlink(tmp)
			except OSError: pass
			os.chdir(cwd)
			try: shutil.rmtree(d)
			except OSError: pass
			err("Waf cannot be unpacked, check that bzip2 support is present")

	for x in t:
		t.extract(x)
	t.close()
	os.unlink(tmp)


	#if sys.hexversion>0x300000f:
		#sys.path = [join(d, WAFFLE)] + sys.path
		#import py3kfixes
		#py3kfixes.fixdir(d)

	os.chdir(cwd)
	return join(d, WAFFLE)


def make_waffle(base_script="waffle_maker.py", packages=[], modules=[], folder=WAFFLE, executable="run.py", target="waffle.py", zip_type="bz2"):
	"""Create a waf-like waffle from the base_script (make_waffle.py), the folder and a python executable (appended to the end of the waf-light part)."""
	print("-> preparing waffle")
	mw = 'tmp-waf-'+VERSION

	import tarfile, re, shutil

	if zip_type not in ['bz2', 'gz']:
		zip_type = 'bz2'

	# copy all modules and packages into the build folder
	if not os.path.isdir(folder):
		os.makedirs(folder)
	
	for i in modules + packages:
		if i.endswith(os.path.sep): 
			i = i[:-1]
		if os.path.isdir(i) and not os.path.isdir(join(folder, i.split(os.path.sep)[-1])):
			shutil.copytree(i, join(folder, i.split(os.path.sep)[-1]))
		elif os.path.isfile(i): 
			shutil.copy(i, folder)

	#open a file as tar.[extension] for writing
	tar = tarfile.open('%s.tar.%s' % (mw, zip_type), "w:%s" % zip_type)
	tarFiles=[]

	def all_files_in(folder): 
		"""Get all paths of files inside the folder."""
		filepaths = []
		walked = [i for i in os.walk(folder)]
		for base, dirs, files in walked:
		    filepaths.extend([os.path.join(base, f) for f in files])
		return filepaths
		
	files = [f for f in all_files_in(folder) if not f.endswith(".pyc") and not f.endswith(".pyo") and not "/." in f]

	for x in files:
		tar.add(x)
	tar.close()

	# first get the basic script which sets up the path
	f = open(base_script, 'r')
	code1 = f.read()
	f.close()
	# make sure it doesn't do anything.
	code1.replace("__name__ == '__main__':", "__name__ == '__main__' and False:")
	# then append the code from the executable 
	if executable is not None:
		f = open(executable, 'r')
		code1 += f.read()
		f.close()

	# now store the revision unique number in waf
	#compute_revision()
	#reg = re.compile('^REVISION=(.*)', re.M)
	#code1 = reg.sub(r'REVISION="%s"' % REVISION, code1)

	prefix = ''
	#if Build.bld:
	#	prefix = Build.bld.env['PREFIX'] or ''

	reg = re.compile('^INSTALL=(.*)', re.M)
	code1 = reg.sub(r'INSTALL=%r' % prefix, code1)
	#change the tarfile extension in the waf script
	reg = re.compile('bz2', re.M)
	code1 = reg.sub(zip_type, code1)

	f = open('%s.tar.%s' % (mw, zip_type), 'rb')
	cnt = f.read()
	f.close()

	# the REVISION value is the md5 sum of the binary blob (facilitate audits)
	m = md5()
	m.update(cnt)
	REVISION = m.hexdigest()
	reg = re.compile('^REVISION=(.*)', re.M)
	code1 = reg.sub(r'REVISION="%s"' % REVISION, code1)
	f = open(target, 'w')
	f.write(code1)
	f.write('#==>\n')
	data = str(binascii.b2a_base64(cnt))
	if sys.hexversion>0x300000f:
		data = data[2:-3] + '\n'
	f.write("#"+data)
	f.write('#<==\n')
	f.close()

	# on windows we want a bat file for starting.
	if sys.platform == 'win32':
		f = open(target + '.bat', 'wb')
		f.write('@python -x %~dp0'+target+' %* & exit /b\n')
		f.close()

	# Now make the script executable
	if sys.platform != 'win32':
		# octal prefix changed in 3.x from 0xxx to 0oxxx. 
		if sys.hexversion>0x300000f:
			os.chmod(target, eval("0o755"))
		else:
			os.chmod(target, eval("0755"))

	# and get rid of the temporary files
	os.unlink('%s.tar.%s' % (mw, zip_type))
	shutil.rmtree(WAFFLE)
	

def test(d):
	try: 
	      os.stat(d)
	      return os.path.abspath(d)
	except OSError: pass

def find_lib():
	"""Find the folder with the modules and packages.

	@return: path to to folder."""
	name = sys.argv[0]
	base = os.path.dirname(os.path.abspath(name))

	#devs use $WAFDIR
	w=test(os.environ.get('WAFDIR', ''))
	if w: return w

	#waffle_maker.py is executed in place.
	if name.endswith(WAFFLE_MAKER):
		w = test(join(base, WAFFLE))
		# if we don’t yet have a waffle dir, just create it.
		if not w:
			os.makedirs(join(base, WAFFLE))
			w = test(join(base, WAFFLE))
		if w: return w
		err("waffle.py requires " + WAFFLE + " -> export WAFDIR=/folder")

	d = "/lib/%s-%s-%s/" % (WAF, VERSION, REVISION)
	for i in [INSTALL,'/usr','/usr/local','/opt']:
		w = test(i+d)
		if w: return w

	# first check if we can use HOME/s,
	# if not, check for s (allowed?)
	# then for /tmp/s (delete it, if it already exists,
	# else it could be used to smuggle in malicious code)
	# and finally give up. 
	
	#waf-local
	s = '.%s-%s-%s'
	if sys.platform == 'win32': s = s[1:]
	# in home
	d = join(HOME, s % (WAF, VERSION, REVISION), WAFFLE)
	w = test(d)
	if w: return w

	# in base
	if base.startswith(HOME):
		d = join(base, s % (WAF, VERSION, REVISION), WAFFLE)
		w = test(d)
		if w: return w
	# if we get here, we didn't find it.
	return None


wafdir = find_lib()
if wafdir is None: # no existing found
	txt = get_waffle_data() # from this file
	if txt is None and __name__ == "__main__": # no waffle data in file
		opts, args = parse_cmdline_args()
		make_waffle(packages=opts.packages, modules=opts.modules, executable=opts.script)
	else: 
		wafdir = unpack_wafdir(txt)
	
elif sys.argv[0].endswith(WAFFLE_MAKER) and __name__ == "__main__": # the build script called
	opts, args = parse_cmdline_args()
	if opts.filename.endswith(WAFFLE_MAKER):
		err("Creating a script whose name ends with " + WAFFLE_MAKER + " would confuse the build script. If you really want to name your script *" + WAFFLE_MAKER + " you need to adapt the WAFFLE_MAKER constant in " + WAFFLE_MAKER + " and rename " + WAFFLE_MAKER + " to that name.")
	make_waffle(packages=opts.packages, modules=opts.modules, executable=opts.script, target=opts.filename)
	# since we’re running the waffle_maker, we can stop here. 
	exit(0)

if wafdir is not None: 
	sys.path = [wafdir] + [join(wafdir, d) for d in os.listdir(wafdir)] + sys.path


## If called with --unpack-only, no further code is executed.
if "--unpack-only" in sys.argv:
	print(sys.argv[0], "unpacked to", wafdir)
	exit(0)

### Waffle Finished ###

#!/usr/bin/env python

import fsutils
import ui
import targets
import variables
import configurations
import parser

def parse_source_tree():
    for filename in fsutils.pake_files:
        parser.parse(filename)

    configuration = configurations.get_selected_configuration()
    variables.export_special_variables(configuration)

def main():
    import command_line

    parse_source_tree()

    configuration = configurations.get_selected_configuration()
    if configuration.name != "__default":
        ui.bigstep("configuration", str(configurations.get_selected_configuration()))

    if command_line.args.target:
        for target in command_line.args.target:
            targets.build(target)
    elif command_line.args.all:
        targets.build_all()
    else:
        ui.info("no target selected\n")

        ui.info(ui.BOLD + "targets:" + ui.RESET)
        for target in targets.targets.values():
            ui.info("  " + str(target))

        ui.info(ui.BOLD + "\nconfigurations:" + ui.RESET)
        for configuration in configurations.configurations:
            ui.info("  " + str(configuration))

        ui.info("\nsee --help for more\n")

if __name__ == '__main__':
    main()

#==>
#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
#<==
