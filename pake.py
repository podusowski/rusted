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
REVISION="df6790b2f7db304906bf1b8c3c65df4a"
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
#QlpoOTFBWSZTWe27vDMAVbz/3dz271X9////////7v////oACA0AEAAAAQBAAUhgMR6+33l6AaCnnvc8yp7nbmoShkwcTAAAAEtG7ne7nXb0N7OPPDx2ztrq5dlC9Hj26942jnebyvSTs4HUMDvN63u7d7uIk7WXu5Kp5spHbOmoAdtJ3uo647D3hjsy17xoLTe9s95ynNPb3bu7um6VXbcEiQgAgJkJgJHlNo0kzU9CHqDTTRoMgNDT1ABoaDQNGglMhCCTJqamIGpqPGVPKPUbUGmg09QNAAAA9QNABoAA4AAAAAAADIAAAAAAAAAAyAAk0kiBATTQmmg1T9UzTFNpDQA00aGgHqGhoNNADygGgBhEkgpqe1MmjIqf5Gqm9U9qn6ptTPSn6p6h6mjJpp6h6j0m1ABo9QANqaHqANAIkhAQmmQTSaYp4Uwpo9I2k9R6jQAGgABoNAAAAD/3nAZ6Ae8DnLYqVUkkZCA1VYKoPZ6vj8fda6eOlKc6LIReCaQVpVtFo/HFk7XS6qWmolnMWxbQHdTcIRJISEghAYCA8IhSCWUCkRUkEUf1+on7pxQ9+enro4P0VP6WL/hvZ/Y6+5RYG6ovcIPtdM5tebOfDTw0/rsYopBslyKvCz55awgEbJA2X/IMLRvh7KNkvtOI2FhRIoK/HcVbg/WplfpMNLOXEN6x0mJNXpmCKdb/UyHmdn+5CHuIeVnEYgoxZFuwl9RoOG7htlm6NsmwJMbLmWs4XUxjPvNE60KxeTb1ZA5Onjtqcemc++9nEXyHnYePsp6/Xw9b5tinseYj5GhRvRMZiDJGSsJFAqHYlSTr1rJTs2yJMStGJaWHz2FFQML87cvOwyImUu+WYILaXs7sDBU1bzqyzO/JRMTU4dNo4Th5dTNXwSVfl8c5jOVs5tjxyq4xtooLbUVbaRURG1BC27MwWZKUYtSsqsKsrplEUMkVlWUlQV2TJK9DfCq/Gyo00rtP6lk1ScUqiodMYg7kuBVhamnnq6K2IoyylqVZVusBds3H+/Usm+9oZSpwtgPuulUT3d2HbrRusOmYmbuONCqdWZj9eOYltKwUmzYRgGUs/P0fYm2zsZEuauEXLZZbFAFRrRdFgU6UonI2BQWbn3PDrK+FBhGdIDUteYcVCEj4a8elE5eeomaUFsCp3DDBeNKiRNhL5aUSG6pvOWmSlMMMMwuQnrZsfgJ50JPFCQ39ndgJpJD4I3b8/Te8kykqnCxylPxqo/JBAIDCAxUBQyBiKoLbWAIvKAipIAoyICyKkgIFhbJOOI6SRQeGOh21P6q9w5xkTlYXdjOiwlho14SsYWMB88HVOvqdM65g7DNr5rxk0yXdAUcPmoYIcGcnZIYJRRCpq3Brrve1kzYGSJa9MwQ1M3+Yiel1ntqNjzdEdpD6VMCP2ks4J/shtBaiD0U/tIMogtRAlk9h4+MLfCPXZZeSrj0tByAvdpsCPeIEFzPtsVbCVTb7xlt0bD0teq2TW04P2VG66lJ0ZS7G57mdvgnLCZj0jH57VU1Xy6CfRS4iUYMYosirme8jLFCj8EY8OMOD1KfYqUKx2sKye0Ph61jiFXrWAEWEzZECKsQg38NlUzo1VUmjMzMn23aI3xDEgwgamWzsg+rfiiJombzc6o2j5BnlwUZMqCEiEijpFBwe1JhPl0QabFfU2+IPY0SRnTi6JmhiBAxIFXUUmfFw1iA6+G5Fd4VBQiqcJDbJDGatmkWVYbl15oSFGEN9r7aafLJkJSRpvtxGTBr44t4t3Bn26Na19vHJGm8wMsYWl7Kqp1aoHO/hgp2x3YJC+0m2EI1CJS7Kalj0QEiO8+L6YVx0V6ebGRdb5x6LsoRw7lqJDMrKZRB7Dti7VwD16ZDMGlhzq+Ng2eIE2gbK60OCAGmjMkSJGJbfLgZjK/lptpUXabkW7E+XU5zfTB2oJhUu3BLJOJOZsvGKTDmOMRFUohg3wu2ywxK3oiPitxsitQjfdO4MoUVbYz8FBS3baWozXIHp9GB+Cl+7SONEImbSCm1nLxXTt9O7pt5tl4VE+9rbbE87pAPkN10qXWKQ6t1eLOZrFjVLwRvQEnhOilN90QAqnzZ5jeM78q1yvChCV2BLOS4KYmvfUb5zr2sxigWCTdUuu8copGOmskXHMxJFoLtx48AY2g8XqrsLu1BfTMTw5yKmqd2W7gKm7MJQnu1CwLUYkVkSk0x38sjSSbQTkYb5DGYz5QYukfHPjPdyThdft7Vu8cbOoKfO1c1icLM4zmUOU4A16HpXAIOooN0bpCs2qqjYzFCFEuMXgmy6pffIxflqMya5f1PrcE7L4u+iKCtnEcwochwPJcz4Z2unlZqYOU8a33DCF2rnwGzmzG2tcpnlDJqupU9xaphB0m4GecavdEpsdKDH+lDcnmnDbbVgnJ4j3rkXlCg2OneQlUeMTtUbh6e/YxAVoxL2Um9+9ffRz4btyvBTPTjym84YKN5K4PFXjAi018Pq6QeyBLbm+9zF3v6KGWeSLzn2y2eVBQTH9xr2GSawYv08cHiaqaKNbmg9LwE5TGWv4n5NWuSY/X1kivlKfEw+y2yqocdXVbr8Po8eU9duy7k5+QxlrrnZo9GUnw1llr64twMr+DT+7VeWcRXdHqats835x/m8Ojun4evs0GBzs074jsEZ2wLCcDaHrYJtCqN1Oq/g49EpJ+DLCWh2xgqzy3EUT2+/edgk7VjWgixYgKMRVJQtkVUR+3cYYKsUGKMpZT+C5ExRcv23GK/QPg+W5f+fhw9CNK/HkMD2cnDPpbz6g2222+FufD+vzeXCgg3b3uPD3qZh3c/q+vo+f/bDqrMi83g9j8j7uKyr1oWNWvpgE02iZVBIMVs25+ku0agnZ9PRMpd/WazstoMfM4ftuNlRTkUVszqgxW2CgygvGCZFmRjPBB6lJo1HX69vwFXyfK+NHZamhRK6dv9eK4jWpbEdCNi1ZoUW8OBOJZBynImVBPKIpVTTuJUamPrQfEsxti5yPcqJUVcQyne/HTWMYzkRRWnjbwQVSbR6s7eHHMrjdW0z4ECUEzeXKiTdPiMBgrl4moRgWGZKuEkYhPBMEveJeLL1deajRVLKKFEzAm0hoEs0NZqVcu7pDjWFcQFTiYqcelZs0Lfx8++2ExXoJzAkFcqefJ9XNFt4ZzOsS981GKwPIkhS/Yw46XHqlZQlUItFeaS6KFL5rMjNrWIDHhblqHFZirqbDphzilbJJO942yajpW+2yQmE7pOopoekZPCxHTEtxVjpTndStqVtRBgRCEROywdzUZo8S5NCHG9hxv9H1Pcj3XSctSLdZFRTer2ROW/nWIzqyqaeZpx9B2na8Z1a6Xel1Mg4SqNA5enFR6LG25hWnKd2cXGMxO+qmycqpnBhxNneb2qTGIyqirhFbWb6zhcWaJTtqSdVLWJapS3JFDfv16+uuM5xtJqCWkeNj6VJ1hDdG06irCuXWuFMqArjaprRsXRQkkhAxKVcm0KXi/SrvgJnXu7E0ls4jKYp2GCIVSuq1JUzYCuZCleJtYWLa2JesYrCRlEP3ntScM9y5FTOyzx5Y/D+J75yp7fcVvi/NlxPVs3VeNnh45WDGOeEYMZV5CmT6ambtRR6demzz3zOXyFCioOSHOakzTlQZTUGkOsuUkJHmHgPO6UYTKbQpXSqiJjZnSlUbqcg4cb3SbQKnGKs9FamlJlPKmSOFQSwmXcIqZVopJYU9MRYjap76rnW0x+T+U7V+2t1OkTtU9kwplEyRusK5X0xx08avPKeOSAR3d/RIRkr0W3yFJpvGIbXHSSppNs5s9k1VS7uC2s3tbAvWNpLmLifAV1ls6NajWygPgoEdZ7Lc0s1RBqAoZpwTES/cEdBRNPH6HWcIZmLeXIibzdFizTC95aDgW+YxjplKMbwn9n7PSo24gxygaC5HG+2a4U6TxZt37nvXtqdb7c5Jy8ovvDMoCuXZBMRmYkD5HqqloaDKh483HoOyEoPLRE/l02n+QqRVc0S0FN309NLMc/aWxEjwaZApdcJEMook3MQpikHVTuCILysYSJeQgxwYt3lrWLbLaz5t+cmoaxJwY4FkEJiAoTHIcLctMZZirGMkEReG2KHfwkIb4nEnnoYRGUR652VIhCWJnhfG09EWPWBmxEwHB06R/DH8cGQK81PmupNcc+dHLIcrbJhcNxYeE4oZtukzb3raJRAkDporuQkz59ti90KAFrIiCXVpkGKzZmZeQBxOOztOk6oE4mSQvEhZX1a2MU6ZtuaMskoLFiDTHxueUjvjBGYUSBsoJRESJA+Z986B+Bbz40+jObmtXAxQifz6L4BPtfT/eAnygE0RAB6fe97z+Xy+fsu9/v+DwoHkBir7GgB92jjGrfivQYEIvylzWJ/ViM5v2tZbag/fpu/PH68f1zewriA+Zx+Xmc68RefGHjl+eeoFlEq8qmpNFGCVxhoF8F471y6yZfbzXnoPnrmb8P4DNUEXpVrnno+OurN6nrnj4tnjMRjwze6vnfoBtW8QZeedSZY1nfMQVtNdc6s0bM1WQImAlRtlTuibm1RmWtN6nVam8JT4xBRIlti9OdXcuU01ARnZxnn5j6BR29dIepbstaN3KX9tjD28FPlRQPIZqE7cSxYfHai0cQD7MMa0XyoQvFKgu1UHqg1pTwhbMrWKLqYPsKNsF+klqTErL0n0bUCRumbnx6kwG+3u+1eNXJGyDd2u6fZNXA3yzlFYqCuroUbXbsnZgilVsQaZyPNVGVUCsyuM5qgxmRkRbETRmzy9kF9Shljs2zNONCiydFT1fwlQE2ZI/lKTE6yottoiVc8qi/qt5dWfKQuHDAqmUFWwPrHtkWP5NJJRYDLSWsbDPsDUYrStukWBFRWAS6sAxIshPj/iyBMIRRVkRNfNx1Cak1kZ7hYhRfV+WIqF+Gvlc11WLq9mFdyYtocR6f5fYXeB9d/kIdIb/WU9C+4pe5eI/iv1WnQXQkJCZfP0xz34MhpmKXn3C1h7HMSSEx48gO9c3sUwni9BK1yWzWLDcVqA8UTq8futlOgXEKX8xT667ActqLBkpPZ2AcvFpEyed676KXsxbVT2Fcl31TBwE7QpgXU/zuId7yPds23IsgngD5JH/JfSBY7c9SGNNpbVWvgDBaS5e4V7OgjPDMHJ0vUPEMgLXrSOGhXFO8yDA17NCup1SHKmWT5G4M2lOrZy5qYwHt9z+sEL7X+fSx0EPw3ZC8k7lKNMmWW6bviXxPBTVcJ8NKWue+EjJlJMsD7tqKtkLNyBzkOMh3mThz95XoJnxh1Y2vsSVVCQgyElVQUq87FOB2j1n9HDjiZMIhIH7BUrm9Xqh0ab8fcfl8z6aLTzxyEwlxTRyELLgF1Fe03WmPtnQnZIb8qsEPpnrwdcexVBPKX05qXvqJpSK4nV5HR5URtthGxGDADbqPdt6r1txWaSWD4/XdVVCqEuAjHSGfTQ74akg+NrmFgxt+fPSKQTGmMYwU2QPfSBPPdIirAEREcpVXwEqAoxYiRVVVVXxh7U8QP0zuJkNQ7T9Q8hwB0QVCkXfDdUO9nsUpK3zazBWlZVgsSYVBKE8FagZAsGMt/ZLXc2Gymg19yTVWwSwc5IqsWJIpQyb4KQincdDkmjWyKfWpalqb5xrjaIorMSqgyCVGMjHM5CBmavIwv55Yo0NjIRx9CGDIWG4JAvGradvMJ9TwXPJ07JOUQ+7aytVxOmSxsZCkRmrYa7Q7hjEO4zU/QKUh2ybE4GQ8Uw0bHM/GYxFiattqiqgIrEYiIgREwO4Tz3qmBIhzKw1NSmSTgXWwWMJg9KdVyD5DBnGWM1cztCA6wizt2iSYwWSVJ1eojkIsFQHgWVeQIaBCgzbMKNSM4i8Es2QmQZI6djuZi3dHMLqv3DQNpGEizdwnAI6HAbtNjiAeB5gdHhBmXL6eTgYfuABlFA2SApqNs9kyAwXYBLgoXE1RDMZZtxja6DptV9Uy4TuhTiDxeYG+6OFUTkHWCHDt3kEJr8XGOfhE1oYEEi4THZ6Sy5egQzNNKBUGSkcSaFQauMzh0nDrfmv2V+ZAXCEo24ijiAgOSgPYpqQOwO9NA6Q88WlDrGoyfTseczGIMmVE+2d4uWNEObE1Q6iUyFitdGHAGK5RObJs14iHmGAIQhsbpycnMRE5QD66BEGRBAUEEAC96t+HVu/NIdxeVywTiPHBYISMUIRQcXiNKInFANbOLp1OmE0GTw3VLEcRxdipYc56VpCBznK4jQhkEeFEMVOlXnU21wAhqbv6ojnq7qJmplgyNIMhlKIEjARiiIqhFkixYiDIMPYkksHpDcDGnfl7BqqkPavPw6w7utl7wtVEsJZa+8zAfeprASESREIsBTYxXQLqn3QP7zMCzhhx24yDqBEih7tSSBJNcrJVERZiPOEQfNXYwwsXUbqHENvLmV/l+zNUkxnqlCS3VUDj7n5eJBzulryCPGIKMbazBtW5zUfzrYdxJCJCQThyTZwhKApgd9WIqkPcLiOQS0sMc2Pdq4YjhnRJpgysDICUGm7BKNAaTOGAywUOOwQXK3Ukgzw2jdIGU7QcKi2UQqRMIKE/qsPcoZ3jJISYQKKMcPIkKlYKKLROLCjwZu6HWRg7/dwNHXL8XNC8fcvIxcoQ5TQwRNMDhSlyyrHUIOre6YojDSTgfz+Gut9NVKKQBQQY8Lx7uzbXbOsnt+OeB42NhVIBJyKClgSMYvDWyPgBXJ33bAZHW8knKBxj9E9/pr07SqCatgK4W2lNqjNvXKKAhouzNla7WomXMKhuY7TBtq3bmLt26WSwpGkCPJsXFhdaKKIECNwjBwkLsLPxa8Hk9fQdl0+rDdBPSHuskOUCGkZ7QE9SLABUiqCrFGMQRelBsKxSllUFAYsE8VMZSDQGr2xoJGLKHE60LZkYgq6zfEym1rCmizoKef4wqBE+NNqDgNbJLIeaIlR2aOXEd/Llpa0hJYOXeqvDWgHLHMk8K7TWXglAthw4Jcp1TRBO7mhXhjSWCy1VOPY6XdQFqL101dWCzNhse/JgLHDEgzu5Vs0h5qFLTjQNl5gmIEe60wxOmDGmhceUkTHE8OO+2VjbaR6+XfXTXI+dIcYgRyzwt4GhkGE4Cr+/3X+3hVccRsJXPApSTbfNVznElCklJ85xtnfGdKGiFCQbLjO7uDtuykGGKRZKqbNd+lrpOnnbwFOF68qsTysSwiUHKwg0b71WG4bdqK2YcqJtD2UEnDjkL0pbavUl4RLxmMybasKrIajMZUN2ZEOKcORZWTSbbyZ1DO5THjuFmgm7NruXpkMyxTCMxk1hRMoGKkE6PIySx6uHI0vVlgWYbsD1ppUF4ToYUczJpeR2FHUpEsWEbl8yJsUBotxwIbmpdR0bCUGHBghDIYUHEJhibkaDUzKHIIlKfQQEyLNDwBSd2ocQ2cP34JdMxZDNLBE+MC3ZiVHTsp1INVewIdjGeGuIZUmW4IhmiQDkwyDihonXPv1UUT1vrnsJ7mFFDRTIyBGQ5uQmkrQ6ayiArEVoOrzzJId1zY/cfN1CmcCVNEE5jVIUQhVFII3BBClcYCJrdgYJhFTBRpW0Udw1ApSlhULzW78Xd2emule0LZnuLC22XyibruhA9a0jANSAHW+/f1eZVHvCXwO18OEs6n2dMThpoEK840zztA1WGsPqa7iOqb2q0JWxtm/VbnlGQkkkJJEZMhbkcx2ukV03dzk1yGyjkEZYNpJIdwJ1dqelmOXxMmZg3CmJg5r4NZsW/X7GWkWlcmRNpzITKHScpQCQih1BKlWqoh3EMeOvapkaZQNBzNTINpIsB8aB1MkMQRgKAxBSLCBRIGHIUBSoMUC1wgugQ0hu6mw2Df1h09+cpQNtuXvSlOCHvNnKIMEkyhZfDL0WN7RWFPlWZsFi5VFF7XANi0G9Hh5eBv6wHEWEgpFkQEgxSCIxSIggkUEGLBhFiCiggRGC+0ySFiKIlgGjqiRiCDEgJEkAkAExFBohGQYoxYAucpGBAhAgkRlrWcGSMgG/BNhzzhfCmDMD2g6PFAXTAd5AaCBsFNL0IFUUMEWtHJmAmJSrFgZctLMQYsXogVgocz0AiIlDyMhNMfvDoJxNlsZOnhGxbuIUliWpKk2Q7nREO2/xSQktBc9TkW9/QSxYOKdsBkAeqCRCRhKIsQmxSOAw5hUNZi0KZJQ5paGQGAHZvOsO+XNG7yRILJERVBhGAbAIMQF1APNOmVEJA0hERpKKKUKaVoxBHM9gNAXYjgH5sJjARAiwwtZv4BEzGNvswHuE0YgjnNlUaDoQ4FI/YkgpipETqI7tg8zvcD0e/g9724ljFCHtDV0u1QTamS7qRpKyR3Jq5mxtMVTHNWYZLExPhyaJhKI/Hvw01G8QqqZi0m5sHygcEBc7d/cCN34kCAH0BdPRZQGlQihkeciJmsHR8APJ/b5ZCpFpGCgKCk7MeiUCD65isgMJWHb0zNp62Kp96yj6/ZuaPSXWGyOqqBe0K8PAovcMbK9TtQSHDIM4el9B1pr5bTxPwWaw8tuKjdUyuejN5FDU9Mx4zuhDkq8bBmPOJvyiBcigJwb7oj9ZYIm50UQPL3FPcoVEOnYUZhX6rOwic5DQeokndPkQgqe9owyzmEEgpFAPAxSDqPEX6jaFWMrM70u+26TwsAWsocQkEjBgG1boG06kuIFpBHX5ugBY5bYN+gmkjaI4C6fL1IFLYiQQ9PVMvlgjhl1EkVBMAYpl8In5hty2EtJTljmD32UBSIxGLFgoKBrSFwiqlpTkkDyBA47G4h6xywGghSiQ9BRUXq2SqaZIWttCWilg2SDSgsFhbQUFtixWLYaCWxRQchYWbFkqCkYRYRIHzMCw3TN4C8G02nVbKIGugU1CiQixCRCUyIQ59MB80VPfsiglBRzp+EZlPH2UYhfDUMVzRO51QS186zFCWhnvjmCQF5/AogkQhLWWaECVTG8C4WEMXoUDKdDpORTtHIoiAp4FMZr3rNob0bqGQ136Dps4iSI0AWCRAdrQkCJGwLhUIK4A8Dvbdx6qNto9r2X9dkXYIp6GWemGht8gI1Fal1vFCDO8TopR1ioyMhIJIKgSIPEyO3JN6krANzcJSgOQZvI4mLgiZgU2kRiHZxwyZT28K4OjENQvxMntcinf2Ekl58zginEVcgMG8CBD28kz100tREahRYb6qGROTsmWwwAeeVTFAjvIQxHVlwMzBIrETRSsl05mGZMykoiItaRiLAu+BpzQWU7k+yMmWxB9/JYKChFXclgk2zGOINzBrXBhbx0UplmNEttW0iyUQS2Ug0OWGLIoYIQGiJZJLYxrJQtldMDGQzAlKiQNjQZJALEaQLOryZROq31appGSiUEsUVNXIlXOp1VtJY3WsLclO3UmK5n7eSsNCOscrCtRyrsrRUqTUFHNSypEjERVVLKct06hwpKcUUpZ5FisRbkhg3fyeIcAu8VQ354LhhColQZUMIwCMWYLOyJJgzCFMYoIMisVYIECAHqn7W24hocADNAXJEDVIzzPBsibgkVcyZJlYmDXdrbYuqbQCyfWOnOCPJo1KwIs66dQJCCCREREZIImASdQUG/ZNcT30huSZsHFRFD6cOpnEk085dSU/ItVQkYgasg8e+QXvmTl8dJYP0AOA7YyGdJUX1uv4gB9qASIh3QHwGfcgaKbqnj9juEXvgIQEIAEgISSLkbZAkENmzPccLBvIBxgdY0KeMIosSKh8J9iIdfGlmVDtHpEqFQLx7oCG0TKC3gNoWi5hYpGhFgur+gaxAoE3IKkgKaGbkKFkF3X0dqDhA7S93Sa4S+4GCjUBQjFkYLVO4VGQm40Ybm0NCVGTlYoBNzcFcMWZcKBauNocjw/OgM/AyqUUdSkKkiEtHqN2gG7EAGgaaTPQKqPEMjM0hPckk/FmWcIpgZtNrgC7EtAWjI3cqWkYbvQIKkTrL++zf6gZHbC9h8zAKdcQErxZL1d8v4DAq6FopEG1CnmqgJawyt8uHgXd4mj0IFw9Y1qiVbv+YB6B9A+rvCCHQJElTh483iU23OSKZWt07MzIDJQ3OAV+IDmDrpvKzNBRg5kh0d3LeMJCQj5cUSR/TIr0DtDPhoVVLQPXkqJjwbtILsVWgcLhDx6gW+wRHLjqRvt6waE8svCAHMTgOkLCNH5KWcEqponILCyJhJgzKXEIdrKyOuyzZjBBdFs3NhmQcSMtmgtjBw1cBMZqSKRTJkXIkU0mkIFZPGWUytJWMPQmKoJgsiEG6ULh352zW978z3mSmAQe7Kef5zuVPqPF7UQNYCnDt2udoOFXgvTQHpCSyl2jbugdyRD18hLHD2nfXglyWcg0u8pbhsK0K4HXu4TkxK8eCbDdCQvBfSfM4/S33PeaiilcQMBBWauYQya05VKNSWumTAYyYkW1TkSbvoFBPVzpVHoPlFOinf6DLziJkgL4jNHxcaB83yAZry7+j20Fu1JpsKGa8DdPtkUDGAsZEnWktslVRURZBkUASIBJChAbk26US9kigXQSCkopKM+mEngJJ6dxDq6zELaJvuk4K2AxJoS9+/YArzI5I6e2H1XjpUUkinQxyVRKUdaccnu48fSbyiFfMySkfePhXvFT0zGFOEmxmXMuJEe5NTVTRdeBAYXYfeDspkStvsnSiGubNJ8nt3gtccuVJOqzMMtxcTLPY1VKJEW3UaxaXGJh2xOGq+274FodOBkd7yy4WCmXI1l2GaYhKRDMuJiYgEEocRierqUkQ04ttGKDEUMjwjcDRMFpaGMoM6tUtDcnVmYMgWbEZzc4gwzEAocEGaqLecURFkZKFJkRiK7vYk2mHNyubkV6DhxJRDQseBAwkZGRaCRllQMDJHl3Weg8/HO2EeVDfIWXDs9KUmaS0IGMFVIBBNbOi+wpo4gmYoWwQC3FjG5if6P7yIH+ybJ/n8l7BgiH53zE+PA0HVRSr8JyqzDQcWrEByMGNSsWPXRaFyY5RSFYxoluKGExsVNMEMFGR3ArUiuWaW9qhMWKtIcjr0U8g8rk8j1I+IZmbo8ZlWjL5N5k4V4MlLgM0aDaQV6AC5NOJgbuejzUPtCHS+lk3XpplGZSa0hy2vMBCLJKC7zBxsAxSBxEaOQYByDt6Owbm4YtF0PY+wtEheupmZ88DVSh80qKRB5FHe1IDwxB4EI763To7dJA+/YdgJbNe18UQMHyDpL6D4NG6vYm4o+TNG+g3d5IfBOUjjwx81hSDsWzLWG++RHtdEWA4hQ4NTkxvNyRDmY9a3Zypw2XZBXzhKCFNdClIjZPllK1slEDUGkTim2MSjCAm7stKrJkUpQy5LbxKNiEM1w1FHLA0TSWipbUFPWClvnAsN3pzEIURkQjz2AOqQlzw6RRlb8klXbQJ8s2IgZQYKOYg4JmQiMAsYA2kghIYXNNCyMLlzRLpfCuXGlNgCuvUKTzB2nUXDeJGSk2j6LT0jtsFD3/GayKxhUmp9Xlhk2s4zMqt6prit557HEH7ZEOjzgHWw2U6+JIkCiiPjIFrXLheIRGHgeJQkROSccnQ9AbXaqNwM3kIl+gqEtlVXA+WXqqQF6k7yWeqZbJawgsVWDFtAsUZFgEGExJrNOoGbOffwsmjhlKVyRebMg4DgQmXuDa3gaa627rG2KiwkSHRq8JAaSABiAGGJTAZAY1AiyDO7oB5CQ6aDeUlJ3fDgg9McUeQUqRYEVYUFH9n3k1NOYByxMW+KBewgT7MZ40Npzcoai+l2G8MgHzRXsHJhSBCsPB4XVe53hHjVMFDiWYZYwEQUWxERniF6N3GeeWEFyaIQLlDLRH4RuBch6ApUMgG5CwRAFVSMg/aPkkkDcBCQ4EPCM8PDwB8T28mgdFtilyhiZcuLcHM1qo4GFBirsYyS5Vij4J88yPbnkTZReAq0HWkktJuA5yEjsFrApsSDvisM+ZTJBvwLn0eYMboCxV+B3ggvpXnwOR6DIB73/YHUvGQYQA0XIm6JVgv04wfSCHawF5MTe+VCfROE2OyfiWqhXzrvc/NQxEO9jbCqII+RA09U11uGgEGEG15NLJjx94nbE58+i13hIeMRPZOz5L/9fJ8vyf+83y+78u3VqPtDjRzvpA1IDvUbPgJ6J9eG9hv9MLywec2v9nZW4aBk4aztTG/vYpgc5RpKAfhg+EFvX2gIKucgL9QOMBdgLtOntmdxJ0iKIcDb3kEiSiCCKEJIpItJgoU54AZ8kUvyBM+D4RX+OB57VE9FTz08XQwfnpMTdPwjbZ9Z1oLD6yLSKooqnelENebMYpqhZ0Ah98XckU4UJDtu7wzA==
#<==
