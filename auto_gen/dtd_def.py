CHAR_TYPE = 'FTDCharType'
FLOAT_TYPE = 'FTDFloatType'
INT_TYPE = 'FTDIntType'
INT64_TYPE = 'FTDInt64Type'
NUMBER_TYPE = 'FTDNumberType'
STRING_TYPE = 'FTDStringType'
WORD_TYPE = 'FTDWordType'

def is_base_type_name(type_name):
    return (type_name.startswith(CHAR_TYPE) 
            or type_name.startswith(FLOAT_TYPE)
            or type_name.startswith(INT_TYPE)
            or type_name.startswith(INT64_TYPE)
            or type_name.startswith(NUMBER_TYPE)
            or type_name.startswith(WORD_TYPE)
            or type_name.startswith(STRING_TYPE))

def parse_ftd_type_spec(basic_type_string):
    basic_t = ''
    size = 0
    precise = 0
    nodes = basic_type_string.split('<')
    basic_t = nodes[0].strip()
    if len(nodes) > 1:
        tail = nodes[1].replace('>', '')
        if tail.find(',') > 0:
            datas = tail.split(',')
            size = int(datas[0].strip())
            precise = int(datas[1].strip())
        else:
            size = int(tail.strip())
    return basic_t, size, precise

def get_basic_name_length_spec_pair_tuple(base_type_name):
    h, l, p = parse_ftd_type_spec(base_type_name)
    type_name = ''
    length_spec = ''
    if h == CHAR_TYPE or h == STRING_TYPE:
        type_name ='char'
    if h ==  NUMBER_TYPE or h == INT_TYPE:
        type_name = 'int'
    if h == FLOAT_TYPE:
        type_name = 'double'
    if h == WORD_TYPE:
        type_name = 'int16_t'
    if h == STRING_TYPE:
        length_spec = '[%d + 1]' % l
    if h == INT64_TYPE:
        type_name = 'int64_t'
    return (type_name, length_spec)

class TypeDTD:
    def __init__(self):
        self.base_type_name = ''
        self.name = ''
        self.length = 0
        self.precision = 0
        self.comment = ''
        self.enum_value_dicts = []

    def get_universal_enum_prefix(self, project_code):
        prefix = 'FTDC_'
        if len(project_code) > 0:
           prefix = '%s_FTDC_' % project_code.upper()
        i = 0
        name = self.name
        if name.startswith("FTD"):
            name = name[3:]
        for i in range(len(name)):
            if name[i].isupper():
                prefix += name[i]
        return prefix +'_'

    def get_universal_define_lines(self, project_code):

        lines = []

        basic_type_cpp, length_spec = get_basic_name_length_spec_pair_tuple(self.base_type_name)
        store_name = self.get_universal_cstore_name(project_code)
        
        lines.append('/' * 80)
        lines.append('///%s  %s' % (store_name, self.comment))
        lines.append('/' * 80)
        
        if len(self.enum_value_dicts) > 0:
            enum_prefix = self.get_universal_enum_prefix(project_code)
            comma =''
            h, l, p = parse_ftd_type_spec(self.base_type_name)
            if h == CHAR_TYPE:
                comma = "'"
            if h == STRING_TYPE:
                comma = '"'
            for d in self.enum_value_dicts:
                const_raw_name = d['enname'] if 'enname' in d else d['name']
                lines.append('///%s' % d['comment'])
                lines.append('#define {0}{1} {2}{3}{2}'.format(enum_prefix,const_raw_name,comma, d['name'] ))
                lines.append('')
                        
        template = "typedef {0} {1}{2};"
        lines.append(template.format(basic_type_cpp, store_name, length_spec))
        lines.append('')
        return lines

    def get_universal_cstore_name(self, project_code=''):
        #1 data_type_prefix
        #2 raw_type_name
        #3 data_type_posfix
        data_type_prefix = 'T%sFtdc' % project_code
        data_type_posfix = 'Type'
        raw_name = self.name[3:]
        return '%s%s%s' %(data_type_prefix,raw_name,data_type_posfix )








class ItemDTD:
    def __init__(self):
        self.name = ""
        self.comment = ""
        self.type_name = ""
        self.use_derivative_type = False
        self.base_type_name = ""

def get_universal_field_item_define_lines(field_item_dict, items, types, project_code):
    item_name = field_item_dict['name']
    item_comment = field_item_dict['comment']
    if item_name not in items:
        print("Item项[%s]未定义与类型的对应关系" % item_name)
        return None
    lines = []
    lines.append('///%s' % item_comment)
    item_entry = items[item_name]
    if item_entry.use_derivative_type:
        type_name = item_entry.type_name
        cstore_name = types[type_name].get_universal_cstore_name(project_code)
        lines.append('%s   %s;' %(cstore_name,item_name))
    else:
        basic_ctype, length_sepc = get_basic_name_length_spec_pair_tuple(item_entry.base_type_name)
        lines.append('%s %s%s;' %(basic_ctype, item_name, length_sepc))
    lines.append('')
    return lines

          

    

    
class FieldDTD:
    def __init__(self):
        self.name = ""
        self.fid = ""
        self.fid_value = 0
        self.comment = ""
        self.item_dicts = []

    def get_universal_struct_name(self, project_name):
        return 'C%sFtdc%s' %(project_name, self.name)


class TIdDTD:
    def __init__(self):
        self.name = ""
        self.value = ""

class FieldInfo:
    def __init__(self):
        self.name = ''
        self.max_occur = 0
        self.min_occur = 0
        self.comment = ''

    def use_vector(self):
        return self.max_occur > 99

    def use_field(self):
        return self.max_occur == 1 and self.min_occur == 1

    def use_smart_ptr(self):
        return self.max_occur == 1 and self.min_occur == 0


class PackageDTD:
    def __init__(self):
        self.name = ''
        self.tid = ''
        self.model = ''
        self.comment = ''
        self.fields = []
        self.same_as_id = False
