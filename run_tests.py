#!/usr/bin/env python

import os
import sys
import subprocess
import threading
import shutil
import random
import signal
import argparse

class Ui:
    if sys.stdout.isatty():
        RESET = '\033[0m'
        BOLD = '\033[1m'
        GRAY = '\033[90m'
        RED = '\033[31m'
        BOLD_RED = '\033[1;31m'
        BOLD_GREEN = "\033[1;32m"
        BOLD_BLUE = "\033[34;1m"
    else:
        RESET = ''
        BOLD = ''
        GRAY = ''
        RED = ''
        BOLD_RED = ''
        BOLD_GREEN = ''
        BOLD_BLUE = ''

    @staticmethod
    def step(tool, parameter):
        if sys.stdout.isatty():
            print(Ui.BOLD + tool + Ui.RESET + " " + parameter)
        else:
            print(tool + " " + parameter)
        sys.stdout.flush()

    @staticmethod
    def bigstep(tool, parameter):
        if sys.stdout.isatty():
            print(Ui.BOLD_BLUE + tool + Ui.RESET + " " + parameter)
        else:
            print(tool + " " + parameter)
        sys.stdout.flush()

    @staticmethod
    def result(timeout, passed, name, retries):
        result = Ui.BOLD_RED + "fail" + Ui.RESET
        if passed:
            result = Ui.BOLD_GREEN + "pass" + Ui.RESET
        elif timeout:
            result = Ui.BOLD_RED + "time" + Ui.RESET

        additional_info = ""
        if retries > 0:
            additional_info += " (after " + str(retries) + " retry)"

        print(result + " " + name + additional_info)
        sys.stdout.flush()

    @staticmethod
    def globalresult(passed, message):
        result = Ui.BOLD_RED + message + Ui.RESET
        if passed:
            result = Ui.BOLD_GREEN + message + Ui.RESET

        print(result)
        sys.stdout.flush()

class PsUtils:
    @staticmethod
    def pids():
        ret = []
        procs = os.listdir("/proc/")
        for proc_dir in procs:
            if proc_dir.isdigit():
                ret.append(int(proc_dir))
        return ret

    @staticmethod
    def childs(pid):
        ret = []
        for p in PsUtils.pids():
            try:
                for line in open("/proc/" + str(p) + "/status", "r").readlines():
                    if line.startswith("PPid:"):
                        parent_pid = line.replace("PPid:\t", "").replace("\n", "")
                        if int(parent_pid) == int(pid):
                            ret.append(int(p))
            # process was just killed
            except:
                pass
        return ret

    @staticmethod
    def tree(pid):
        ret = []
        for child in PsUtils.childs(pid):
            for s in PsUtils.tree(child):
                ret.append(s)
            ret.append(int(child))
        return ret

class AsyncExecute(threading.Thread):
    def __init__(self, port_assigner, token, command, environment, working_directory, result_listener, retries, timeout=15):
        threading.Thread.__init__(self)

        self.port_assigner = port_assigner
        self.token = token
        self.command = command
        self.environment = environment.copy()
        self.working_directory = working_directory
        self.result_listener = result_listener
        self.process = None
        self.retries = retries
        self.timeout = timeout
        self.timeout_troggered = False
        self.out = ""

    def run(self):
        returncode = 1
        out = ""

        if self.retries < 0:
            raise Exception("wrong no of retries")

        for i in xrange(int(self.retries) + 1):
            (returncode, out) = self.__run_once()
            if returncode == 0:
                break

        self.result_listener.async_execute_completed(self.token, returncode, out, i, self.timeout_troggered)

    def __kill(self, sig):
        for pid in PsUtils.tree(self.process.pid):
            try:
                os.kill(pid, sig)
            except: pass
        try:
            os.kill(self.process.pid, sig)
        except: pass

    def __run_once(self):
        def execute():
            env = self.environment
            env["SERVER_SCT_PORT"] = str(self.port_assigner.acquire())
            self.process = subprocess.Popen(self.command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, env=env, cwd=self.working_directory)
            (self.out, err) = self.process.communicate()

        thread = threading.Thread(target=execute)
        thread.start()
        thread.join(self.timeout)

        if thread.is_alive() and self.process != None:
            self.timeout_troggered = True
            self.__kill(signal.SIGTERM)
            self.__kill(signal.SIGKILL)
            thread.join()

        return (self.process.returncode, self.out)

