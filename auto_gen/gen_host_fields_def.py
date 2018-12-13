# -*- coding: utf-8 -*-
import os

from io import BytesIO
from dtd_def import *
from file_util import *


host_struct_template = './templates/host_struct.template'

single_struct_template = './templates/host_single_struct_def.template'

def generate_host_struct_file(fields, items, types, project_code, api_data_type_file_name,                                              target_path):
    struct_file_name = '%sFtdcUserApiStruct.h' % project_code
    lines = []
    template = load_template_file(host_struct_template)
    single_field_template = load_template_file(single_struct_template)
    for field in fields.values():
        section_lines = generate_field_struct_def_lines(field, items, types, 
                                                         single_field_template, project_code)
        lines.extend(section_lines)
    d = {}
    d['project_code_upper'] = project_code.upper()
    d['stuct_define_entrys'] = '\n'.join(lines)
    d['api_data_type_file_name'] = api_data_type_file_name
    save_cpp_file(template.format_map(d),  '%s/%s' %(target_path, struct_file_name))

def generate_field_struct_def_lines(field, items, types, template, project_code):
    lines = []
    item_lines = []
    for item_dict in field.item_dicts:
        item_lines.extend(get_universal_field_item_define_lines(item_dict, items,types,project_code))
    
    d = {}
    d['struct_prefix']  = 'C%sFtdc' % project_code
    d['field_comment'] = field.comment
    d['field_name'] = field.name
    d['item_lines'] = add_whitespaces('\n'.join(item_lines), 4)
    lines.extend(template.format_map(d).split('\n'))
    lines.append('')
    return lines