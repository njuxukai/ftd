# -*- coding: utf-8 -*- 
import os

from io import BytesIO
from dtd_def import *
from file_util import *

ftd_type_template = './templates/type_def.template'
target_fname = 'DerivativeType.h'

def generate_type_df(version, types, target_path):
    template = load_template_file(ftd_type_template)
    d = {}
    def_type_statments = []
    def_ttype_statements = []
    type_comment_format = "///T{0}(IO类型) {1}"
    ttype_comment_format = "///TT{0}(存储类型) {1}"
    enum_format = "#define {0}{1} {3}{2}{3}"
    for type in types:
        #def_type_statments.append('/' * 80);
        #def_type_statments.append(type_comment_format.format(type.name, type.comment))
        def_type_statments.append(type.get_tftd_def())
        #def_type_statments.append('/' * 80);
        #def_type_statments.append('\n')

        def_ttype_statements.append('/' * 80);
        def_ttype_statements.append(ttype_comment_format.format(type.name, type.comment))
        def_ttype_statements.append('/' * 80);

        comma = ''
        h, l, p = parse_ftd_type_spec(type.base_type_name)
        if h == CHAR_TYPE:
            comma = "'"
        if h == STRING_TYPE:
            comma = '"'
        for enum_value_dict in type.enum_value_dicts:
            def_ttype_statements.append('/'*3 + enum_value_dict['comment'])
            def_ttype_statements.append(enum_format.format
                                        (type.get_enum_prefix(), 
                                         enum_value_dict['enname'] if 'enname' in enum_value_dict else enum_value_dict['name'],
                                         enum_value_dict['name'], comma))
        if len(type.enum_value_dicts) > 0:
            def_ttype_statements.append('')
        def_ttype_statements.append(type.get_tt_def())
        def_ttype_statements.append('')

    d['version'] = version
    d['t_type_def'] = '\n'.join(def_type_statments)
    d['tt_type_def'] = '\n'.join(def_ttype_statements)
    save_cpp_file(template.format_map(d), '%s/%s'%(target_path, target_fname))
    
host_data_type_template = './templates/host_data_type.template'

def generate_ftd_data_type_file(types, version, 
                                 target_path):
    lines = []
    template = load_template_file(ftd_type_template)
    d = {}
    
    """
    for t in items.values():
        append_lines = t.get_host_tt_item_define(types, data_type_prefix, project_code)
        if append_lines is not None:
            lines.extend(append_lines)
        else:
            a = 1
    
    """
    data_type_prefix = "TFtdc"
    for t in types.values():
        append_lines = t.get_ftd_define_lines(data_type_prefix)
        lines.extend(append_lines)

    d['version'] = version
    d['t_type_def'] = ''
    d['tt_type_def'] = '\n'.join(lines)
    save_cpp_file(template.format_map(d), '%s/%s'%(target_path, target_fname))