import object_xml_parser

import os
from datetime import datetime
from prettytable import PrettyTable
from prettytable import PLAIN_COLUMNS
from optparse import OptionParser

PLACE_IF_DEF_DIRECTIVE = "<<<0>>>"
PLACE_CLASS_NAME = "<<<1>>>"
PLACE_RESOURCES_ENUM = "<<<2>>>"
PLACE_RESOURCES_MAP = "<<<3>>>"
PLACE_FOLDER = "<<<4>>>"
PLACE_RESOURCES_INIT = "<<<5>>>"

TYPE_REGISTRY = "WppRegistry"
TYPE_OPERATION = "ResOp"
TYPE_OBJECT = "Object"
TYPE_OBJECT_INFO = "ObjectInfo"
TYPE_RESOURCE = "Resource"
TYPE_INSTANCE = "Instance"
TYPE_I_SUBJECT = "InstSubject"

TYPE_1 = f"{TYPE_OPERATION}::TYPE"
TYPE_2 = "const ResLink"

LOG_FUNC_CUSTOM = "WPP_LOGD_ARG"

STOP_STRING_CNFG_CMK = ["# The end of the options of the mandatory objects.",
                        "# The end of the options of the optional objects."]
STOP_STRING_OBJ_ID = ["/* The end of the IDs of the mandatory objects. */",
                      "/* The end of the IDs of the optional objects. */"]
STOP_STRING_REG_PRT = ["/* The end of the prototypes of the mandatory objects. */",
                       "/* The end of the prototypes of the optional objects. */"]
STOP_STRING_REG_INCL = ["/* The end of the includes of the mandatory objects. */",
                        "/* The end of the includes of the optional objects. */"]

FILE_TYPE_TAG = "File type:"
FILE_TYPE_OBJ_IMPL_H = "FILE_TYPE_OBJ_IMPL_H"
FILE_TYPE_OBJ_IMPL_CPP = "FILE_TYPE_OBJ_IMPL_CPP"
FILE_TYPE_OBJ_CFG = "FILE_TYPE_OBJ_CFG"
FILE_TYPE_OBJ_INFO = "FILE_TYPE_OBJ_INFO"
FILE_TYPE_OBJ_CMAKE = "FILE_TYPE_OBJ_CMAKE"
FILE_TYPES = [FILE_TYPE_OBJ_IMPL_H, FILE_TYPE_OBJ_IMPL_CPP, FILE_TYPE_OBJ_CFG, FILE_TYPE_OBJ_INFO, FILE_TYPE_OBJ_CMAKE]

MAIN_H_COMMENT = \
    f"""/*\n""" \
    f""" * {FILE_TYPE_TAG} {FILE_TYPE_OBJ_IMPL_H}  \n""" \
    f""" * Generated on: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n""" \
    f""" * Author: valentin\n""" \
    f""" * Generated by: Vadimzakreva\n""" \
    f""" */\n\n"""

MAIN_CPP_COMMENT = \
    f"""/*\n""" \
    f""" * {FILE_TYPE_TAG} {FILE_TYPE_OBJ_IMPL_CPP}\n""" \
    f""" * Generated on: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n""" \
    f""" * Author: valentin\n""" \
    f""" * Generated by: Vadimzakreva\n""" \
    f""" */\n\n"""

CONFIG_COMMENT = \
    f"""/*\n""" \
    f""" * {FILE_TYPE_TAG} {FILE_TYPE_OBJ_CFG}\n""" \
    f""" * Generated on: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n""" \
    f""" * Author: valentin\n""" \
    f""" * Generated by: Vadimzakreva\n""" \
    f""" */\n\n"""

INFO_COMMENT = \
    f"""/*\n""" \
    f""" * {FILE_TYPE_TAG} {FILE_TYPE_OBJ_INFO}\n""" \
    f""" * Generated on: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n""" \
    f""" * Author: valentin\n""" \
    f""" * Generated by: Vadimzakreva\n""" \
    f""" */\n\n"""

CMAKE_COMMENT = \
    f"""# {FILE_TYPE_TAG} {FILE_TYPE_OBJ_CMAKE}\n""" \
    f"""# Generated on: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n""" \
    f"""# Author: valentin\n""" \
    f"""# Generated by: Vadimzakreva\n\n""" \

