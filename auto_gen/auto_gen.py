# -*- coding: utf-8 -*- 
import os

path = '../spec/'
files = ['ftd_3_0.xml',]
target_path = '../ftd/'
host_targe_path = '../xcptraderapi/'
host_targe_path2 = '../xcpmduserapi/'
host_csharp_target_path = '../XcpApiCSharp/'
testcase_target_path = '../test_unit/'
project_code = 'Xcp'


import gen_types_def
import gen_fields_def
import gen_packages_def
import gen_id_def
import gen_host_types_def
import gen_host_fields_def
import gen_host_csharp_fields_def
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
    gen_types_def.generate_ftd_data_type_file(types, version, target_path)
    gen_id_def.generate_id_df(version, list(tids.values()), list(fields.values()), target_path, version_number)
    for field in fields.values():
        gen_fields_def.generate_field_struct(version, field, target_path, items, types)
    gen_fields_def.generate_fields_include(version,
           list(fields.values()), target_path)
    for package in packages.values():
        gen_packages_def.generate_package_struct(version, package, target_path, version_number, fields)
    gen_packages_def.generate_package_include(version, list(packages.values()), target_path)
    gen_packages_def.generate_package_cracker(version, list(packages.values()), target_path)
    
def generate_host_files(path, file, target_path):
    data_type_file_name = '%sFtdcUserApiDataType.h' % project_code
    types,items,fields, packages, tids = load_dtd_file_fields(path, file)
    gen_host_types_def.generate_host_data_type_file(types, project_code, 
                                                     target_path)
    gen_host_fields_def.generate_host_struct_file(fields, items, types, project_code,data_type_file_name, target_path)

def generate_csharp_files(path, file, target_path):
    types,items,fields, packages, tids = load_dtd_file_fields(path, file)
    gen_host_csharp_fields_def.generate_host_cs_struct_file(fields, items, types, project_code, target_path)
    gen_host_csharp_fields_def.generate_host_cs_enum_file(types, project_code, target_path)


def generate_testcase(path, file, target_path):
    struct_type_prefix = 'C%sFtdc' % project_code
    version = file.split('.')[0]
    nodes = version.split('_')
    version_number = nodes[1]
    version = ''.join(nodes).upper()
    types,items,fields, packages, tids = load_dtd_file_fields(path, file)
    gen_testcase.generate_struct_validation(fields, version, project_code, target_path)
    pass

if __name__ == "__main__":
    for file in files:
        generate_ftd(path, file, target_path)
        generate_host_files(path, file, host_targe_path)
        generate_host_files(path, file, host_targe_path2)
        generate_testcase(path,file, testcase_target_path)
        generate_csharp_files(path, file, host_csharp_target_path)
        #generate_ftd(path, file, target_path2)