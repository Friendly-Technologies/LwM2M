import os
import sys
from datetime import datetime
from tabulate import tabulate
import xml.etree.ElementTree as ET
from optparse import OptionParser

PLACE_CLASS_NAME_UPPER = "<<<0>>>"
PLACE_CLASS_NAME = "<<<1>>>"
PLACE_RESOURCES_ENUM = "<<<2>>>"
PLACE_RESOURCES_MAP = "<<<3>>>"
PLACE_FOLDER = "<<<4>>>"
PLACE_RESOURCES_INIT = "<<<5>>>"

TYPE_REGISTRY = "WppRegistry"
TYPE_OPERATION = "Operation"
TYPE_OBJECT = "Object"
TYPE_OBJECT_INFO = "ObjectInfo"
TYPE_RESOURCE = "Resource"
TYPE_INSTANCE = "Instance"
TYPE_I_SUBJECT = "InstSubject"

LOG_FUNC_CUSTOM = "WPP_LOGD_ARG"

STOP_STRING_CNFG_CMK = ["# The end of the options of the mandatory objects.",
                        "# The end of the options of the optional objects."]
STOP_STRING_OBJ_ID = ["/* The end of the IDs of the mandatory objects. */",
                      "/* The end of the IDs of the optional objects. */"]
STOP_STRING_REG_PRT = ["/* The end of the prototypes of the mandatory objects. */",
                       "/* The end of the prototypes of the optional objects. */"]
STOP_STRING_REG_INCL = ["/* The end of the includes of the mandatory objects. */",
                        "/* The end of the includes of the optional objects. */"]

MAIN_COMMENT = \
    f"""/*\n""" \
    f""" * {PLACE_CLASS_NAME}\n""" \
    f""" * Generated on: {datetime.now()}\n""" \
    f""" * Author: valentin\n""" \
    f""" * Generated by: Vadimzakreva\n""" \
    f""" */\n\n"""

PREFIX_H = \
    f"""#ifndef {PLACE_CLASS_NAME_UPPER}_H\n""" \
    f"""#define {PLACE_CLASS_NAME_UPPER}_H\n\n""" \
    f"""#include "{PLACE_CLASS_NAME}Config.h"\n#include "{PLACE_CLASS_NAME}Info.h"\n""" \
    f"""#include "{TYPE_INSTANCE}.h"\n#include "{TYPE_I_SUBJECT}.h"\n\n""" \
    f"""namespace wpp {{\n\nclass {PLACE_CLASS_NAME} : """ \
    f"""public {TYPE_INSTANCE}, public {TYPE_I_SUBJECT}<{PLACE_CLASS_NAME}> {{\n\n"""

PUBLIC_ENUM_H = \
    f"""\tpublic:\n\t\tenum ID: ID_T {{\n{PLACE_RESOURCES_ENUM}\t\t}};\n\n"""

PUBLIC_CONSTRUCTOR_H = \
    f"""\tpublic:\n\t\t{PLACE_CLASS_NAME}(WppClient &client, const InstanceID &id);\n\n"""

I_INSTANCE_IMPLEMENTATIONS = \
    f"""\t/* --------------- {TYPE_INSTANCE} implementation part --------------- */\n"""

