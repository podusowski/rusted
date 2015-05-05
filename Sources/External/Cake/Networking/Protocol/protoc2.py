#! /usr/bin/env python

import os
import sys
import argparse
from xml.dom import minidom

def as_cpp_type(xmlType):
    if xmlType == "boolean":
        return "Boolean"
    elif xmlType == "string":
        return "String"
    elif xmlType == "int32":
        return "Integer"
    elif xmlType == "real":
        return "Real";
    else:
        print "unknown param type: " + xmlType
        sys.exit(1)

class Param:
    def __init__(self, xml_node):
        self.name = xml_node.getAttribute("name")
        self.type = xml_node.getAttribute("type")
        self.cpp_type = as_cpp_type(xml_node.getAttribute("type")) 

class List:
    def __init__(self, xml_node):
        self.name = xml_node.getAttribute("name")
        self.type = xml_node.getAttribute("type")
        self.xml_node = xml_node
        self.params = self.__parse_params()

    def __parse_params(self):
        params = []
        for listparam_xml_node in self.xml_node.getElementsByTagName("listparam"):
            param = Param(listparam_xml_node)
            params.append(param)
        return params

class Struct:
    def __init__(self, xml_node):
        self.xml_node = xml_node
        self.id = xml_node.getAttribute("id")
        self.params = self.__parse_params()
        self.lists = self.__parse_lists()

    def has_params(self):
        return len(self.params) > 0 or len(self.lists) > 0

    def __parse_params(self):
        params = []
        for param_xml_node in self.xml_node.getElementsByTagName("param"):
            param = Param(param_xml_node) 
            params.append(param)
        return params

    def __parse_lists(self):
        lists = []
        for list_xml_node in self.xml_node.getElementsByTagName("list"):
            list = List(list_xml_node)
            lists.append(list)
        return lists

# cpp generator

class CppFormatter:
    def idented_code(self, code):
        ident = "    "
        lines = code.split("\n")
        return ident + ("\n" + ident).join(lines)

class CppContainer:
    def __init__(self):
        self.elements = []

    def add_element(self, element):
        self.elements.append(element)

    def code(self):
        s = ""
        for e in self.elements:
            s += e.code() + "\n"
        return s

class CppVariableDeclaration:
    def __init__(self, type, name):
        self.s = type + " " + name + ";"

    def code(self):
        return self.s

class CppFunction:
    def __init__(self, return_type, name, arguments, body, const=False, inline=False, override=False):
        self.s = ""

        if inline:
            self.s += "inline "

        self.s += return_type + " " + name + "(" + ", ".join(arguments) + ")"

        if const:
            self.s += " const"

        if override:
            self.s += " override"

        self.s += "\n"
        self.s += "{\n"

        f = CppFormatter()

        self.s += f.idented_code(body)
        self.s += "\n"
        self.s += "}\n"

    def code(self):
        return self.s

class CppPureVirtualFunction:
    def __init__(self, return_type, name, arguments, const = False):
        s = "virtual " + return_type + " " + name + "("
        s += ", ".join(arguments)
        s += ")"

        if const:
            s += " const"

        s += " = 0;"
        self.s = s

    def code(self):
        return self.s

class CppConstructor:
    def __init__(self, name, arguments = [], initialization_list = [], body=""):
        self.s = "explicit " + name + "(" + ", ".join(arguments) + ")"

        if len(initialization_list) > 0:
            self.s += " :\n"

            initialization_list_code = []
            for arg, dest in initialization_list:
                initialization_list_code.append(dest + "(" + arg + ")")

            self.s += ", ".join(initialization_list_code)

        self.s += "\n"
        self.s += "{\n"
        self.s += body + "\n"
        self.s += "}\n"

    def code(self):
        return self.s

class CppStruct(CppContainer):
    def __init__(self, name, inherits = [], virtual_destructor = True, default_constructor = False):
        CppContainer.__init__(self)

        self.name = name
        self.inherits = inherits
        self.virtual_destructor = virtual_destructor
        self.default_constructor = default_constructor

    def code(self):
        s = "struct " + self.name

        if len(self.inherits) > 0:
            inherits_code = []
            for i in self.inherits:
                inherits_code.append("public " + i)
            s += " : " + ", ".join(inherits_code)

        s += "\n"
        s += "{\n"

        f = CppFormatter()
        s += f.idented_code(CppContainer.code(self))

        if self.default_constructor:
            s += self.name + "() {}\n"

        if self.virtual_destructor:
            s += "virtual ~" + self.name + "() {}\n"

        s += "};\n"
        return s

