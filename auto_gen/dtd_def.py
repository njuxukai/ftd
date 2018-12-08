﻿def parse_ftd_type_spec(basic_type_string):
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

class TypeDTD:
    def __init__(self):
        self.base_type_name = ''
        self.name = ''
        self.length = 0
        self.precision = 0
        self.comment = ''
        self.enum_value_dicts = []
    
    def get_enum_prefix(self):
        name = self.name
        nodes = []
        if name.startswith("FTD"):
            nodes.append("FTD")
            name = name[3:]
        i = 0
        j = 1
        while i < len(name):
            while j < len(name) and name[j].islower() : 
                j += 1
            nodes.append(name[i:j].upper())
            i = j
            j += 1
        return '_'.join(nodes) +'_'


    def get_tftd_def(self):
        template = "typedef {0} T{1};"
        #return template.format(self.base_type_name.ljust(30), self.name)
        return template.format(self.base_type_name, self.name)

    def get_name_length_tuple(self):
        type_name = ''
        length_spec = ''
        h, l, p = parse_ftd_type_spec(self.base_type_name)
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
        #return template.format(type_name.ljust(30), name, length_spec)
        return (type_name, length_spec)
    
    def get_tt_def(self):
        template = 'typedef {0} TT{1}{2};';
        type_name = ''
        name = self.name
        length_spec = ''

        h, l, p = parse_ftd_type_spec(self.base_type_name)
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
        #return template.format(type_name.ljust(30), name, length_spec)
        return template.format(type_name, name, length_spec)


CHAR_TYPE = 'FTDCharType'
FLOAT_TYPE = 'FTDFloatType'
INT_TYPE = 'FTDIntType'
NUMBER_TYPE = 'FTDNumberType'
STRING_TYPE = 'FTDStringType'
WORD_TYPE = 'FTDWordType'




class ItemDTD:
    def __init__(self):
        self.name = ""
        self.comment = ""
        self.type_name = ""

    def get_item_tt_name(self):
        return 'TTItem' + self.name

    def get_item_t_name(self):
        return 'TItem' + self.name

    def get_t_item_define(self):
        template = 'typedef {0} TItem{1};';
        type_name = self.type_name;
        name = self.name;
        h, l, p = parse_ftd_type_spec(self.type_name)
        if (h != CHAR_TYPE and h != FLOAT_TYPE 
            and h != NUMBER_TYPE and h != STRING_TYPE
            and h != WORD_TYPE and h!= INT_TYPE): 
            type_name = 'T' + self.type_name
        return template.format(type_name.ljust(30), name)

        

        

    def get_tt_item_define(self):
        template = 'typedef {0} TTItem{1}{2};';
        type_name = '';
        name = self.name;
        length_spec ='';
        h, l, p = parse_ftd_type_spec(self.type_name)
        if (h != CHAR_TYPE and h != FLOAT_TYPE 
            and h != NUMBER_TYPE and h != STRING_TYPE
            and h != WORD_TYPE and h!= INT_TYPE): 
            type_name =  'TT' + self.type_name
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
        return template.format(type_name.ljust(30), name, length_spec)

    def get_host_tt_type_name(self, data_type_prefix):
        return '%s%sType' % (data_type_prefix,self.name)

    def get_host_tt_item_define(self, derative_type_dict, data_type_prefix, project_code):
        lines = []
        lines.append('/' * 80)
        lines.append('///%s %s' %(self.get_host_tt_type_name(data_type_prefix), self.comment))
        lines.append('/' * 80)
        template = 'typedef {0} {1}{2};';
        type_name = '';
        name = self.name;
        length_spec ='';
        h, l, p = parse_ftd_type_spec(self.type_name)
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
        if (h != CHAR_TYPE and h != FLOAT_TYPE 
            and h != NUMBER_TYPE and h != STRING_TYPE
            and h != WORD_TYPE and h!= INT_TYPE): 
            if self.type_name not in derative_type_dict:
                print("类型[%s]的关联衍生类型[%s]找不到定义")
                return
            dt = derative_type_dict[self.type_name]
            type_name, length_spec = dt.get_name_length_tuple()
            if len(dt.enum_value_dicts) > 0:
                const_prefix = self.get_host_enum_prefix(project_code)
                comma =''
                h, l, p = parse_ftd_type_spec(dt.base_type_name)
                if h == CHAR_TYPE:
                    comma = "'"
                if h == STRING_TYPE:
                    comma = '"'
                for  d in dt.enum_value_dicts:
                    const_name = const_prefix + (d['enname'] if 'enname' in d else d['name'])
                    lines.append('///%s %s' % (const_name, d['comment']))
                    lines.append('#define {0} {2}{1}{2}'.format(const_name, d['name'], comma))
                    lines.append('')
        lines.append(template.format(type_name, self.get_host_tt_type_name(data_type_prefix), length_spec))
        lines.append('')
        return lines

    def get_host_enum_prefix(self, project_code):
        prefix = '%s_FTDC_' % project_code.upper()
        i = 0
        for i in range(len(self.name)):
            if self.name[i].isupper():
                prefix += self.name[i]
        return prefix +'_'
        

          

    

    
class FieldDTD:
    def __init__(self):
        self.name = ""
        self.fid = ""
        self.fid_value = 0
        self.comment = ""
        self.item_dicts = []


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
