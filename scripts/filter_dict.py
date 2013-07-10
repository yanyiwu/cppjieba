#!/usr/bin/python

import sys

if len(sys.argv) == 1:
    print "usage : %s dict_file_path"
    exit(1)

d = {}
with open(sys.argv[1], "r") as fin:
    for i, line in enumerate(fin):
        word, cnt, tag = line.strip().split(" ")
        if word in d:
            #print "error file[%s] line[%s] : %s" %(fname, i, line)
            #exit(1)
            continue
        else:
            d[word] = True
        if 0 >= int(cnt) :
            continue
        
        print line.strip()