PREFIX_H = \
    f"""#ifndef WPP_{PLACE_IF_DEF_DIRECTIVE}_H\n""" \
    f"""#define WPP_{PLACE_IF_DEF_DIRECTIVE}_H\n\n""" \
    f"""#include "{PLACE_CLASS_NAME}Config.h"\n""" \
    f"""#include "{PLACE_CLASS_NAME}Info.h"\n""" \
    f"""#include "{TYPE_INSTANCE}.h"\n""" \
    f"""#include "{TYPE_I_SUBJECT}.h"\n\n""" \
    f"""/* --------------- Сode_h block 0 start --------------- */\n""" \
    f"""/* --------------- Сode_h block 0 end --------------- */\n\n""" \
    f"""namespace wpp {{\n\nclass {PLACE_CLASS_NAME} : """ \
    f"""public {TYPE_INSTANCE}, public {TYPE_I_SUBJECT}<{PLACE_CLASS_NAME}> {{\n"""

PUBLIC_ENUM_H = \
    f"""public:\n\tenum ID: ID_T {{\n{PLACE_RESOURCES_ENUM}\t}};\n\n""" \
    f"""\t/* --------------- Code_h block 1 start --------------- */\n""" \
    f"""\t/* --------------- Code_h block 1 end --------------- */\n\n""" \

PUBLIC_CONSTRUCTOR_H = \
    f"""public:\n\t{PLACE_CLASS_NAME}(lwm2m_context_t &context, const OBJ_LINK_T &id);\n\t""" \
    f"""~{PLACE_CLASS_NAME}();\n\n""" \
    f"""\t/* --------------- Code_h block 2 start --------------- */\n""" \
    f"""\t/* --------------- Code_h block 2 end --------------- */\n\n"""

I_INSTANCE_IMPLEMENTATIONS = \
    f"""/* --------------- {TYPE_INSTANCE} implementation part --------------- */\n"""

I_INSTANCE_IMPLEMENTATIONS_H = \
    f"""protected:\n\t{I_INSTANCE_IMPLEMENTATIONS}\t/* \n\t""" \
    f""" * Returns Resource object if it is exist\n\t */\n\t{TYPE_RESOURCE} * getResource(ID_T id) override;\n\t""" \
    f"""/*\n\t * Returns list with available resources\n\t */\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> getResourcesList() override;\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> getResourcesList(const {TYPE_OPERATION}& filter) override;\n\t/*\n\t""" \
    f""" * Returns list with available instantiated resources\n\t */\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> getInstantiatedResourcesList() override;\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> getInstantiatedResourcesList(const {TYPE_OPERATION}& filter) override;\n\t""" \
    f"""/*\n\t * Reset all resources values and internal state to default.\n\t */\n\t""" \
    f"""void setDefaultState() override;\n\t/*\n\t""" \
    f""" * Handles information about resource operation that made server\n\t */\n\t""" \
    f"""void serverOperationNotifier({TYPE_1} type, {TYPE_2} &resId) override;\n\t/*\n\t""" \
    f""" * Handles information about resource operation that made user\n\t */\n\t""" \
    f"""void userOperationNotifier({TYPE_1} type, {TYPE_2} &resId) override;"""

CLASS_PRIVATE_METHODS_H = \
    f"""\n\nprivate:\n\t/* --------------- Class private methods --------------- */\n\t""" \
    f"""/*\n\t * Initialize resources with default values\n\t""" \
    f""" * Resource always must have at least one instance.\n\t""" \
    f""" * Note: From server side, empty resource == undefined resource.\n\t */\t\n\t""" \
    f"""void resourcesInit();\n\t\n""" \
    f"""\t/* --------------- Code_h block 3 start --------------- */\n""" \
    f"""\t/* --------------- Code_h block 3 end --------------- */\n\n""" \
    f"""private:\n\tstd::unordered_map<ID_T, {TYPE_RESOURCE}> _resources = {{\n\t""" \
    f"""\t// KEY   VALUE\n{PLACE_RESOURCES_MAP}\n\t}};\n\n""" \
    f"""\t/* --------------- Code_h block 4 start --------------- */\n""" \
    f"""\t/* --------------- Code_h block 4 end --------------- */\n""" \
    f"""}};\n\n""" \
    f"""}} /* namespace wpp */\n\n#endif /* WPP_{PLACE_IF_DEF_DIRECTIVE}_H */\n"""

