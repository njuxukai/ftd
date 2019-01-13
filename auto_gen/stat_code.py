# -*- coding: utf-8 -*- 

import os
code_dirs = ['../ftd', 
             '../xcptraderapi', 
             '../mqclient',
             '.',
             '../include/', 
             '../XcpApiCSharp/',
             '../dbcore', 
             '../SampleClient', 
             '../SampleServer', 
             '../SampleCore']
code_types = set(['.cpp', '.h', '.py', '.cs'])

def stat_dir(dir, stat, stated_file_set):
    list_dirs = os.walk(dir) 
    for root, dirs, files in list_dirs:
        for f in files:
            if get_fname_postfix(f) in code_types and f not in stated_file_set:
                stat_single_file(os.path.join(root, f), stat)
                stated_file_set.add(f)
        for d in dirs:
            stat_dir(os.path.join(root, d), stat,stated_file_set)

def stat_single_file(fname, stat):
    if fname in stated_file_set:
        return
    postfix = get_fname_postfix(fname)
    read_result = True
    line_count = 0
    try:
        fo = open(fname, "r", encoding='UTF-8')
        lines = list(fo.readlines())
        line_count = len(lines)
    except Exception as e:
        #print(e)
        read_result = False
    if  read_result:
        stat[postfix] += line_count
        return

    try:
        fo = open(fname, "r", encoding='gb2312')
        lines = list(fo.readlines())
        line_count = len(lines)
        read_result = True
    except  Exception as e:
        print(e)
        read_result = False
    if read_result:
        stat[postfix] += line_count
    else:
        print("read[%s]error" %fname)


    
def get_fname_postfix(fname):
    pos = fname.rfind(".")
    if pos > 0 and pos < len(fname):
        return  fname[pos:]    


if __name__ == "__main__":
    stat = {}
    stated_file_set = set()
    for code_type in code_types:
        stat[code_type] = 0
    for code_dir in code_dirs:
        stat_dir(code_dir, stat, stated_file_set)
    total_lines = 0
    for code_type, line_count in stat.items():
        print("%s : %d" %(code_type, line_count)) 
        total_lines += line_count
    print("total:files[%d]lines[%d]"%(len(stated_file_set), total_lines))
