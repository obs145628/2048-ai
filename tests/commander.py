import subprocess

import ioutils

def ebool(x):
    if x:
        return 'T'
    else:
        return 'F'

class Commander:


    def __init__(self):
        self.use_valgrind = False
        self.timeout = -1


    def run_cmd(self, args, read_stdout = True, read_stderr = True):
        run_dict = { 'args': args }
        if read_stderr:
            run_dict['stdout'] = subprocess.PIPE
        if read_stderr:
            run_dict['stderr'] = subprocess.PIPE
    
        return (res.returncode, res.stdout, res.stderr)

    '''
    Returns tuple (valid, errs, res)
    valid: bool, true if no errors
    errs: string errors
    res: subprocess object
    '''
    def run_test(self, cmd, code = None,
                 has_stdout = None, stdout = None, stdout_file = None,
                 has_stderr = None, stderr = None, stderr_file = None):

        res = subprocess.run(cmd, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
        cmd_code = res.returncode
        cmd_stdout = res.stdout
        cmd_stderr = res.stderr
    
        errs = []

        if code != None and code != cmd_code:
            errs.append(('CODE', cmd_code, code))

        if has_stdout != None and (len(cmd_stdout) == 0) == has_stdout:
            errs.append(('HAS_STDOUT', ebool(len(cmd_stdout)), ebool(has_stdout)))

        if stdout != None and cmd_stdout.decode('ascii') != stdout:
            errs.append(('STDOUT', '.', '.'))

        if stdout_file != None and not ioutils.file_content_is(cmd_stdout, stdout_file):
            errs.append(('STDOUT_FILE', '.', '.'))

        if has_stderr != None and (len(cmd_stderr) == 0) == has_stderr:
            errs.append(('HAS_STDERR', ebool(len(cmd_stderr)), ebool(has_stderr)))

        if stderr != None and cmd_stderr.decode('ascii') != stderr:
            errs.append(('STDERR', '.', '.'))

        if stderr_file != None and not ioutils.file_content_is(cmd_stderr, stderr_file):
            errs.append(('STDERR_FILE', '.', '.'))

        return (len(errs) == 0, errs, res)
