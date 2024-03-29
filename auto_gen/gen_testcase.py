# -*- coding: utf-8 -*-
import os

from io import BytesIO
from dtd_def import *
from file_util import *
import datetime

host_struct_template = './templates/testcase_struct_compare.template'

validation_template = "CHECK(sizeof({0}) >= 0);"
compare_template = "CHECK(sizeof({0}) == sizeof({1}));"
fname ='TestXcpStructTestCase.cpp'

def generate_struct_validation(fields,protocal_version, project_code, target_path):
    lines = []
    template = load_template_file(host_struct_template)
    d= {}
    compare_lines = []
    validate_lines = []
    for field in fields.values():
        project_struct_name = field.get_universal_struct_name(project_code)
        ftd_struct_name = field.get_universal_struct_name('')
        validate_lines.append(validation_template.format(project_struct_name))
        compare_lines.append(compare_template.format(project_struct_name, ftd_struct_name))
    d['protocal_version'] = protocal_version
    d['host_struct_size_valid_lines'] = add_whitespaces('\n'.join(validate_lines), 12)
    d['host_ftd_struct_size_compare_lines'] = add_whitespaces('\n'.join(compare_lines), 12)
    d['datetime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    save_cpp_file(template.format_map(d), '%s/%s'%(target_path, fname))
