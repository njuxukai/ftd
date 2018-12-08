# -*- coding: utf-8 -*-
import os

from io import BytesIO
from dtd_def import *
from file_util import *


host_data_type_template = './templates/host_data_type.template'

def generate_host_data_type_file(types, items, project_code, data_type_prefix, 
                                 data_type_file_name):
    lines = []
    template = load_template_file(host_data_type_template)
    d = {}
    
    for t in items.values():
        lines.extend(t.get_host_tt_item_define(types, data_type_prefix, project_code))
    d['project_code_upper'] = project_code.upper()
    d['data_type_def_lines'] = '\n'.join(lines)
    save_cpp_file(template.format_map(d), data_type_file_name)


    