PREFIX_CPP = \
    f"""#include "{PLACE_FOLDER}/{PLACE_CLASS_NAME}.h"\n\n""" \
    f"""#include <unordered_map>\n""" \
    f"""#include <iostream>\n\n""" \
    f"""#include "{TYPE_RESOURCE}.h"\n""" \
    f"""#include "{TYPE_OPERATION}.h"\n""" \
    f"""#include "types.h"\n""" \
    f"""#include "WppLogs.h"\n\n""" \
    f"""/* --------------- Code_cpp block 0 start --------------- */\n""" \
    f"""/* --------------- Code_cpp block 0 end --------------- */\n\n""" \
    f"""#define TAG "{PLACE_CLASS_NAME}"\n\n""" \
    f"""namespace wpp {{\n\n"""

PUBLIC_CONSTRUCTOR_CPP = \
    f"""{PLACE_CLASS_NAME}::{PLACE_CLASS_NAME}(lwm2m_context_t &context, const OBJ_LINK_T &id): {TYPE_INSTANCE}(context, id) {{\n\n""" \
    f"""\t/* --------------- Code_cpp block 1 start --------------- */\n""" \
    f"""\t/* --------------- Code_cpp block 1 end --------------- */\n""" \
    f"""\n\tresourcesInit();\n\n""" \
    f"""\t/* --------------- Code_cpp block 2 start --------------- */\n""" \
    f"""\t/* --------------- Code_cpp block 2 end --------------- */\n""" \
    f"""}}\n\n"""

PUBLIC_DESTRUCTOR_CPP = \
    f"""{PLACE_CLASS_NAME}::~{PLACE_CLASS_NAME}() {{\n""" \
    f"""\t/* --------------- Code_cpp block 3 start --------------- */\n""" \
    f"""\t/* --------------- Code_cpp block 3 end --------------- */\n""" \
    f"""}}\n\n"""

CLASS_PRIVATE_METHODS_CPP = \
    f"""{PLACE_RESOURCES_INIT}""" \
    f"""/* --------------- Code_cpp block 10 start --------------- */\n""" \
    f"""/* --------------- Code_cpp block 10 end --------------- */\n\n"""

FUNC_GET_RESOURCE_T = \
    f"""{TYPE_RESOURCE} * {PLACE_CLASS_NAME}::getResource(ID_T id) {{\n\t""" \
    f"""// Check if resource ID is valid\n\t""" \
    f"""if (_resources.find(id) == _resources.end()) return NULL;\n\t""" \
    f"""return &_resources[id];\n}}\n\n"""

FUNC_GET_RESOURCE_LIST = \
    f"""std::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getResourcesList() {{\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""for (auto &pair : _resources) {{\n\t""" \
    f"""\tlist.push_back(&pair.second);\n\t}}\n\t""" \
    f"""return list;\n}}\n\n"""

FUNC_GET_RESOURCE_LIST_P = \
    f"""std::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getResourcesList(const {TYPE_OPERATION}& filter) {{\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""for (auto &pair : _resources) {{\n\t""" \
    f"""\tif (filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);\n\t}}\n\t""" \
    f"""return list;\n}}\n\n"""

FUNC_GET_INSTANTIATED_LIST = \
    f"""std::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getInstantiatedResourcesList() {{\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""for (auto &pair : _resources) {{\n\t""" \
    f"""\tif (!pair.second.isEmpty()) list.push_back(&pair.second);\n\t}}\n\t""" \
    f"""return list;\n}}\n\n"""

FUNC_GET_INSTANTIATED_LIST_P = \
    f"""std::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getInstantiatedResourcesList(const {TYPE_OPERATION}& filter) {{\n\t""" \
    f"""std::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""for (auto &pair : _resources) {{\n\t""" \
    f"""\tif (!pair.second.isEmpty() && filter.isCompatible(pair.second.getOperation())) """ \
    f"""list.push_back(&pair.second);\n\t}}\n\treturn list;\n}}\n\n"""

