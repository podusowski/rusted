#!/usr/bin/env python

import os
import sys
import subprocess
import threading
import shutil
import random

class Ui:
    RESET = '\033[0m'
    BOLD = '\033[1m'
    GRAY = '\033[90m'
    RED = '\033[31m'
    BOLD_RED = '\033[1;31m'
    BOLD_GREEN = "\033[1;32m"
    BOLD_BLUE = "\033[34;1m"

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
    def result(passed, name):
        result = Ui.BOLD_RED + "fail" + Ui.RESET
        if passed:
            result = Ui.BOLD_GREEN + "pass" + Ui.RESET

        print(result + " " + name)

class AsyncExecute(threading.Thread):
    def __init__(self, token, command, environment, working_directory, result_listener):
        threading.Thread.__init__(self)

        self.token = token
        self.command = command
        self.environment = environment.copy()
        self.working_directory = working_directory
        self.result_listener = result_listener

    def run(self):
        old_dir = os.getcwd()
        os.chdir(self.working_directory)

        returncode = None
        out = ""
        try:
            out = subprocess.check_output(self.command, shell=True, stderr=subprocess.STDOUT, env=self.environment)
            returncode = 0
        except subprocess.CalledProcessError as e:
            returncode = e.returncode
            out = e.output

        os.chdir(old_dir)
        self.result_listener.async_execute_completed(self.token, returncode, out)

class Binary:
    def __init__(self, filename, result, log_writer):
        self.filename = filename
        self.result = result
        self.log_writer = log_writer
        self.port = random.randrange(3000, 3500)
        self.tests = self.__scan_for_testnames()
        self.test_result_lock = threading.Lock()

    def run(self):
        Ui.bigstep("running", self.filename)
        executors = []
        for test_name in self.tests:
            executors.append(self.__run_test(test_name))
        for e in executors:
            e.join()

    def async_execute_completed(self, test_name, returncode, output):
        self.test_result_lock.acquire()
        if returncode != 0:
            self.result.fail()

        Ui.result(returncode == 0, test_name)
        self.test_result_lock.release()

        self.log_writer.write_log(test_name, returncode, output)

    def __run_test(self, test_name):
        env = self.__prepare_env()

        async_execute = AsyncExecute(
            test_name,
            self.filename + " --gtest_filter=" + test_name,
            env,
            os.path.dirname(self.filename),
            self)

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
    def __init__(self, result, log_writer):
        self.result = result
        self.log_writer = log_writer
        self.binaries = []
        for f in self.__find_binary_filenames():
            self.binaries.append(Binary(f, result, log_writer))

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


def main():
    result = GlobalResult()
    log_writer = LogWriter()
    tree = Tree(result, log_writer)
    tree.run()

    sys.exit(result.errorcode)

if __name__ == '__main__':
    main()