I_INSTANCE_IMPLEMENTATIONS_H = \
    f"""\tprotected:\n\t{I_INSTANCE_IMPLEMENTATIONS}\t\t/* \n\t""" \
    f"""\t * Returns Resource object if it is exist\n\t\t */\n\t\t{TYPE_RESOURCE} * getResource(ID_T id) override;\n\t\t""" \
    f"""/*\n\t\t * Returns list with available resources\n\t\t */\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getResourcesList() override;\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getResourcesList(const {TYPE_OPERATION}& filter) override;\n\t\t/*\n\t """ \
    f"""\t * Returns list with available instantiated resources\n\t\t */\n\t """ \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getInstantiatedResourcesList() override;\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> getInstantiatedResourcesList(const {TYPE_OPERATION}& filter) override;\n\t\t/*\n\t""" \
    f"""\t * Handles information about resource operation that made server\n\t\t */\n\t""" \
    f"""\tvoid serverOperationNotifier({TYPE_OPERATION}::TYPE type, const ResourceID &resId) override;\n\t\t/*\n\t""" \
    f"""\t * Handles information about resource operation that made user\n\t\t */\n\t""" \
    f"""\tvoid userOperationNotifier({TYPE_OPERATION}::TYPE type, const ResourceID &resId) override;"""

CLASS_PRIVATE_METHODS_H = \
    f"""\n\n\tprivate:\n\t\t/* --------------- Class private methods --------------- */\n\t""" \
    f"""\t/*\n\t\t * Initialize resources with default values\n\t""" \
    f"""\t * Resource always must have at least one instance.\n\t\t */\t\n\t""" \
    f"""\tvoid resourcesInit();\n\t\n\n""" \
    f"""\tprivate:\n\t\tstd::unordered_map<ID_T, {TYPE_RESOURCE}> _resources = {{\n\t\t""" \
    f"""\t// KEY   VALUE\n{PLACE_RESOURCES_MAP}\n\t}};\n}};\n\n""" \
    f"""}} /* namespace wpp */\n\n#endif /* {PLACE_CLASS_NAME_UPPER}_H */\n"""

PREFIX_CPP = \
    f"""#include "mandatory/{PLACE_FOLDER}/{PLACE_CLASS_NAME}.h"\n\n""" \
    f"""#include <unordered_map>\n""" \
    f"""#include <iostream>\n\n""" \
    f"""#include "{TYPE_RESOURCE}.h"\n""" \
    f"""#include "{TYPE_OPERATION}.h"\n""" \
    f"""#include "types.h"\n""" \
    f"""#include "WppLogs.h"\n\n""" \
    f"""#define TAG "{PLACE_CLASS_NAME}"\n\n""" \
    f"""namespace wpp {{\n\n"""

PUBLIC_CONSTRUCTOR_CPP = \
    f"""\t{PLACE_CLASS_NAME}::{PLACE_CLASS_NAME}(WppClient &client, const InstanceID &id): {TYPE_INSTANCE}(client, id) {{""" \
    f"""\n\t\tresourcesInit();\n\t}}\n\n"""

CLASS_PRIVATE_METHODS_CPP = \
    f"""\t/* --------------- Class private methods --------------- */\n{PLACE_RESOURCES_INIT}"""

FUNC_GET_RESOURCE_T = \
    f"""\t{TYPE_RESOURCE} * {PLACE_CLASS_NAME}::getResource(ID_T id) {{\n\t""" \
    f"""\t// Check if resource ID is valid\n\t""" \
    f"""\tif (_resources.find(id) == _resources.end()) return NULL;\n\t""" \
    f"""\treturn &_resources[id];\n\t}}\n\n"""

FUNC_GET_RESOURCE_LIST = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getResourcesList() {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tlist.push_back(&pair.second);\n\t\t}}\n\t""" \
    f"""\treturn list;\n\t}}\n\n"""

FUNC_GET_RESOURCE_LIST_P = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getResourcesList(const {TYPE_OPERATION}& filter) {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tif (filter.isCompatible(pair.second.getOperation())) list.push_back(&pair.second);\n\t\t}}\n\t""" \
    f"""\treturn list;\n\t}}\n\n"""

