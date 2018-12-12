# -*- coding: utf-8 -*-
import os

from io import BytesIO
from dtd_def import *
from file_util import *


host_struct_template = './templates/host_struct.template'

single_struct_template = './templates/host_single_struct_def.template'

def generate_host_struct_file(fields, items, types, project_code, api_data_type_file_name,
                                              data_type_prefix, struct_type_prefix, struct_fpath):
    lines = []
    template = load_template_file(host_struct_template)
    single_field_template = load_template_file(single_struct_template)
    for field in fields.values():
        section_lines = generate_field_struct_def_lines(field, items, types, struct_type_prefix,
                                                        data_type_prefix, single_field_template)
        lines.extend(section_lines)
    d = {}
    d['project_code_upper'] = project_code.upper()
    d['stuct_define_entrys'] = '\n'.join(lines)
    d['api_data_type_file_name'] = api_data_type_file_name
    save_cpp_file(template.format_map(d), struct_fpath)

def generate_field_struct_def_lines(field, items, types, struct_type_prefix,data_type_prefix, template):
    lines = []
    item_lines = []
    for item_dict in field.item_dicts:
        item_lines.append('///%s' % item_dict['comment'])
        #item_lines.append('{0}{1}Type {1};'.format(
        #    data_type_prefix, item_dict['name']))
        item_name = item_dict['name']
        type_name = items[item_name].type_name
        if type_name in types:
            #已定义衍生类型
            item_line_template = "{0} {1};";
            host_deravative_name = types[type_name].get_host_derivative_name(data_type_prefix)
            item_lines.append(item_line_template.format(host_deravative_name, item_name));
        else:
            item_line_template = "{0} {1}{2};";
            basic_type, len_spec = get_basic_name_length_spec_pair_tuple(type_name)
            item_lines.append(item_line_template.format(basic_type, item_name, len_spec));

    
    d = {}
    d['struct_prefix'] = struct_type_prefix
    d['field_comment'] = field.comment
    d['field_name'] = field.name
    d['item_lines'] = add_whitespaces('\n'.join(item_lines), 4)
    lines.extend(template.format_map(d).split('\n'))
    lines.append('')
    return lines