FUNC_CLEAR = \
    f"""\tvoid {PLACE_CLASS_NAME}::clear() {{\n\t""" \
    f"""\t/* --------------- Code_cpp block 4 start --------------- */\n\t""" \
    f"""\t/* --------------- Code_cpp block 4 end --------------- */\n\n\t""" \
    f"""\tfor (auto &pair : _resources) pair.second.clear();\n\t""" \
    f"""\tresourcesInit();\n\n\t""" \
    f"""\t/* --------------- Code_cpp block 5 start --------------- */\n\t""" \
    f"""\t/* --------------- Code_cpp block 5 end --------------- */\n\t""" \
    f"""}}\n\n"""

FUNC_SET_DEF_STATE = \
    f"""void {PLACE_CLASS_NAME}::setDefaultState() {{\n\t""" \
    f"""/* --------------- Code_cpp block 4 start --------------- */\n\t""" \
    f"""/* --------------- Code_cpp block 4 end --------------- */\n\n\t""" \
    f"""for (auto &pair : _resources) pair.second.clear();\n\t""" \
    f"""resourcesInit();\n\n\t""" \
    f"""/* --------------- Code_cpp block 5 start --------------- */\n\t""" \
    f"""/* --------------- Code_cpp block 5 end --------------- */\n""" \
    f"""}}\n\n"""

