#!/usr/bin/python

import sys


def get_pointer(str):
    beg = str.find("ptr[")
    end = str.find("]")

    if (beg > 0 and end > 0):
        return str[beg + 4:end]
    else:
        print("Can't get point from log line")
        sys.exit(-2)


def check_log(list_log):
    alloc_list = []
    free_list   = []

    for entry in list_log:
        if (entry.find("wrap_free") > -1):
            free_list.append(entry)
        else:
            alloc_list.append(entry)

    for alloc_entry in alloc_list:
        ptr_str = get_pointer(alloc_entry)
        is_leak = 1
        for free_entry in free_list:
            if (free_entry.find(ptr_str) > 0):
                is_leak = 0
        if (is_leak == 1):
            print("LEAK DETECTED : %s" % (alloc_entry))
        


def read_and_check_log(file_name):
    try:
        fp = open(file_name, "r")
        log = []
        for line in fp:
            log.append(line)
        fp.close()
    except:
        print("Can't open %s" % (file_name))
        sys.exit(-1)

    check_log(log)


def usage():
    print("usage: %s <file>" % (sys.argv[0]))
    sys.exit(-1)


#### Main ####
if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()
    else:
        read_and_check_log(sys.argv[1])
