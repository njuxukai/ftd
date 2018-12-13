# -*- coding: utf-8 -*-
import os

from io import BytesIO
from dtd_def import *
from file_util import *


host_data_type_template = './templates/host_data_type.template'

def generate_host_data_type_file(types, project_code, 
                                 target_path):

    data_type_prefix = 'T%sFtdc' % project_code
    data_type_file_name = '%sFtdcUserApiDataType.h' % project_code
    lines = []
    template = load_template_file(host_data_type_template)
    d = {}
    
    for t in types.values():
        append_lines = t.get_universal_define_lines(project_code)
        lines.extend(append_lines)

    d['project_code_upper'] = project_code.upper()
    d['data_type_def_lines'] = '\n'.join(lines)
    save_cpp_file(template.format_map(d), '%s/%s' % (target_path, data_type_file_name))


    
