#!/usr/bin/python

import sys

if len(sys.argv) == 1:
    print "usage : %s dict_file1 dict_file2 ..."
    exit(1)

d = {}

for fname in sys.argv[1:]:
    with open(fname, "r") as fin:
        for i, line in enumerate(fin):
            try:
                word, cnt, tag = line.strip().split(" ")
                if word in d:
                    print "error file[%s] line[%s] : %s" %(fname, i, line)
                    exit(1)
                else:
                    d[word] = True
                
                if 0 >= int(cnt) :
                    print "error file[%s] line[%s] : %s" %(fname, i, line)
                    exit(1)
            except Exception as err:
                print "error file[%s] line[%s] : %s" %(fname, i, line)
                exit(1)

print "OK"
