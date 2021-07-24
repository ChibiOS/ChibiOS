#!/usr/bin/env python

"""
This script is intended to be used to run make under supervision for
the purpose of capturing its stdout/stderr and retcode to generate
a JUnit XML report. This report can be used in Jenkins or any other CI.
This report contains one test suite and as many cases as many targets
are specified.

To get help on usage, possible options and their descriptions, use
the following command:

    make.py --help

An example of running this script for building a demo project:

    make.py -C demos/STM32/RT-STM32WB55RG-NUCLEO68 -f Makefile all

This script requires the following packages to be installed:

    pip install junit-xml
    pip install pyyaml
"""

import argparse
import os
import re
import subprocess
import sys
import time

import junit_xml
import yaml


def get_jobserver_auth_fds():
    """Get "jobserver" file descriptors from MAKEFLAGS."""
    make_flags = os.environ.get('MAKEFLAGS', '')
    jobserver_auth_flag = '--jobserver-auth='

    for make_flag in make_flags.split():
        if make_flag.startswith(jobserver_auth_flag):
            fdr, _, fdw = make_flag[len(jobserver_auth_flag):].partition(',')
            return (int(fdr), int(fdw))

    return ()


class SkipRules:
    def __init__(self, filename):
        self.filename = filename
        self.rules = None

    def match(self, path, stderr):
        if self.rules is None:
            self.load()

        for rule in self.rules:
            if (re.match(rule['path'], path)
                    and re.search(rule['stderr'], stderr)):
                return True, rule['reason']

        return False, None

    def load(self):
        with open(self.filename) as fd:
            self.rules = yaml.load(fd, Loader=yaml.FullLoader)


def make(args):
    if args.skip_rules:
        skip_rules = SkipRules(args.skip_rules)

    directory = args.directory or ''
    makefile = args.makefile or ''

    path = os.path.join(directory, makefile)
    if args.prefix:
        assert path.startswith(args.prefix)
        path = path[len(args.prefix):]

    suite = junit_xml.TestSuite(
        path,
        timestamp=time.time(),
        file=path,
    )

    cmd = ['/usr/bin/env', 'make']
    if args.directory:
        cmd.extend(['-C', args.directory])
    if args.makefile:
        cmd.extend(['-f', args.makefile])
    if args.jobs > 1:
        cmd.extend(['-j', str(args.jobs)])

    jobserver_auth_fds = get_jobserver_auth_fds()

    for target in args.targets:
        if not target:
            continue

        cmd_target = cmd + [target]

        timestamp=time.time()
        start = time.monotonic()
        # To better control the use of resources by a subprocess make, pass
        # jobserver auth file descriptors in the subprocess, so it will
        # participate in the jobserver protocol and schedule its jobs
        # accordingly.
        ret = subprocess.run(cmd_target, pass_fds=jobserver_auth_fds,
                             capture_output=True, text=True)
        end = time.monotonic()

        case_name = 'make ' + target
        case_log = ' '.join(cmd_target)
        case = junit_xml.TestCase(
            case_name,
            classname=path,
            file=path,
            log=case_log,
            timestamp=timestamp,
            elapsed_sec=(end - start),
            stdout=ret.stdout,
            stderr=ret.stderr,
        )
        suite.test_cases.append(case)

        if ret.returncode != 0:
            skipped, msg = skip_rules.match(path, ret.stderr)
            if skipped:
                case.add_skipped_info(msg)
                continue

            msg = 'Ended with non-zero exit code: {}'.format(ret.returncode)
            case.add_failure_info(msg)

    if args.result == '-':
        suite.to_file(sys.stdout, [suite])
    else:
        test_result_path = os.path.join(args.result, path) + '.xml'
        with open(test_result_path, 'w') as f:
            suite.to_file(f, [suite])


def main():
    parser = argparse.ArgumentParser(description=(
        'Execute make targets and generate JUnit results'
    ))
    # Make specific arguments
    parser.add_argument('-C', '--directory', metavar='dir',
                        help='Change directory to dir (equivalent to '
                             '`make -C dir`)')
    parser.add_argument('-f', '--makefile', metavar='file',
                        help='Use file as a makefile (equivalent to '
                             '`make -f file`)')
    parser.add_argument('-j', '--jobs', metavar='jobs',
                        type=int, default=1,
                        help='Number of simultaneous jobs (equivalent to '
                             '`make -j jobs`)')
    # Test specific arguments
    parser.add_argument('-r', '--result', metavar='result',
                        default='-',
                        help='Directory to store JUnit XML test result')
    parser.add_argument('-p', '--prefix', metavar='prefix',
                        help=('Prefix path which should be removed for test '
                              'result'))
    parser.add_argument('-s', '--skip-rules', metavar='skip-rules',
                        help=('YAML-file with skip rules'))
    parser.add_argument('targets', metavar='target', nargs='*',
                        default=['all', 'clean'],
                        help='Names of targets to run')

    args = parser.parse_args()
    make(args)


if __name__ == '__main__':
    main()
