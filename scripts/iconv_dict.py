#!/usr/bin/python

import sys

if len(sys.argv) != 4:
    print "usage : %s from_enc to_enc dict_file_path \nexample: %s gbk utf-8 fname" %(__file__, __file__)
    exit(1)

with open(sys.argv[3], "r") as fin:
    for i, line in enumerate(fin):
        try:
            print line.strip().decode(sys.argv[1]).encode(sys.argv[2])
        except Exception as err:
            print >> sys.stderr, err

