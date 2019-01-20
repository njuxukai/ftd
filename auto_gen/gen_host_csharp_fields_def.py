# -*- coding: utf-8 -*-
import os

from io import BytesIO
from dtd_def import *
from file_util import *


host_struct_template = './templates/cs_struct.template'

single_struct_template = './templates/cs_single_struct_def.template'

host_enum_template = './templates/cs_enum.template'
single_enum_template = './templates/cs_single_enum_def.template'
single_static_string_template = './templates/cs_single_string_static_class_def.template'

def generate_host_cs_struct_file(fields, items, types, project_code, target_path):  
    struct_file_name = 'ApiStructure.cs'
    lines = []
    template = load_template_file(host_struct_template)
    single_field_template = load_template_file(single_struct_template)
    for field in fields.values():
        section_lines = _generate_field_struct_def_lines(field, items, types, 
                                                         single_field_template, project_code)
        lines.extend(section_lines)
    d = {}
    d['project_code_upper'] = project_code.upper()
    d['stuct_define_entrys'] = '\n'.join(lines)
    save_cpp_file(template.format_map(d),  '%s/%s' %(target_path, struct_file_name))

def generate_host_cs_enum_file(types, project_code, target_path):
    struct_file_name = 'ApiEnum.cs'
    lines = []
    template = load_template_file(host_enum_template)
    for t in types.values():
        if len(t.enum_value_dicts) == 0:
            continue
        if t.base_type_name.startswith('FTDStringType'):
            lines.extend(_generate_static_string_class(t))
        if t.base_type_name.startswith('FTDChar') or t.base_type_name.startswith('FTDInt'):
            tmp_lines = _generate_single_enum(t)
            lines.extend(tmp_lines)
        lines.append('')
    d = {}
    d['enum_define_entrys'] = '\n'.join(lines)
    save_cpp_file(template.format_map(d),  '%s/%s' %(target_path, struct_file_name))


def _generate_field_struct_def_lines(field, items, types, template, project_code):
    lines = []
    item_lines = []
    for item_dict in field.item_dicts:
        item_lines.extend(_get_universal_field_item_csharp_define_lines(item_dict, items,types,project_code))
    
    d = {}
    d['struct_prefix']  = 'C%sFtdc' % project_code
    d['field_comment'] = field.comment
    d['field_name'] = field.name
    d['item_lines'] = add_whitespaces('\n'.join(item_lines), 4)
    lines.extend(template.format_map(d).split('\n'))
    lines.append('')
    return lines


def _get_universal_field_item_csharp_define_lines(field_item_dict, items, types, project_code):
    item_name = field_item_dict['name']
    item_comment = field_item_dict['comment']
    if item_name not in items:
        print("Item项[%s]未定义与类型的对应关系" % item_name)
        return None
    lines = []
    lines.append('///%s' % item_comment)
    item_entry = items[item_name]
    base_type_name = item_entry.base_type_name;
    lines.append(_get_csharpstore_marshal_spec(base_type_name))
    lines.append('public %s %s;' %(_get_csharpstore_name(base_type_name), item_name) )
    lines.append('')
    return lines   


def _get_csharpstore_name(base_type_name):
    """
    public <String> itemname;
    public <Int32> itemname;
    """
    h, l, p = parse_ftd_type_spec(base_type_name)
    if h == CHAR_TYPE:
        return 'char' 
    if h == STRING_TYPE:
        return 'String'
    if h ==  NUMBER_TYPE or h == INT_TYPE:
        return 'Int32'
    if h == FLOAT_TYPE:
        return  'double'
    if h == WORD_TYPE:
        return  'Int16'
    if h == INT64_TYPE:
        return  'Int64'

def _get_csharpstore_marshal_spec(base_type_name):
    """
        <[MarshalAs(UnmanagedType.I4)]>
        <[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]>
    """
    template = "[MarshalAs(UnmanagedType.{0}{1})]"
    t1 = ''
    t2 = ''
    h, l, p = parse_ftd_type_spec(base_type_name)
    if h == CHAR_TYPE:
        t1 = 'I1' 
    if h == STRING_TYPE:
        t1 = 'ByValTStr' 
        t2 = ', SizeConst = %d' %(l+1)
    if h ==  NUMBER_TYPE or h == INT_TYPE:
        t1 = 'I4'
    if h == FLOAT_TYPE:
        t1 = 'R8'
    if h == WORD_TYPE:
        t1 =  'I2'
    if h == INT64_TYPE:
        t1 =  'I8'
    return template.format(t1, t2)


def _generate_static_string_class(t):
    lines = []
    template = load_template_file(single_static_string_template)
    td ={}
    lines = []
    raw_name = t.name
    if raw_name.startswith('FTD'):
        raw_name = raw_name[3:]
    for d in t.enum_value_dicts:
        entry_item_name = ''
        if 'enname' in d and len(d['enname']) > 0:
            entry_item_name = d['enname']
        else:
            entry_item_name = raw_name + '_' + d['name']

        lines.append('/// %s' % d['comment'])
        lines.append('public static readonly string %s = "%s";' %(entry_item_name, d['name']))
    td['static_class_name'] = raw_name + 's';
    td['item_lines'] = add_whitespaces('\n'.join(lines), 4)
    return template.format_map(td).split('\n')



def _generate_single_enum(t):
    lines = []
    entry_template = load_template_file(single_enum_template)
    td ={}
    lines = []
    raw_name = t.name
    if raw_name.startswith('FTD'):
        raw_name = raw_name[3:]
    quoter = ''
    converter = ''
    if t.base_type_name.startswith('FTDChar'):
        quoter = "'"
        converter = '(byte)'
    for d in t.enum_value_dicts:
        lines.append('[Description("%s")]' % d['comment'])
        item_name = raw_name +'_' + d['name']
        if 'enname' in d and len(d['enname']) > 0:
            item_name = d['enname']
        value = d['name']
        
        template = "{0} = {3}{2}{1}{2},"
        lines.append(template.format(item_name, value, quoter, converter))

    raw_name = t.name
    if raw_name.startswith('FTD'):
        raw_name = raw_name[3:]
    td['enum_type_name'] = raw_name
    if t.base_type_name.startswith('FTDChar'):
        td['int_type'] = 'byte'
    else:
        td['int_type'] = 'int'
    td['item_lines'] = add_whitespaces('\n'.join(lines),4)
    return entry_template.format_map(td).split('\n')