class CppEnum:
    def __init__(self, name, instance_name=False):
        self.name = name
        self.instance_name = instance_name
        self.values = []

    def add_value(self, name, value=None):
        if value:
            self.values.append("    " + name + " = " + str(value))
        else:
            self.values.append("    " + name)

    def code(self):
        s = "enum class " + self.name + "\n{\n"
        s += ",\n".join(self.values)
        s += "\n};\n"

        if self.instance_name:
            s += "\n" + self.name + " " + self.instance_name + ";\n"

        return s

class CppNamespace(CppContainer):
    def __init__(self, names):
        CppContainer.__init__(self)

        self.names = names

    def code(self):
        s = ""
        for name in self.names:
            s += "namespace " + name + "\n{\n"

        s += "\n"
        s += CppContainer.code(self)
        s += "\n"

        for name in self.names:
            s += "} // namespace " + name + "\n"

        return s

class CppCustomCode:
    def __init__(self, content):
        self.s = content

    def code(self):
        return self.s

class CppFile:
    def __init__(self, header = True):
        self.code = "/* this file autogenerated by {}, do not edit by hand */\n\n".format(__file__)

        if header:
            self.code += "#pragma once\n\n"

    def add_element(self, element):
        self.code += element.code()

    def add_system_includes(self, names):
        for name in names:
            self.code += "#include <" + name + ">\n"
        self.code += "\n"

    def add_local_includes(self, names):
        for name in names:
            self.code += "#include \"" + name + "\"\n"
        self.code += "\n"

    def add_custom_code(self, code):
        self.code += code

    def write(self, filename):
        try:
            os.makedirs(os.path.dirname(filename))
        except: pass

        f = open(filename, "w")
        f.write(self.code)
        f.close()

# cpp generator end

class Generator:
    def __init__(self, structs, output_file, namespace):
        self.cpp_file = CppFile()
        self.cpp_namespace = CppNamespace(namespace.split("::"))

        self.output_file = output_file
        self.structs = structs
        self.namespace = namespace.split("::")

        print("writing to " + self.output_file)
        print("namespace: " + str(self.namespace))

    def generate(self):
        self.__generate_includes()
        self.__generate_id_enum()
        self.__generate_messages()
        self.__generate_message_factory()
        self.__generate_handler_caller()
        self.__generate_stream_operator_for_messages()

        self.cpp_file.add_element(self.cpp_namespace)
        self.cpp_file.write(self.output_file)

    def __generate_includes(self):
        self.cpp_file.add_system_includes([
            "stdexcept",
            "vector",
            "string",
            "ostream",
            "sstream",
            "memory"
        ])

        self.cpp_file.add_local_includes([
            "Cake/Networking/Protocol/FcDecoder.hpp",
            "Cake/Networking/Protocol/Primitives.hpp"
        ])

    def __generate_id_enum(self):
        print("generating Id enum")
        cpp_enum = CppEnum("Id")
        cpp_enum.add_value("InvalidMessage", 0)

        print("\tInvalidMessage = 0")

        number = 100
        for struct in self.structs:
            print("\t" + struct.id + " = " + str(number))
            cpp_enum.add_value(struct.id, number)
            number += 1

        self.cpp_namespace.add_element(cpp_enum)

    def __generate_messages(self):
        print("generating message structures")
        print(str(self.structs))

        for integer_identifier, struct in enumerate(self.structs):
            cpp_struct = MessageGenerator(struct, integer_identifier)
            code = cpp_struct.generate()
            self.cpp_namespace.add_element(CppCustomCode(code))

    def __generate_message_factory(self):
        print("generating message factory")
        message_factory = MessageFactoryGenerator(self.structs)
        code = message_factory.generate()
        self.cpp_namespace.add_element(CppCustomCode(code))

    def __generate_handler_caller(self):
        print("generating handler caller")
        handler_caller = HandlerCallerGenerator(self.structs)

        for i in range(2):
            code = handler_caller.generate(i)
            self.cpp_namespace.add_element(CppCustomCode(code))

    def __generate_stream_operator_for_messages(self):
        f = CppFunction(
            inline = True,
            return_type = "std::ostream &",
            name = "operator <<",
            arguments = ["std::ostream & stream", "const AbstractMessage & message"],
            body = "return stream << message.toString();")

        self.cpp_namespace.add_element(f)

