# -*- coding: utf-8 -*-
import os
import datetime
from io import BytesIO
from dtd_def import *
from file_util import *

package_struct_template_file = './templates/package_struct.template'

packages_include_template_file = './templates/packages.template'

package_cracker_template_file = './templates/package_crack.template'

include_all_headers = 'Packages.h'

package_cracer_header = 'PackageCracker.h'

def generate_package_struct(version, package, target_path, version_number, fields):
    template = load_template_file(package_struct_template_file)
    package_name_upper = package.name.upper()
    member_def_lines = []
    member_clear_lines = []
    member_merge_lines = []
    member_write_sections = []
    member_clone_lines = []
    for field_info in package.fields:
        field = fields[field_info.name]
        member_def_lines.append('///%s' % field_info.comment)
        member_def_lines.append(_format_member_def_line(field_info,field))

        member_clear_lines.append(_format_member_clear_lines(field_info,field))
        member_merge_lines.append(_format_member_merge_lines(field_info,field))
        member_write_sections.append(_format_member_write_lines(field_info, field))
        member_clone_lines.append(_format_member_clone_lines(field_info, field))

    d = {}
    d['version'] = version
    d['tid'] = package.tid
    d['package_name_upper'] = package_name_upper
    d['package_name'] = package.name
    d['member_def_lines'] = add_whitespaces('\n'.join(member_def_lines), 4)
    d['member_clear_lines'] = add_whitespaces('\n'.join(member_clear_lines), 8)
    d['member_merge_lines'] = add_whitespaces('\n'.join(member_merge_lines), 8)
    d['member_write_sections'] = add_whitespaces('\n'.join(member_write_sections), 8)
    d['member_clone_lines'] = add_whitespaces(''.join(member_clone_lines), 8)
    d['version_number'] = version_number
    d['package_comment'] = package.comment
    d['datetime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    if package.same_as_id:
        d['mode'] = 'FTD_MODE_'+ package.model.upper()
    else:
        if package.name.startswith('Req'):
            d['mode'] = 'FTD_MODE_' + package.model.upper() +'_REQUEST'
        else:
            d['mode'] = 'FTD_MODE_' + package.model.upper() + '_RESPONSE'
    target_fpath = '{0}/{1}/{2}.h'.format(target_path, version,package.name) 
    save_cpp_file(template.format_map(d), target_fpath)

def _format_member_def_line(field_info, field):
    f_struct_name = field.get_universal_struct_name('')
    fname = field.name
    vec_fmt = 'std::vector<{0}> {1}s;'
    field_fmt = '{0} {1};'
    ptr_fmt = '{0}Ptr p{1};'
    if field_info.use_vector():
        return vec_fmt.format(f_struct_name, fname[0].lower()+fname[1:])
    if field_info.use_field():
        return field_fmt.format(f_struct_name, fname[0].lower()+fname[1:])
    if field_info.use_smart_ptr():
        return ptr_fmt.format(f_struct_name,fname )


def _format_member_clear_lines(field_info, field):
    fname = field_info.name
    vec_fmt = '{0}s.clear();'
    field_fmt = 'memset(&{0}, 0, sizeof({1}));'
    ptr_fmt = 'p{0} = nullptr;'
    if field_info.use_vector():
        return vec_fmt.format(fname[0].lower()+fname[1:])
    if field_info.use_field():
        return field_fmt.format(fname[0].lower()+fname[1:], field.get_universal_struct_name(''))
    if field_info.use_smart_ptr():
        return ptr_fmt.format(fname)

def _format_member_merge_lines(field_info, field):
    univerl_name = field.get_universal_struct_name('')
    fname = field_info.name
    vec_fmt = """if (fid == FID_{0})
{{
	{1}s.push_back(field.{1});
	return true;
}}"""
    field_fmt = """if (fid == FID_{0})
{{
	memcpy(&{1}, &field.{1}, sizeof({2}));
	return true;
}}"""
    ptr_fmt = """if (fid == FID_{0})
{{
	if (p{0}.get() == nullptr)
	{{
		p{0} = {2}Ptr(new {2}());
	}}
	memcpy(p{0}.get(), &field.{1}, sizeof({2}));
	return true;
}}"""
    if field_info.use_vector():
        return vec_fmt.format(fname, fname[0].lower()+fname[1:])
    if field_info.use_field():
        return field_fmt.format(fname, fname[0].lower()+fname[1:],univerl_name)
    if field_info.use_smart_ptr():
        return ptr_fmt.format(fname, fname[0].lower()+fname[1:],univerl_name)

def _format_member_write_lines(field_info, field):
    univerl_name = field.get_universal_struct_name('')
    fname = field_info.name
    vec_fmt = """//std::vector<{2}> 
vecSize = {1}s.size();			
for (int i = 0; i < vecSize; i++)
{{
	{2}Helper::writeBuffer({1}s[i],
		fieldBuffer, fieldLen);
	if (MAX_FTDC_CONTENT_LENGTH - (nextWrite - ftdcBuffer) < FTDC_FIELD_HEADER_LENGTH + fieldLen)
	{{					
		ftdcHeader.contentLength = nextWrite - ftdcBuffer;
		ftdcHeader.fieldCount = writeFieldCount;
		headers.push_back(ftdcHeader);
		ftdcContents.push_back(std::string(ftdcBuffer, nextWrite - ftdcBuffer));
		memset(ftdcBuffer, 0, MAX_FTDC_LENGTH + 1);
		nextWrite = ftdcBuffer;
		writeFieldCount = 0;
	}}
	fieldHeader.fid = FID_{0};
	fieldHeader.fidLength = fieldLen;
	nextWrite += writeFtdcFieldHeader(fieldHeader, nextWrite);
	memcpy(nextWrite, fieldBuffer, fieldLen);
	nextWrite += fieldLen;
	writeFieldCount += 1;
}}
"""
    field_fmt = """//{2}
{2}Helper::writeBuffer({1},
	fieldBuffer, fieldLen);
if (MAX_FTDC_CONTENT_LENGTH - (nextWrite - ftdcBuffer) < FTDC_FIELD_HEADER_LENGTH + fieldLen)
{{
	ftdcHeader.contentLength = nextWrite - ftdcBuffer;
	ftdcHeader.fieldCount = writeFieldCount;
	headers.push_back(ftdcHeader);
	ftdcContents.push_back(std::string(ftdcBuffer, nextWrite - ftdcBuffer));
	memset(ftdcBuffer, 0, MAX_FTDC_LENGTH + 1);
	nextWrite = ftdcBuffer;
	writeFieldCount = 0;
}}
fieldHeader.fid = FID_{0};
fieldHeader.fidLength = fieldLen;
nextWrite += writeFtdcFieldHeader(fieldHeader, nextWrite);
memcpy(nextWrite, fieldBuffer, fieldLen);
nextWrite += fieldLen;
writeFieldCount += 1;
"""
    ptr_fmt = """//{0}
if(p{0}.get() != nullptr)
{{
	{2}Helper::writeBuffer(*(p{0}.get()),
		fieldBuffer, fieldLen);
	if (MAX_FTDC_CONTENT_LENGTH - (nextWrite - ftdcBuffer) < FTDC_FIELD_HEADER_LENGTH + fieldLen)
	{{
		ftdcHeader.contentLength = nextWrite - ftdcBuffer;
		ftdcHeader.fieldCount = writeFieldCount;
		headers.push_back(ftdcHeader);
		ftdcContents.push_back(std::string(ftdcBuffer, nextWrite - ftdcBuffer));
		memset(ftdcBuffer, 0, MAX_FTDC_LENGTH + 1);
		nextWrite = ftdcBuffer;
		writeFieldCount = 0;
	}}
	fieldHeader.fid = FID_{0};
	fieldHeader.fidLength = fieldLen;
	nextWrite += writeFtdcFieldHeader(fieldHeader, nextWrite);
	memcpy(nextWrite, fieldBuffer, fieldLen);
	nextWrite += fieldLen;
	writeFieldCount += 1;
}}
""" 
    if field_info.use_vector():
        return vec_fmt.format(fname, fname[0].lower()+fname[1:], univerl_name)
    if field_info.use_field():
        return field_fmt.format(fname, fname[0].lower()+fname[1:],univerl_name)
    if field_info.use_smart_ptr():
        return ptr_fmt.format(fname, fname[0].lower()+fname[1:],univerl_name)


def _format_member_clone_lines(field_info, field):
    univerl_name = field.get_universal_struct_name('')
    fname = field_info.name
    vec_fmt = """///std::vector<{2}> {1}s
for(unsigned int i = 0;i < {1}s.size(); i++)
{{
    pCopy->{1}s.push_back({1}s[i]);
}}
"""
    field_fmt = """///{2} {1}
memcpy(&(pCopy->{1}), &{1}, sizeof({2}));
"""
    ptr_fmt = """///{2}Ptr p{0}
if (p{0}.get())
{{
    pCopy->p{0} = {2}Ptr(new {2}());
    memcpy(pCopy->p{0}.get(),p{0}.get(),sizeof({2}));
}}
"""
    if field_info.use_vector():
        return vec_fmt.format(fname, fname[0].lower()+fname[1:], univerl_name)
    if field_info.use_field():
        return field_fmt.format(fname, fname[0].lower()+fname[1:],univerl_name)
    if field_info.use_smart_ptr():
        return ptr_fmt.format(fname, fname[0].lower()+fname[1:],univerl_name)



def generate_package_include(version, packages, target_path):
    template = load_template_file(packages_include_template_file)
    include_lines = []
    ptr_lines = []
    member_lines = []
    read_cases_lines = []
    retrieve_cases_lines = []
    include_template = '#include "{0}.h"'
    ptr_template = 'DECLARE_PTR({0})'
    member_template ='{0} {1};'
    read_template = """case(TID_{0}):
{{
    if({1}.mergeFtdcMessage(message))
    {{
        newArrivedPackage = &{1};       
    }}
    break;
}}"""
    read_template_dialog = """case(TID_{0}):
{{
    //bool parseRtn = false;
    if((header.ftdMode & FTD_REQUEST) == FTD_REQUEST && req{0}.mergeFtdcMessage(message))
    {{
        newArrivedPackage = &req{0};
        break;
    }}
    if((header.ftdMode & FTD_RESPONSE) == FTD_RESPONSE && rsp{0}.mergeFtdcMessage(message))
    {{
        newArrivedPackage = &rsp{0};
        break;
    }}
    break;
}}"""
    retrieve_template = """case(TID_{0}):
{{
    retrieveResult = (Package*)&{1};
    break;
}}"""
    retrieve_template_dialog = """case(TID_{0}):
{{
    if(m_isReceiveReq)
    {{
        retrieveResult = (Package*)&req{0};
    }}
    else
    {{
        retrieveResult = (Package*)&rsp{0};
    }}
    break;
}}"""
    tids = []
    for package in packages:
        var_name = package.name[0].lower() + package.name[1:]
        include_lines.append(include_template.format(package.name))
        ptr_lines.append(ptr_template.format(package.name))
        member_lines.append(member_template.format(package.name, var_name ))        
        tids.append([package.tid,package.model,package.same_as_id])
    
    tid_set = set()
    for tid in tids:
        if tid[0] in tid_set:
            continue
        tid_tag = tid[0].split('_')[1]
        if tid[2]:
            var_name = tid_tag[0].lower() + tid_tag[1:]
            read_cases_lines.append(read_template.format(tid_tag, var_name))
            retrieve_cases_lines.append(retrieve_template.format(tid_tag, var_name))
        else:
            read_cases_lines.append(read_template_dialog.format(tid_tag))
            retrieve_cases_lines.append(retrieve_template_dialog.format(tid_tag))
        tid_set.add(tid[0])


    d = {}
    d['version'] = version
    d['include_file_headers'] = '\n'.join(include_lines)
    d['declare_smart_ptr_lines'] = '\n'.join(ptr_lines)
    d['package_members'] = add_whitespaces('\n'.join(member_lines),4)
    d['read_cases'] = add_whitespaces('\n'.join(read_cases_lines), 12)
    d['retrieve_cases'] = add_whitespaces('\n'.join(retrieve_cases_lines),8)
    d['datetime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    target_fpath = '{0}/{1}/{2}'.format(target_path, version, include_all_headers) 
    save_cpp_file(template.format_map(d), target_fpath)


def generate_package_cracker(version, packages, target_path):
    template = load_template_file(package_cracker_template_file)
    pre_def_lines = []
    package_sections = []
    const_package_sections = []
    if_else_sections = []
    const_if_else_sections = []
    class_pre_def_template = 'struct {0};'
    default_on_const_package_section_template ="virtual void OnPackage( const {0}&, const SessionID&){{}}"
    default_on_package_section_template ="virtual void OnPackage( {0}&, const SessionID&){{}}"
    const_if_else_section_template = """if ( package.m_transactionId == {tid}{judge_expression})
    OnPackage((const {package_type}&)package, sessionID);
else"""
    if_else_section_template = """if ( package.m_transactionId == {tid}{judge_expression})
    OnPackage(({package_type}&)package, sessionID);
else"""
    for package in packages:
        pre_def_lines.append(class_pre_def_template.format(package.name))
        const_package_sections.append(default_on_const_package_section_template.format(package.name))
        package_sections.append(default_on_package_section_template.format(package.name))
        d ={}
        d['tid'] = package.tid
        d['package_type'] = package.name
        if package.same_as_id:
            d['judge_expression'] = ''
        else:
            if package.name.startswith('Req'):
                d['judge_expression'] = ' && package.m_mode == '+ 'FTD_MODE_' + package.model.upper() + '_REQUEST'
            else:
                d['judge_expression'] = ' && package.m_mode == ' + 'FTD_MODE_' + package.model.upper() + '_RESPONSE'
        if_else_sections.append(if_else_section_template.format_map(d))
        const_if_else_sections.append(const_if_else_section_template.format_map(d))


    d ={}
    d['version'] = version
    d['class_pre_def'] = '\n'.join(pre_def_lines)
    d['default_on_const_package_section'] = add_whitespaces('\n'.join(const_package_sections), 4)
    d['default_on_package_section'] = add_whitespaces('\n'.join(package_sections),4)
    d['const_if_else_sections'] = add_whitespaces('\n'.join(const_if_else_sections),8)
    d['if_else_sections'] = add_whitespaces('\n'.join(if_else_sections),8)
    d['datetime'] = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    target_fpath = '{0}/{1}/{2}'.format(target_path, version, package_cracer_header) 
    save_cpp_file(template.format_map(d), target_fpath)



    
    

