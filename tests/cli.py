#!/usr/bin/env python3
"""Smoke-test the CLI and rejection of malformed/unsafe FEN input."""
import subprocess
import sys
from pathlib import Path
app = str(Path(sys.argv[1]).resolve())
def run(*args):
    return subprocess.run([app, *args], capture_output=True, text=True, timeout=5)
assert run('--help').returncode == 0
start = run()
assert start.returncode == 0 and 'Legal moves: 20' in start.stdout
assert 'e2->e4' in start.stdout
assert run('extra', 'argument').returncode == 1
for fen in ('', '8', '7k/8/8/8/8/8/8/K7',
            '7k/K7 w - - 0 1',
            '7k/8/8/8/8/8/8/K7 w KK - 0 1',
            '7k/8/8/8/8/8/8/K7 w K- - 0 1',
            '7k/8/8/8/8/8/8/K7 w - - 65536 1',
            '7k/8/8/8/8/8/8/K7 w - - 0 0',
            '7k/8/8/8/8/8/8/K7 w - - 0 1 extra',
            '7k/8/8/8/8/8/8/K7 w - e6 0 1',
            '7k/8/4N3/4p3/8/8/8/K7 w - e6 0 1',
            '7k/8/8/8/8/8/8/K6P w - - 0 1',
            '7k/7R/8/8/8/8/8/K7 w - - 0 1',
            '8/8/8/8/8/8/8/8 w - - 0 1',
            '7k/8/8/8/8/8/8/K7 w - - ' + '9'*10000 + ' 1'):
    result = run(fen)
    assert result.returncode == 1 and 'Wrong FEN' in result.stderr, (fen[:100], result)
print('CLI and malformed-FEN checks passed')
