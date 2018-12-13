# -*- coding: utf-8 -*- 
import os

path = '../spec/'
files = ['ftd_3_0.xml',]
target_path = '../ftd/'
host_targe_path = '../include/xcp'
testcase_target_path = '../test_unit'

project_code = 'Xcp'
data_type_prefix = 'T%sFtdc' % project_code
data_type_file_name = '%sFtdcUserApiDataType.h' % project_code

struct_type_prefix = 'C%sFtdc' % project_code
struct_file_name = '%sFtdcUserApiStruct.h' % project_code

import gen_type_df
import gen_item_df
import gen_field_df
import gen_package_df
import gen_id_def
import gen_host_data_type
import gen_host_struct
import gen_testcase

from dtd_parse import *

def generate_ftd(path, file, target_path):
    types,items,fields, packages, tids = load_dtd_file_fields(path, file)
    version = file.split('.')[0]
    nodes = version.split('_')
    version_number = nodes[1]
    version = ''.join(nodes).upper()
    ver_path = '%s/%s' %(target_path, version)
    if not os.path.exists(ver_path):
        os.makedirs(ver_path)
    #gen_type_df.generate_type_df(version, list(types.values()), target_path)
    gen_type_df.generate_ftd_data_type_file(types, version, target_path)
    #gen_item_df.generate_item_df(version, list(items.values()), target_path)
    gen_id_def.generate_id_df(version, list(tids.values()), list(fields.values()), target_path, version_number)
    for field in fields.values():
        gen_field_df.generate_field_struct(version, field, target_path, items, types)
    gen_field_df.generate_fields_include(version,
           list(fields.values()), target_path)
    for package in packages.values():
        gen_package_df.generate_package_struct(version, package, target_path, version_number)
    gen_package_df.generate_package_include(version, list(packages.values()), target_path)
    gen_package_df.generate_package_cracker(version, list(packages.values()), target_path)
    
def generate_host_files(path, file, target_path):
    types,items,fields, packages, tids = load_dtd_file_fields(path, file)
    data_type_fpath = '%s/%s' %(target_path, data_type_file_name)
    struct_fpath = '%s/%s' %(target_path, struct_file_name)
    gen_host_data_type.generate_host_data_type_file(types, items, project_code, 
                                                    data_type_prefix, data_type_fpath)
    gen_host_struct.generate_host_struct_file(fields, items, types, project_code,data_type_file_name,
                                              data_type_prefix, struct_type_prefix, struct_fpath)

def generate_testcase(path, file, target_path):
    version = file.split('.')[0]
    nodes = version.split('_')
    version_number = nodes[1]
    version = ''.join(nodes).upper()
    types,items,fields, packages, tids = load_dtd_file_fields(path, file)
    gen_testcase.generate_struct_validation(fields, version, struct_type_prefix, target_path)
    pass

if __name__ == "__main__":
    for file in files:
        generate_ftd(path, file, target_path)
        generate_host_files(path, file, host_targe_path)
        generate_testcase(path,file, testcase_target_path)
        #generate_ftd(path, file, target_path2)