class MessageGenerator:
    def __init__(self, message, integer_identifier):
        self.message = message
        self.integer_identifier = integer_identifier

    def generate(self):
        print("\t" + self.message.id)

        cpp_struct = CppStruct(
            name = self.message.id,
            inherits = ["ICodable", "ICodableStructure"],
            virtual_destructor = False,
            default_constructor = True
        )

        cpp_struct.add_element(self.__generate_params())
        cpp_struct.add_element(self.__generate_sequences())
        cpp_struct.add_element(self.__generate_from_string_constructor())
        cpp_struct.add_element(self.__generate_eq_operator())
        cpp_struct.add_element(self.__generate_ne_operator())
        cpp_struct.add_element(self.__generate_getid_method())
        cpp_struct.add_element(self.__generate_encode_method())

        cpp_struct.add_element(self.__generate_decoder_state())
        cpp_struct.add_element(self.__generate_decode_method())
        #cpp_struct.add_element(self.__generate_unserialize_from_string_method())
        cpp_struct.add_element(self.__generate_to_string_method())

        return cpp_struct.code()

    def __generate_getid_method(self):
        return CppFunction(
            return_type = "int",
            name = "id",
            arguments = [],
            body = "return " + str(self.integer_identifier) + ";",
            const = True,
            override = True
        )

    def __generate_encode_method(self):
        s = "Cake::Networking::Bytes bytes;\n"

        members = self.message.params + self.message.lists
        for param in members:
            s += "bytes.extend({}.encode());\n".format(param.name)

        s += "return bytes;"

        return CppFunction(
            return_type = "Cake::Networking::Bytes",
            name = "encode",
            arguments = [],
            const = True,
            body = s,
            override = True
        )

    def __generate_decoder_state(self):
        state_type = CppEnum("DecoderState", "m_decoderState")

        members = self.message.params + self.message.lists
        for member in members:
            state_type.add_value(member.name)

        return state_type

    def __generate_decode_method(self):
        s = "switch (m_decoderState)\n{\n"

        members = self.message.params + self.message.lists
        for member in members:
            s += "case DecoderState::{}:\n".format(member.name)
            s += "    break;\n"

        s += "};\n"
        s += "return 0;"

        return CppFunction(
            return_type = "std::size_t",
            name = "decode",
            arguments = ["const Cake::Networking::Bytes & bytes"],
            body = s,
            override = True
        )

    def __generate_unserialize_from_string_method(self):
        s = "Cake::Networking::Protocol::FcDecoder decoder(fc);\n"

        if len(self.message.params) > 0 or len(self.message.lists) > 0:
            s = s + "decoder"

            for param in self.message.params:
                s = s + " >> " + param.name

            for param in self.message.lists:
                s = s + " >> " + param.name

            s = s + ";"

        return CppFunction(
            return_type = "void",
            name = "unserialize",
            arguments = ["const Cake::Serialization::Fc & fc"],
            body = s
        )

    def __generate_params(self):
        ret = CppContainer()

        for param in self.message.params:
            print("\t\t" + param.name + ": " + param.type)
            ret.add_element(CppVariableDeclaration(param.cpp_type, param.name))

        return ret

    def __generate_sequences(self):
        ret = CppContainer()

        for param in self.message.lists:
            print("\t\t" + param.name + ": list of " + param.type)
            cpp_type = "Sequence<" + param.type + ">"
            ret.add_element(CppVariableDeclaration(cpp_type, param.name))

        return ret

    def __generate_from_string_constructor(self):
        body = "    const auto parameters = Cake::Serialization::Fc{fcString}.getParameters();\n"

        if len(self.message.params) > 0 or len(self.message.lists) > 0:

            index = 0
            for param in self.message.params:
                body += "    {} = {}(parameters.at({}));\n".format(param.name, param.cpp_type, index)
                index += 1

            for param in self.message.lists:
                body += "    {} = Sequence<{}>(parameters.at({}));\n".format(param.name, param.type, index)
                index += 1

        ret = CppConstructor(
            name = self.message.id,
            arguments = ["std::string fcString"],
            body = body
        )

        return ret

    def __generate_eq_operator(self):
        body = ""

        if len(self.message.params) > 0 or len(self.message.lists) > 0:
            comparisons = []
            for param in self.message.params:
                comparisons.append(param.name + " == other." + param.name)

            for param in self.message.lists:
                comparisons.append(param.name + " == other." + param.name)

            body = "return " + " &&\n       ".join(comparisons) + ";"
        else:
            body = "return true;"

        return CppFunction(
            return_type = "bool",
            name = "operator ==",
            arguments = ["const " + self.message.id + " & other"],
            const = True,
            body = body
        )

    def __generate_ne_operator(self):
        return CppFunction(
            return_type = "bool",
            name = "operator !=",
            arguments = ["const " + self.message.id + " & other"],
            const = True,
            body = "return !(*this == other);"
        )

    def __generate_to_string_method(self):
        s = "std::stringstream ss;\n"

        s += 'ss << "' + self.message.id + ' ( ";\n'

        for param in self.message.params:
            s += 'ss << "' + param.name + ':" << ' + param.name + ' << " ";\n'

        for p in self.message.lists:
            s += 'ss << "' + p.name + ':( ";\n'
            s += 'for (const auto & e : ' + p.name + ')\n'
            s += '{\n'
            s += '    ss << e.toString() << " ";\n'
            s += '}\n'
            s += 'ss << ") ";\n'

        s += 'ss << ")";\n'

        s += "return ss.str();"

        return CppFunction(
            return_type = "std::string",
            name = "toString",
            arguments = [],
            const = True,
            body = s
        )

