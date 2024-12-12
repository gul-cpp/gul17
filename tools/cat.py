#!/usr/bin/env python3

# Output the files given as arguments
# Differences from real cat: Does never output stdin, has no options

import sys, os

myname = os.path.basename(sys.argv[0])

if len(sys.argv) < 2:
    sys.exit('Usage: ' + myname + ' FILE...')

returncode = 0
for filename in sys.argv[1:]:
    try:
        f = open(filename, 'r')
    except IOError:
        print(myname + ': ' + filename + ': Could not open specified file.', file = sys.stderr)
        returncode = 1
    else:
        print(f.read(), end = '')
        f.close()

sys.exit(returncode)