class Binary:
    def __init__(self, filename, result, log_writer, port_assigner, retries):
        self.filename = filename
        self.result = result
        self.log_writer = log_writer
        self.port_assigner = port_assigner
        self.port = random.randrange(3000, 3500)
        self.tests = self.__scan_for_testnames()
        self.test_result_lock = threading.Lock()
        self.retries = retries

    def run(self):
        Ui.bigstep("running", self.filename)
        executors = []
        for test_name in self.tests:
            executors.append(self.__run_test(test_name))
        for e in executors:
            e.join()

    def async_execute_completed(self, test_name, returncode, output, retries, timeout):
        self.test_result_lock.acquire()
        if returncode != 0:
            self.result.fail()

        Ui.result(timeout, returncode == 0, test_name, retries)
        self.test_result_lock.release()

        self.log_writer.write_log(test_name, returncode, output)

    def __run_test(self, test_name):
        env = self.__prepare_env()

        async_execute = AsyncExecute(
            self.port_assigner,
            test_name,
            [self.filename, "--gtest_filter=" + test_name],
            env,
            os.path.dirname(self.filename),
            self,
            self.retries)

        async_execute.start()

        return async_execute

    def __execute(self, command):
        root_dir = os.getcwd()
        os.chdir(os.path.dirname(self.filename))

        env = self.__prepare_env()
        out = ""
        try:
            out = subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT, env=env)
            return (0, out)
        except subprocess.CalledProcessError as e:
            return (e.returncode, e.output)

        os.chdir(root_dir)

        return out

    def __scan_for_testnames(self):
        (returncode, out) = self.__execute(self.filename + " --gtest_list_tests")
        lines = out.split()
        ret = []
        test_suite = None
        for line in lines:
            if line.endswith("."):
                test_suite = line
            else:
                ret.append(test_suite + line)
        return ret

    def __prepare_env(self):
        directory = os.path.abspath(os.path.dirname(self.filename))
        env = os.environ
        env["LD_LIBRARY_PATH"] = directory
        env["SERVER_SCT_PORT"] = str(self.port)
        self.port += 1
        return env

class Tree:
    def __init__(self, result, log_writer, port_assigner, retries):
        self.result = result
        self.log_writer = log_writer
        self.port_assigner = port_assigner
        self.retries = retries
        self.binaries = []
        for f in self.__find_binary_filenames():
            self.binaries.append(Binary(f, result, log_writer, port_assigner, self.retries))

    def run(self):
        for binary in self.binaries:
            binary.run()

    def __find_binary_filenames(self):
        ret = []
        for (dirpath, dirnames, filenames) in os.walk("__build/__default"):
            for f in filenames:
                if f.endswith("UT") or f.endswith("SCT"):
                    filename = os.path.abspath(dirpath + "/" + f)
                    ret.append(filename)
        return ret

class GlobalResult:
    def __init__(self):
        self.errorcode = 0

    def fail(self):
        self.errorcode = 1

class LogWriter:
    def __init__(self):
        self.log_directory = os.path.abspath("__build/__default/tests/")

        try:
            shutil.rmtree(self.log_directory)
        except:
            pass

    def write_log(self, test_name, returncode, output):
        try:
            os.makedirs(self.log_directory)
        except:
            pass

        prefix = "pass_"
        if returncode != 0:
            prefix = "fail_"

        f = open(self.log_directory + "/" + prefix + test_name + ".log", "w")
        f.write(output)
        f.close()

class TcpPortAssigner:
    def __init__(self):
        self.lock = threading.Lock()
        self.port = random.randrange(3000, 3500)

    def acquire(self):
        self.lock.acquire()
        self.port += 1
        ret = self.port
        self.lock.release()
        return ret

def main():
    parser = argparse.ArgumentParser(description='test runner')
    parser.add_argument('-r', action='store', dest='retries', default="5", nargs="?", help='number of retries in case of failure')
    args = parser.parse_args()

    result = GlobalResult()
    log_writer = LogWriter()
    port_assigner = TcpPortAssigner()
    tree = Tree(result, log_writer, port_assigner, args.retries)
    tree.run()

    print("")

    if result.errorcode == 0:
        Ui.globalresult(True, "all tests passed")
    else:
        Ui.globalresult(False, "not all tests passed")

    sys.exit(result.errorcode)

if __name__ == '__main__':
    main()