FUNC_GET_INSTANTIATED_LIST = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getInstantiatedResourcesList() {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tif (!pair.second.isEmpty()) list.push_back(&pair.second);\n\t\t}}\n\t""" \
    f"""\treturn list;\n\t}}\n\n"""

FUNC_GET_INSTANTIATED_LIST_P = \
    f"""\tstd::vector<{TYPE_RESOURCE} *> {PLACE_CLASS_NAME}::getInstantiatedResourcesList(const {TYPE_OPERATION}& filter) {{\n\t""" \
    f"""\tstd::vector<{TYPE_RESOURCE} *> list;\n\t""" \
    f"""\tfor (auto &pair : _resources) {{\n\t\t""" \
    f"""\tif (!pair.second.isEmpty() && filter.isCompatible(pair.second.getOperation())) """ \
    f"""list.push_back(&pair.second);\n\t\t}}\n\t\treturn list;\n\t}}\n\n"""


class CodeGenerator:
    """Add some comments here"""
    meta_object = None
    meta_resources = None
    obj_name_class = None
    obj_name_folder = None
    obj_define = None
    obj_name = None

    def __init__(self, object_dict, resources_list):
        self.meta_object = object_dict
        self.meta_resources = resources_list
        plain_name = object_dict["object_name"]
        self.obj_name = plain_name
        folder_name = plain_name.replace(' ', '_').lower()
        class_name = plain_name.replace(' ', '')

        self.obj_name_class = f"Wpp{class_name}"        # LwM2MServer
        self.obj_name_folder = folder_name                              # lwm2m_server
        self.obj_define = self.create_object_define()                   # MANDATORY_LWM2MSERVER_OBJ


    def create_object_define(self):
        define_name = self.obj_name_class.replace(' ', '_').upper()
        define_prefix = "MANDATORY" if self.meta_object["is_mandatory"] else "OPTIONAL"
        return f"{define_prefix}_{define_name}_OBJ"

    def parse_operation(self, xml_operation):
        operation = f"{TYPE_OPERATION}::"
        match xml_operation:
            case "E":
                operation += "EXECUTE"
            case "R":
                operation += "READ"
            case "W":
                operation += "WRITE"
            case "RW":
                operation = f"{TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
            case default:
                operation = f"{TYPE_OPERATION}::READ|{TYPE_OPERATION}::WRITE"
        operation += ","
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
        resources_map = []
        for resource_xml in resources_list_xml:
            resource_name = resource_xml['Name']
            postfix = "M" if resource_xml['Mandatory'] == "MANDATORY" else "O"
            # fill the Resources' enum:
            resource_define = f"{resource_name}_{postfix}"
            resource_enum = f"\t\t\t{resource_define} = {resource_xml['ID']},\n"
            resources_enum += resource_enum
            # fill the unordered_map<ID_T, Resource> table:
            resource = [f"TAB#if {resource_define}_",
                        f"CRLFTAB{{{resource_name}_{postfix},",
                        f"{{{resource_name}_{postfix},",
                        self.parse_operation(resource_xml['Operations']),
                        f"IS_SINGLE::{resource_xml['MultipleInstances']},",
                        f"IS_MANDATORY::{resource_xml['Mandatory']},",
                        f"{self.parse_resource_data_type(resource_xml['Type'])} }}}},"
                        f"CRLFTAB#endif"]
            resources_map.append(resource)

        return resources_enum, tabulate(resources_map, tablefmt="plain")

    def get_content_resourcesInit_f(self, resources_list_xml):
        content = f"""\tvoid {PLACE_CLASS_NAME}::resourcesInit() {{\n"""
        for resource in resources_list_xml:
            if resource["Mandatory"] == "MANDATORY":
                content += f"""\t\t#if {resource["Name"]}_{resource["Mandatory"]}\n\t"""
                content += f"\t\t_resources[{resource['Name']}].set( /* TODO */ );\n\t"
                content += f"\t\t_resources[{resource['Name']}].setDataVerifier( /* TODO */ );\n"
                content += f"\t\t#endif\n\n"
        return content + "\t}\n\n"

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
            text = '"' + text + '\\n"'
            for argument in arguments:
                text += ', ' + argument
            return f"""{LOG_FUNC_CUSTOM}(TAG, {text});"""

    def get_content_serverOperationNotifier(self):
        cases = ["READ", "WRITE", "EXECUTE", "DISCOVER", "DELETE"]
        base = \
            f"""\tvoid {PLACE_CLASS_NAME}::serverOperationNotifier({TYPE_OPERATION}::TYPE type, const ResourceID &resId) {{""" \
            f"""\n\t\tobserverNotify(*this, resId, type);\n\n""" \
            f"""\t\tswitch (type) {{\n\t"""
        for case in cases:
            base += f"""\t\tcase {TYPE_OPERATION}::{case}:\n\t\t\t\t{self.create_log_string(
                f"Server {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\t\t\tbreak;\n\t"""
        return f"""{base}\t\tdefault: break;\n\t\t}}\n\t}}\n\n"""

    def get_content_userOperationNotifier(self):
        cases = ["READ", "WRITE", "DELETE"]
        prefix = \
            f"""\tvoid {PLACE_CLASS_NAME}::userOperationNotifier({TYPE_OPERATION}::TYPE type, const ResourceID &resId) {{""" \
            f"""\n\t\tswitch (type) {{\n\t"""
        for case in cases:
            prefix += f"""\t\tcase {TYPE_OPERATION}::{case}:\n\t\t\t\t{self.create_log_string(
                f"User {case} -> resId: %d, resInstId: %d",
                ["resId.resId", "resId.resInstId"],
                False
            )}\n\t\t\t\tbreak;\n\t"""
        return f"""{prefix}\t\tdefault: break;\n\t\t}}\n\t}}\n\n"""

    def generate_content_header(self):
        resources_enum, resources_map = self.get_map_of_resources(self.meta_resources)
        code_header = (MAIN_COMMENT +
                       PREFIX_H +
                       PUBLIC_ENUM_H +
                       PUBLIC_CONSTRUCTOR_H +
                       I_INSTANCE_IMPLEMENTATIONS_H +
                       CLASS_PRIVATE_METHODS_H +
                       "")
        code_header = code_header.replace(PLACE_CLASS_NAME_UPPER, self.obj_name_class.upper())
        code_header = code_header.replace(PLACE_CLASS_NAME, self.obj_name_class)
        code_header = code_header.replace(PLACE_RESOURCES_ENUM, resources_enum)
        code_header = code_header.replace(PLACE_RESOURCES_MAP, resources_map)
        code_header = code_header.replace("TAB", "\t\t\t")
        code_header = code_header.replace("CRLF", "\n")

        return code_header

    def generate_content_cpp(self):
        code_cpp = (MAIN_COMMENT +
                    PREFIX_CPP +
                    PUBLIC_CONSTRUCTOR_CPP +
                    I_INSTANCE_IMPLEMENTATIONS +
                    FUNC_GET_RESOURCE_T +
                    FUNC_GET_RESOURCE_LIST +
                    FUNC_GET_RESOURCE_LIST_P +
                    FUNC_GET_INSTANTIATED_LIST +
                    FUNC_GET_INSTANTIATED_LIST_P +
                    self.get_content_serverOperationNotifier() +
                    self.get_content_userOperationNotifier() +
                    CLASS_PRIVATE_METHODS_CPP +
                    "} /* namespace wpp */\n")
        code_cpp = code_cpp.replace(PLACE_RESOURCES_INIT, self.get_content_resourcesInit_f(self.meta_resources))
        code_cpp = code_cpp.replace(PLACE_CLASS_NAME, self.obj_name_class)
        code_cpp = code_cpp.replace(PLACE_FOLDER, self.obj_name_folder)

        return code_cpp

    def generate_content_cmake_list(self):
        main_line = f"""set(SOURCES ${{SOURCES}} ${{CMAKE_CURRENT_SOURCE_DIR}}/{self.obj_name_class}.cpp PARENT_SCOPE)"""

        return f"""if({self.obj_define})\n\t# Update SOURCES variable from parent scope.\n\t""" \
               f"""{main_line}\nendif()"""

    def generate_content_info_header(self):
        class_name = f"{self.obj_name_class.replace(' ', '').upper()}INFO_H"
        is_multiple = "MULTIPLE" if self.meta_object["is_multiple"] else "SINGLE"
        is_mandatory = "MANDATORY" if self.meta_object["is_mandatory"] else "OPTIONAL"

        content = \
            f"""#ifndef {class_name}\n""" \
            f"""#define {class_name}\n\n""" \
            f"""#include "{TYPE_OBJECT_INFO}.h"\n\n""" \
            f"""#if {self.obj_define}\n\n""" \
            f"""namespace wpp {{\n\n""" \
            f"""static const {TYPE_OBJECT_INFO} {self.obj_name_class.replace(' ', '').upper()}_OBJ_INFO = {{\n""" \
            f"""\t/* Name */\n\t"{self.meta_object["object_name"]}",\n\n""" \
            f"""\t/* Object ID */\n\tOBJ_ID::SERVER,\n\n""" \
            f"""\t/* URN */\n\t"{self.meta_object["object_urn"]}",\n\n""" \
            f"""\t/* Object version */\n\t{{{self.meta_object["object_lwm2m_version"].replace('.', ',')}}},\n\n""" \
            f"""\t/* Lwm2m version */\n\t{{{self.meta_object["object_version"].replace('.', ',')}}},\n\n""" \
            f"""\t/* Is single */\n\tIS_SINGLE::{is_multiple},\n\n""" \
            f"""\t/* Is Mandatory */\n\tIS_MANDATORY::{is_mandatory},\n\n""" \
            f"""\t/* Object supported operations */\n""" \
            f"""\t{TYPE_OPERATION}(\t{TYPE_OPERATION}::READ|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::WRITE|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::DISCOVER|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::EXECUTE|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::CREATE|\n""" \
            f"""\t\t\t\t{TYPE_OPERATION}::DELETE),\n""" \
            f"""\t}};\n\n""" \
            f"""}} /* namespace wpp */\n\n""" \
            f"""#endif /* {self.obj_define} */\n""" \
            f"""#endif // {class_name}\n"""

        return content

    def generate_content_config(self):
        ifdef = f"WPP_{self.obj_name.replace(' ', '_').upper()}_CONFIG_H"
        defines = ""
        for i in self.meta_resources:
            defines += f"""#define {i["Name"]}_{"M" if i["Mandatory"] == "MANDATORY" else "O"}_"""
            defines += " 1\n" if i["Mandatory"] == "MANDATORY" else " 0\n"
        content = \
            f"""#ifndef {ifdef}\n""" \
            f"""#define {ifdef}\n\n""" \
            f"""#if {self.obj_define}\n\n""" \
            f"""/* ---------- Server optional resources start ---------- */\n\n""" \
            f"""{defines}\n""" \
            f"""/* ---------- Server optional resources end ---------- */\n\n""" \
            f"""#endif // {ifdef}\n""" \
            f"""#endif // {ifdef}\n"""
        return content

    def update_file(self, stop_string, content, path_to_file):
        is_stop_string_present = False
        new_content = ''
        with open(path_to_file, 'r') as f:
            for i in f:
                # print(i)
                if " ".join(i.split()) == stop_string:
                    is_stop_string_present = True
                    new_content += content

                new_content += i
        f.close()

        if not is_stop_string_present:
            print(f"The {path_to_file} was not updated!")

        with open(path_to_file, 'w') as f:
            f.write(new_content)
        f.close()

    def update_files(self, object_dict):
        is_obj_mandatory = object_dict["is_mandatory"]
        stop_string_obj_id = STOP_STRING_OBJ_ID[0] if is_obj_mandatory else STOP_STRING_OBJ_ID[1]
        stop_string_cfg_cmk = STOP_STRING_CNFG_CMK[0] if is_obj_mandatory else STOP_STRING_CNFG_CMK[1]

        stop_string_reg_cpp = STOP_STRING_REG_PRT[0] if is_obj_mandatory else STOP_STRING_REG_PRT[1]
        stop_string_reg_incl = STOP_STRING_REG_INCL[0] if is_obj_mandatory else STOP_STRING_REG_INCL[1]
        stop_string_reg_prot = STOP_STRING_REG_PRT[0] if is_obj_mandatory else STOP_STRING_REG_PRT[1]

        content_obj_id = \
            f"#ifdef {self.obj_define}\n" \
            f"\t{self.obj_name_class.upper()} = {object_dict['object_id']},\n" \
            f"#endif /* {self.obj_define} */\n\n"

        content_cnfg_cmk = \
            f"""\noption({self.obj_define} """ \
            f""""Include {"mandatory" if is_obj_mandatory else "optional"} """ \
            f"""{self.obj_name_class} object in the build" {"ON" if is_obj_mandatory else "OFF"})\n""" \
            f"""if ({self.obj_define})\n\tset(WPP_DEFINITIONS ${{WPP_DEFINITIONS}} {self.obj_define}=1)""" \
            f"""\nendif()\n\n"""

        content_reg_h_incl = \
            f"""#if {self.obj_define}\n""" \
            f"""#include "mandatory/{self.obj_name_folder}/{self.obj_name_class}.h"\n""" \
            f"""#endif\n\n"""

        content_reg_h_prt = \
            f"""\t#if {self.obj_define}\n\t""" \
            f"""{TYPE_OBJECT} <{self.obj_name_class}> & {self.obj_name.replace(" ", '').lower()}();\n\t""" \
            f"""#endif\n\n"""

        content_reg_cpp = \
            f"""# if {self.obj_define}\n""" \
            f"""{TYPE_OBJECT} <{self.obj_name_class}> & {TYPE_REGISTRY}::{self.obj_name.replace(" ", '').lower()}() {{\n\t""" \
            f"""if (!{TYPE_OBJECT} <{self.obj_name_class}>::isCreated()) {TYPE_OBJECT} <{self.obj_name_class}>::create(_client, {self.obj_name_class.replace(' ', '').upper()}_OBJ_INFO);\n\t""" \
            f"""return *{TYPE_OBJECT} <{self.obj_name_class}>::object();\n""" \
            f"""}}\n""" \
            f"""# endif\n"""

        self.update_file(stop_string_obj_id, content_obj_id, "../wpp/registry/ObjectID.h")
        self.update_file(stop_string_cfg_cmk, content_cnfg_cmk, "../wpp/config/config.cmake")

        self.update_file(stop_string_reg_incl, content_reg_h_incl, "../wpp/registry/WppRegistry.h")
        self.update_file(stop_string_reg_prot, content_reg_h_prt, "../wpp/registry/WppRegistry.h")

        self.update_file(stop_string_reg_cpp, content_reg_cpp, "../wpp/registry/WppRegistry.cpp")


class XmlToCppObjectGenerator:
    """Add some comments here"""

    def create_folder(self, folder_name):
        try:
            os.mkdir(folder_name)
        except FileExistsError:
            pass

    def create_file(self, path, filename, filetype, content):
        f = open(f"{path}/{filename}.{filetype}", "w+")
        f.write(content)
        f.close()

    def parse_xml(self, file_path):
        tree = ET.parse(file_path)
        root = tree.getroot()

        # pack the dictionary of the object:
        object_meta = {"object_name": root[0][0].text,
                       "object_description": root[0][1].text,
                       "object_id": root[0][2].text,
                       "object_urn": root[0][3].text,
                       "object_lwm2m_version": root[0][4].text,
                       "object_version": root[0][5].text,
                       "is_multiple": root[0][6].text == "Multiple",
                       "is_mandatory": root[0][7].text == "Mandatory",
                       }

        # pack the list of the dictionary of the resources:
        resources_list = []
        for resources in root.findall('./Object/Resources/Item'):
            # get already existing dictionary (with ID) and fill it by another data in loop:
            resource_dict = resources.attrib
            repl_characters = [' ', '-', '\\', '/', '(', ')', '.', ',']  # TODO: the "(s)" postfix changes to "_S_"
            for resource in resources:
                resource_name = ' '.join(resource.text.split()) if resource.text else "none"
                for character in repl_characters:
                    resource_name = resource_name.replace(character, '_')
                resource_dict[resource.tag] = resource_name.upper()
            resources_list.append(resource_dict)

        # return name, object_id, resources_list, is_mandatory
        return object_meta, resources_list

    def create_code(self, xml_file_path):
        object_dict, resources_list = self.parse_xml(xml_file_path)

        class_name = object_dict["object_name"]
        is_obj_mandatory = object_dict["is_mandatory"]
        folder_name = class_name.replace(' ', '_').lower()
        class_name = class_name.replace(' ', '')

        code_generator = CodeGenerator(object_dict, resources_list)

        generated_header = code_generator.generate_content_header()
        generated_cpp = code_generator.generate_content_cpp()
        generated_cmake_list = code_generator.generate_content_cmake_list()
        generated_info_header = code_generator.generate_content_info_header()
        generated_config = code_generator.generate_content_config()

        path_to_files_prefix = "mandatory" if is_obj_mandatory else "optional"
        path_to_files = f"""../wpp/registry/objects/{path_to_files_prefix}/{folder_name}"""

        self.create_folder(path_to_files)

        self.create_file(path_to_files, f"Wpp{class_name}",         "h",    generated_header)
        self.create_file(path_to_files, f"Wpp{class_name}",         "cpp",  generated_cpp)
        self.create_file(path_to_files, f"CMakeLists",              "txt",  generated_cmake_list)
        self.create_file(path_to_files, f"Wpp{class_name}Info",     "h",    generated_info_header)
        self.create_file(path_to_files, f"Wpp{class_name}Config",   "h",    generated_config)

        code_generator.update_files(object_dict)


parser = OptionParser()
parser.add_option("-f", "--file", dest="filename", help="path to xml file")
(options, args) = parser.parse_args()

if len(args) != 1:
    parser.error("The path to xml file is not provided")
else:
    generator = XmlToCppObjectGenerator()
    generator.create_code(sys.argv[1])
