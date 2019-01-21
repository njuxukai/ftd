# -*- coding: utf-8 -*- 

def load_template_file(fname):
    result =  []
    with   open(fname,'r', encoding='utf-8') as f:
        result = [line for line in f.readlines()]
    return ''.join(result)
               
def save_cpp_file(content, fname, encoding=None):
    if encoding is None:
        with open(fname, 'w') as f:
            f.writelines(content)
    else:
        with open(fname, 'w', encoding=encoding) as f:
            f.writelines(content)


def add_whitespaces(content, count):
    lines = content.split('\n')
    lines =[count *' ' + line for line in lines]
    return '\n'.join(lines)