class ObjectGenerator:
    """Add some comments here"""

    def __init__(self, xml_file, xml_url):

        xp = object_xml_parser.ObjectXmlParser(xml_file=xml_file, xml_url=xml_url)
        obj_dict, res_list = xp.parse_xml()
        obj_names = xp.create_metadata()

        self.meta_object = obj_dict
        self.meta_resources = res_list
        self.object_names = obj_names

    def get_folder_path(self):
        return self.object_names["obj_name_folder"]

    def parse_operation(self, xml_operation):
        operation = f"{TYPE_OPERATION}({TYPE_OPERATION}::"
        match xml_operation:
            case "E":
                operation += "EXECUTE"
            case "R":
                operation += "READ"
            case "W":
                operation += "WRITE"
            case "RW":
                operation = f"{TYPE_OPERATION}({TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
            case default:
                operation = f"{TYPE_OPERATION}({TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
        operation += "),"
        return operation

    def parse_resource_data_type(self, xml_type):
        resource_type = "TYPE_ID::"
        match xml_type:
            case "INTEGER":
                resource_type += "INT"
            case "UNSIGNED_INTEGER":
                resource_type += "UINT"
            case "BOOLEAN":
                resource_type += "BOOL"
            case "STRING":
                resource_type += "STRING"
            case "EXECUTE":
                resource_type += "EXECUTE"
            case "OPAQUE":
                resource_type += "OPAQUE"
            case "?":
                resource_type += "FLOAT"            # TODO: check case
            case "OBJLNK":
                resource_type += "OBJ_LINK"
            case "TIME":
                resource_type += "TIME"
            case "?":
                resource_type += "CORE_LINK"        # TODO: check case
            case default:
                resource_type += "EXECUTE"          # TODO: check case

        return resource_type

    def parse_type(self, xml_type):
        resource_type = ""
        match xml_type:
            case "INTEGER":
                resource_type += "INT_T"
            case "UNSIGNED_INTEGER":
                resource_type += "UINT_T"
            case "BOOLEAN":
                resource_type += "BOOL_T"
            case "STRING":
                resource_type += "STRING_T"
            case "EXECUTE":
                resource_type += "EXECUTE_T"
            case "OPAQUE":
                resource_type += "OPAQUE_T"
            case "FLOAT":
                resource_type += "FLOAT_T"          # TODO: check case
            case "OBJLNK":
                resource_type += "OBJ_LINK_T"
            case "TIME":
                resource_type += "TIME_T"
            case "CORE_LINK":
                resource_type += "CORE_LINK_T"      # TODO: check case
            case default:
                resource_type += "NONE"             # TODO: check case

        return resource_type

    def get_map_of_resources(self, resources_list_xml):
        resources_enum = ""
        x = PrettyTable()
        x.set_style(PLAIN_COLUMNS)
        x.header = False
        x.align = "l"

        for resource_xml in resources_list_xml:
            resource_name = resource_xml['Name']
            resource_id = resource_xml['ID']
            # postfix = "M" if resource_xml['Mandatory'] == "MANDATORY" else "O"

            # fill the Resources' enum:
            resource_define = resource_xml['Define']
            resource = f"{resource_name}_{resource_id}"            

            if resource_xml['Mandatory'] != "MANDATORY":
                resources_enum += f"\t\t#if {resource_define}\n"
            resources_enum += f"\t\t{resource} = {resource_xml['ID']},\n"
            if resource_xml['Mandatory'] != "MANDATORY":
                resources_enum += f"\t\t#endif\n"

            # fill the unordered_map<ID_T, Resource> table:
            if resource_xml['Mandatory'] != "MANDATORY":
                x.add_row([f"*TAB*#if {resource_define}", "", "", "", "", ""])

            x.add_row([f"*TAB*{{{resource},",
                       f"{{{resource},",
                       self.parse_operation(resource_xml['Operations']),
                       f"IS_SINGLE::{resource_xml['MultipleInstances']},",
                       f"IS_MANDATORY::{resource_xml['Mandatory']},",
                       f"{self.parse_resource_data_type(resource_xml['Type'])} }}}},"])

            if resource_xml['Mandatory'] != "MANDATORY":
                x.add_row([f"*TAB*#endif", "", "", "", "", ""])

        resources_map = str(x).replace("*TAB*", "\t\t")

        return resources_enum, resources_map

    def get_content_resourcesInit_f(self, resources_list_xml):
        content = f"""void {PLACE_CLASS_NAME}::resourcesInit() {{\n""" \
                  f"""\t/* --------------- Code_cpp block 9 start --------------- */\n"""
        for resource in resources_list_xml:
            if resource["Mandatory"] == "MANDATORY":
                # content += f"""\t\t#if {resource["Name"]}_{resource["Mandatory"]}\n\t"""
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].set( /* TODO */ );\n"
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].setDataVerifier( /* TODO */ );\n\n"
                # content += f"\t\t#endif\n\n"
            if resource["Mandatory"] == "OPTIONAL":
                content += f"""\t#if {resource["Define"]}\n"""
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].set( /* TODO */ );\n"
                content += f"\t_resources[{resource['Name']}_{resource['ID']}].setDataVerifier( /* TODO */ );\n"
                content += f"\t#endif\n\n"

        return content + f"""\t/* --------------- Code_cpp block 9 end --------------- */\n}}\n\n"""

    def create_log_string(self, text: str, arguments: list, is_std, pattern: str = "%d"):
        if not isinstance(text, str) or not isinstance(arguments, list):
            raise TypeError

        if len(pattern) != text.count(pattern):
            return ""

        if is_std:
            for argument in arguments:
                text = text.replace(pattern, f"\" << {argument} << \"", 1)
            return f"""std::cout << "{text}" << std::endl;"""

        else:
            text = '"' + text + '"'
            for argument in arguments:
                text += ', ' + argument
            return f"""{LOG_FUNC_CUSTOM}(TAG, {text});"""

    def get_content_serverOperationNotifier(self, param_1, param_2):
        cases = ["READ", "WRITE", "EXECUTE", "DISCOVER", "DELETE"]
        base = \
            f"""void {PLACE_CLASS_NAME}::serverOperationNotifier({param_1} type, {param_2} &resId) {{\n""" \
            f"""\t/* --------------- Code_cpp block 6 start --------------- */\n""" \
            f"""\t/* --------------- Code_cpp block 6 end --------------- */\n""" \
            f"""\n\tobserverNotify(*this, resId, type);\n\n""" \
            f"""\t/* --------------- Code_cpp block 7 start --------------- */\n""" \
            f"""\tswitch (type) {{\n\t"""
        for case in cases:
            base += f"""case {TYPE_OPERATION}::{case}:\n\t\t{self.create_log_string(
                f"Server {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\tbreak;\n\t"""
        return f"""{base}default: break;\n\t}}\n\t""" \
               f"""/* --------------- Code_cpp block 7 end --------------- */\n}}\n\n"""

    def get_content_userOperationNotifier(self, param_1, param_2):
        cases = ["READ", "WRITE", "DELETE"]
        prefix = \
            f"""void {PLACE_CLASS_NAME}::userOperationNotifier({param_1} type, {param_2} &resId) {{\n""" \
            f"""\t/* --------------- Code_cpp block 8 start --------------- */\n""" \
            f"""\tswitch (type) {{\n\t"""
        for case in cases:
            prefix += f"""case {TYPE_OPERATION}::{case}:\n\t\t{self.create_log_string(
                f"User {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\tbreak;\n\t"""
        postfix = f"""default: break;\n\t}}\n""" \
                  f"""\t/* --------------- Code_cpp block 8 end --------------- */\n}}\n\n"""
        return prefix + postfix

    def generate_content_header(self):
        resources_enum, resources_map = self.get_map_of_resources(self.meta_resources)
        code_header = (MAIN_H_COMMENT +
                       PREFIX_H +
                       PUBLIC_ENUM_H +
                       PUBLIC_CONSTRUCTOR_H +
                       I_INSTANCE_IMPLEMENTATIONS_H +
                       CLASS_PRIVATE_METHODS_H +
                       "")
        code_header = code_header.replace(PLACE_IF_DEF_DIRECTIVE, self.object_names["obj_name_folder"].upper())
        code_header = code_header.replace(PLACE_CLASS_NAME, self.object_names["obj_name_class"])
        code_header = code_header.replace(PLACE_RESOURCES_ENUM, resources_enum)
        code_header = code_header.replace(PLACE_RESOURCES_MAP, resources_map)

        return code_header

    def generate_content_cpp(self):
        code_cpp = (MAIN_CPP_COMMENT +
                    PREFIX_CPP +
                    PUBLIC_CONSTRUCTOR_CPP +
                    PUBLIC_DESTRUCTOR_CPP +
                    FUNC_GET_RESOURCE_T +
                    FUNC_GET_RESOURCE_LIST +
                    FUNC_GET_RESOURCE_LIST_P +
                    FUNC_GET_INSTANTIATED_LIST +
                    FUNC_GET_INSTANTIATED_LIST_P +
                    # FUNC_CLEAR +
                    FUNC_SET_DEF_STATE +
                    self.get_content_serverOperationNotifier(TYPE_1, TYPE_2) +
                    self.get_content_userOperationNotifier(TYPE_1, TYPE_2) +
                    CLASS_PRIVATE_METHODS_CPP +
                    "} /* namespace wpp */\n")
        code_cpp = code_cpp.replace(PLACE_RESOURCES_INIT, self.get_content_resourcesInit_f(self.meta_resources))
        code_cpp = code_cpp.replace(PLACE_CLASS_NAME, self.object_names["obj_name_class"])
        code_cpp = code_cpp.replace(PLACE_FOLDER, self.object_names["obj_name_folder"])

        return code_cpp

    def generate_content_cmake_list(self):
        name_class = self.object_names["obj_name_class"]
        name_def = self.object_names["obj_name_define"]

        main_line = \
            f"""set(WPP_SOURCES ${{WPP_SOURCES}} ${{CMAKE_CURRENT_SOURCE_DIR}}/{name_class}.cpp)\n\n""" \
                    f"""\t# --------------- CMake block 0 start ---------------\n""" \
                    f"""\t# --------------- CMake block 0 end ---------------\n\n""" \
                    f"""\t# Update WPP_SOURCES variable from parent scope.\n""" \
                    f"""\tset(WPP_SOURCES ${{WPP_SOURCES}} PARENT_SCOPE)\n"""

        return CMAKE_COMMENT + \
               f"""if({name_def})\n\t""" \
               f"""# Update SOURCES variable from parent scope.\n\t""" \
               f"""{main_line}\nendif()"""

    def generate_content_info_header(self):
        if_not_def = f"""WPP_{self.object_names["obj_name_up_underline"]}_INFO_H"""
        is_multiple = "MULTIPLE" if self.meta_object["is_multiple"] else "SINGLE"
        is_mandatory = "MANDATORY" if self.meta_object["is_mandatory"] else "OPTIONAL"

        content = \
            INFO_COMMENT + \
            f"""#ifndef {if_not_def}\n""" \
            f"""#define {if_not_def}\n\n""" \
            f"""#include "{TYPE_OBJECT_INFO}.h"\n\n""" \
            f"""#if {self.object_names["obj_name_define"]}\n\n""" \
            f"""namespace wpp {{\n\n""" \
            f"""static const {TYPE_OBJECT_INFO} {self.object_names["obj_name_up_underline"]}_OBJ_INFO = {{\n""" \
            f"""\t/* Name */\n\t"{self.meta_object["object_name"]}",\n""" \
            f"""\t/* Object ID */\n\tOBJ_ID::{self.object_names["obj_name_up_underline"]},\n""" \
            f"""\t/* URN */\n\t"{self.meta_object["object_urn"]}",\n""" \
            f"""\t/* Object version */\n\t{{{self.meta_object["object_version"].replace('.', ',')}}},\n""" \
            f"""\t/* Lwm2m version */\n\t{{{self.meta_object["object_lwm2m_version"].replace('.', ',')}}},\n""" \
            f"""\t/* Is single */\n\tIS_SINGLE::{is_multiple},\n""" \
            f"""\t/* Is Mandatory */\n\tIS_MANDATORY::{is_mandatory},\n""" \
            f"""\t/* Object supported operations */\n""" \
            f"""\t/* --------------- Info block 0 start --------------- */\n""" \
            f"""\tInstOp(\tInstOp::CREATE |\n""" \
            f"""\t\t\tInstOp::DELETE),\n""" \
            f"""\t{TYPE_OPERATION}(\t{TYPE_OPERATION}::READ|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::WRITE|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::DISCOVER|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::EXECUTE|\n""" \
            f"""\t\t\t{TYPE_OPERATION}::DELETE),\n""" \
            f"""\t/* --------------- Info block 0 end --------------- */\n""" \
            f"""}};\n\n""" \
            f"""}} /* namespace wpp */\n\n""" \
            f"""#endif /* {self.object_names["obj_name_define"]} */\n""" \
            f"""#endif // {if_not_def}\n"""

        return content

    def generate_content_config(self):
        if_not_def = f"""WPP_{self.object_names["obj_name_up_underline"]}_CONFIG_H"""
        defines = ""
        for resource in self.meta_resources:
            if resource["Mandatory"] == "MANDATORY":
                continue
            defines += f"""#define {resource['Define']} 0\n"""
        content = \
            CONFIG_COMMENT + \
            f"""#ifndef {if_not_def}\n""" \
            f"""#define {if_not_def}\n\n""" \
            f"""#if {self.object_names["obj_name_define"]}\n\n""" \
            f"""/* --------------- Config block 0 start --------------- */\n""" \
            f"""/* --------------- Config block 0 end --------------- */\n\n""" \
            f"""#endif // {if_not_def}\n""" \
            f"""#endif // {self.object_names["obj_name_define"]}\n"""
        return content

    def create_folder(self):
        try:
            os.mkdir(self.object_names['obj_name_folder'])
        except FileExistsError:
            pass

    def create_file(self, filename, filetype, content):
        f = open(f"./{self.object_names['obj_name_folder']}/{filename}.{filetype}", "w+")
        f.write(content)
        f.close()

    def object_code_generate(self):
        generated_header = self.generate_content_header()
        generated_cpp_file = self.generate_content_cpp()
        generated_cmake_list = self.generate_content_cmake_list()
        generated_info_header = self.generate_content_info_header()
        generated_config = self.generate_content_config()

        name_class = self.object_names["obj_name_class"]

        self.create_folder()

        self.create_file(f"{name_class}",       "h",    generated_header)
        self.create_file(f"{name_class}Info",   "h",    generated_info_header)
        self.create_file(f"{name_class}Config", "h",    generated_config)
        self.create_file(f"{name_class}",       "cpp",  generated_cpp_file)
        self.create_file(f"CMakeLists",         "txt",  generated_cmake_list)


if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-f", "--file", dest="xml_file", help="the path to the xml file of the Object")
    parser.add_option("-u", "--url", dest="xml_url", help="the url to the xml file of the Object")
    options, args = parser.parse_args()

    if options.xml_file or options.xml_url:
        og = ObjectGenerator(options.xml_file, options.xml_url)
        og.object_code_generate()
    else:
        parser.error("the path or url to the XML-file of the Object is not provided")
