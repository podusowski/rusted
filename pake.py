#!/usr/bin/env python2
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
REVISION="7b192b9742ff9bfce86f0fc2cb8dfce3"
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
import pake.parser
import command_line


def parse_source_tree():
    for filename in fsutils.pake_files:
        pake.parser.parse(filename)

    configuration = configurations.get_selected_configuration()
    variables.export_special_variables(configuration)


def _build_some_targets_if_requested():
    if command_line.args.target:
        for target in command_line.args.target:
            targets.build(target)
        return True
    elif command_line.args.all:
        targets.build_all()
        return True


def main():
    parse_source_tree()

    configuration = configurations.get_selected_configuration()
    if configuration.name != "__default":
        ui.bigstep("configuration", str(configurations.get_selected_configuration()))

    if not _build_some_targets_if_requested():
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