class MessageFactoryGenerator:
    def __init__(self, messages):
        self.messages = messages

    def generate(self):
        cpp_struct = CppStruct(
            name = "MessageFactory",
            virtual_destructor = False
        )

        s = ""
        s = s + self.__generate_create_from_buffer_method()
        s = s + self.__generate_create_from_string_method()

        cpp_struct.add_element(CppCustomCode(s))

        return cpp_struct.code()

    def __generate_create_from_buffer_method(self):
        s = ""
        s = s + "    static std::shared_ptr<ICodableStructure> create(int id)\n"
        s = s + "    {\n"
        s = s + "        switch(id)\n"
        s = s + "        {\n"

        for message in self.messages:
            s = s + self.__generate_message_case(message)

        s = s + "        default:\n"
        s = s + "            throw std::runtime_error(\"unknown message id\");\n"
        s = s + "        } // switch\n"
        s = s + "    } // create\n"
        s = s + "\n"
        return s

    def __generate_message_case(self, message):
        s = ""
        s = s + "        case static_cast<int>(Id::" + message.id + "):\n"
        s = s + "            return std::make_shared<" + message.id + ">();\n"
        s = s + "            break;\n"
        return s

    def __generate_create_from_string_method(self):
        s = (""
             "    static std::shared_ptr<ICodableStructure> create(std::string fcString)\n"
             "    {\n"
             "        std::shared_ptr<AbstractMessage> ret;\n"
             "        Cake::Serialization::Fc fc(fcString);\n")

        for message in self.messages:
            s = s + ("        if (fc.getName() == \"" + message.id + "\")\n"
                     "        {\n"
                     "            return std::make_shared<" + message.id + ">(fcString);\n"
                     "        }\n")

        s += "    }\n"

        return s

class HandlerCallerGenerator:
    def __init__(self, messages):
        self.messages = messages

    def generate(self, parameter_count):
        s = ""
        s = s + self.__generate_header(parameter_count)

        for message in self.messages:
            s = s + self.__generate_message_case(parameter_count, message)

        s = s + self.__generate_footer()
        s = s + "\n"

        return s

    def __generate_header(self, parameter_count):
        s = ""
        s = s + "template<class HandlerType"

        for i in range(parameter_count):
            s = s + ", class T" + str(i)

        s = s + ">\n"
        s = s + "struct HandlerCaller" + str(parameter_count) + "\n"
        s = s + "{\n"
        s = s + "\tHandlerCaller" + str(parameter_count) + "(HandlerType & handler) :\n"
        s = s + "\tm_handler(handler)\n"
        s = s + "\t{\n"
        s = s + "\t}\n"
        s = s + "\n"
        s = s + "\tvoid call(ICodableStructure & message"

        for i in range(parameter_count):
            s = s + ", T" + str(i) + " p" + str(i)

        s = s + ")\n"
        s = s + "\t{\n"
        s = s + "\t\tswitch(message.id())\n"
        s = s + "\t\t{\n"
        return s

    def __generate_message_case(self, parameter_count, message):
        s = ""
        s = s + "\t\tcase static_cast<int>(Id::" + message.id + "):\n"
        s = s + "\t\t\tm_handler.handle(static_cast<const " + message.id + "&>(message)"

        for i in range(parameter_count):
            s = s + ", p" + str(i)

        s = s + "); break;\n"
        s = s + "\n"

        return s

    def __generate_footer(self):
        s = ""
        s = s + "\t\tdefault:\n"
        s = s + "\t\t\tthrow std::runtime_error(\"unknown message id\");\n"
        s = s + "\t\t} // switch\n"
        s = s + "\t} // call\n"
        s = s + "\n"
        s = s + "\tprivate:\n"
        s = s + "\t\tHandlerType & m_handler;\n"
        s = s + "}; // class HandlerCaller\n"
        return s


parser = argparse.ArgumentParser()
parser.add_argument("-o", "--output", required=True)
parser.add_argument("-n", "--namespace", default="Cake::Networking::Protocol")
parser.add_argument("xml")
options = parser.parse_args()

print("reading " + options.xml)
messages = minidom.parse(options.xml)

structs = []
for struct_xml_node in messages.childNodes[0].getElementsByTagName("struct"):
    struct = Struct(struct_xml_node)
    structs.append(struct)

for message_xml_node in messages.childNodes[0].getElementsByTagName("message"):
    message = Struct(message_xml_node)
    structs.append(message)

hpp = Generator(structs, options.output, options.namespace)
hpp.generate()


