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
REVISION="1354b0b35e0e5907e052933613c4c6c2"
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
