#! /usr/bin/env python2

import os
import sys
import argparse
from xml.dom import minidom

def as_cpp_type(xmlType):
    if xmlType == "boolean":
        return "bool"
    elif xmlType == "string":
        return "std::string"
    elif xmlType == "int32":
        return "int"
    elif xmlType == "real":
        return "float";
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
    def __init__(self, return_type, name, arguments, body, const = False, inline = False):
        self.s = ""

        if inline:
            self.s += "inline "

        self.s += return_type + " " + name + "(" + ", ".join(arguments) + ")"

        if const:
            self.s += " const"

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
    def __init__(self, name, arguments = [], initialization_list = []):
        self.s = "explicit " + name + "(" + ", ".join(arguments) + ")"

        if len(initialization_list) > 0:
            self.s += " :\n"

            initialization_list_code = []
            for arg, dest in initialization_list:
                initialization_list_code.append(dest + "(" + arg + ")")

            self.s += ", ".join(initialization_list_code)

        self.s += "\n"
        self.s += "{\n"
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
    def __init__(self, name):
        self.name = name
        self.values = []

    def add_value(self, name, value):
        self.values.append("    " + name + " = " + str(value))

    def code(self):
        s = "enum " + self.name + "\n{\n"
        s += ",\n".join(self.values)
        s += "\n};"

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
    def __init__(self, structs, messages, output_file, namespace):
        self.cpp_file = CppFile()
        self.cpp_namespace = CppNamespace(namespace.split("::"))

        self.output_file = output_file
        self.messages = messages
        self.structs = structs
        self.namespace = namespace.split("::")

        print("writing to " + self.output_file)
        print("namespace: " + str(self.namespace))

    def generate(self):
        self.__generate_includes()
        self.__generate_id_enum()
        self.__generate_serializable_interface()
        self.__generate_message_base_class()
        self.__generate_structures()
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
            "Cake/Networking/Protocol/IWriteBuffer.hpp",
            "Cake/Networking/Protocol/IReadBuffer.hpp",
            "Cake/Networking/Protocol/BinaryCoder.hpp",
            "Cake/Networking/Protocol/BinaryDecoder.hpp",
            "Cake/Networking/Protocol/FcDecoder.hpp"
        ])

    def __generate_id_enum(self):
        print("generating Id enum")
        cpp_enum = CppEnum("Messages")
        cpp_enum.add_value("InvalidMessage", 0)

        print("\tInvalidMessage = 0")

        number = 100
        for message in self.messages:
            print("\t" + message.id + " = " + str(number))
            cpp_enum.add_value(message.id, number)
            number += 1

        id_namespace = CppNamespace(["Id"])
        id_namespace.add_element(cpp_enum)
        self.cpp_namespace.add_element(id_namespace)

    def __generate_message_base_class(self):
        abstract_message = CppStruct("AbstractMessage", ["ISerializable"])
        abstract_message.add_element(CppPureVirtualFunction("Id::Messages", "getId", [], True))
        self.cpp_namespace.add_element(abstract_message)

    def __generate_serializable_interface(self):
        serializable_interface = CppStruct("ISerializable")

        serializable_interface.add_element(
            CppPureVirtualFunction("void", "serialize", ["Cake::Networking::Protocol::IWriteBuffer &"], True)
        )

        serializable_interface.add_element(
            CppPureVirtualFunction("void", "unserialize", ["Cake::Networking::Protocol::IReadBuffer &"], False)
        )

        serializable_interface.add_element(
            CppPureVirtualFunction("void", "unserialize", ["const Cake::Serialization::Fc &"], False)
        )

        serializable_interface.add_element(
            CppPureVirtualFunction("std::string", "toString", [], True)
        )

        self.cpp_namespace.add_element(serializable_interface)

    def __generate_structures(self):
        print("generating structures")
        for struct in self.structs:
            cpp_struct = MessageGenerator(struct, False)
            code = cpp_struct.generate()
            self.cpp_namespace.add_element(CppCustomCode(code))

    def __generate_messages(self):
        print("generating message structures")
        for message in self.messages:
            cpp_struct = MessageGenerator(message, True)
            code = cpp_struct.generate()
            self.cpp_namespace.add_element(CppCustomCode(code))

    def __generate_message_factory(self):
        print("generating message factory")
        message_factory = MessageFactoryGenerator(self.messages)
        code = message_factory.generate()
        self.cpp_namespace.add_element(CppCustomCode(code))

    def __generate_handler_caller(self):
        print("generating handler caller")
        handler_caller = HandlerCallerGenerator(self.messages)

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
    def __init__(self, message, is_message):
        self.message = message
        self.is_message = is_message

    def generate(self):
        inherits = []
        if self.is_message:
            print("\tmessage " + self.message.id)
            inherits = ["AbstractMessage"]
        else:
            print("\tstructure " + self.message.id)
            inherits = ["ISerializable"]

        cpp_struct = CppStruct(
            name = self.message.id,
            inherits = inherits,
            virtual_destructor = False,
            default_constructor = True
        )

        cpp_struct.add_element(self.__generate_params())
        cpp_struct.add_element(self.__generate_lists())
        cpp_struct.add_element(self.__generate_specialized_constructor())
        cpp_struct.add_element(self.__generate_eq_operator())
        cpp_struct.add_element(self.__generate_ne_operator())

        if self.is_message:
            cpp_struct.add_element(self.__generate_getid_method())

        cpp_struct.add_element(self.__generate_serialize_method())

        cpp_struct.add_element(self.__generate_unserialize_method())
        cpp_struct.add_element(self.__generate_unserialize_from_string_method())
        cpp_struct.add_element(self.__generate_to_string_method())

        return cpp_struct.code()

    def __generate_getid_method(self):
        return CppFunction(
            return_type = "Id::Messages",
            name = "getId",
            arguments = [],
            body = "return Id::" + self.message.id + ";",
            const = True
        )

    def __generate_serialize_method(self):
        s = "Cake::Networking::Protocol::BinaryCoder coder(buffer);\n"

        if len(self.message.params) > 0 or len(self.message.lists) > 0 or self.is_message:
            s += "coder"

            if self.is_message:
                s += " << getId()"

            for param in self.message.params:
                s += " << " + param.name

            for param in self.message.lists:
                s += " << " + param.name

            s += ";"

        return CppFunction(
            return_type = "void",
            name = "serialize",
            arguments = ["Cake::Networking::Protocol::IWriteBuffer & buffer"],
            const = True,
            body = s
        )

    def __generate_unserialize_method(self):
        s = "Cake::Networking::Protocol::BinaryDecoder decoder(buffer);\n"

        if len(self.message.params) > 0 or len(self.message.lists) > 0:
            s += "decoder"

            for param in self.message.params:
                s += " >> " + param.name

            for param in self.message.lists:
                s += " >> " + param.name

            s += ";"

        return CppFunction(
            return_type = "void",
            name = "unserialize",
            arguments = ["Cake::Networking::Protocol::IReadBuffer & buffer"],
            body = s
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

    def __generate_lists(self):
        ret = CppContainer()

        for param in self.message.lists:
            print("\t\t" + param.name + ": list of " + param.type)
            cpp_type = "std::vector<" + param.type + ">"
            ret.add_element(CppVariableDeclaration(cpp_type, param.name))

        return ret

    def __generate_specialized_constructor(self):
        ret = CppCustomCode("")

        if len(self.message.params) > 0 or len(self.message.lists) > 0:
            arguments = []
            initialization_list = []
            for param in self.message.params:
                arguments.append("const " + param.cpp_type + " & _" + param.name)
                initialization_list.append(("_" + param.name, param.name))

            for param in self.message.lists:
                arguments.append("const std::vector<" + param.type + "> & _" + param.name)
                initialization_list.append(("_" + param.name, param.name))

            ret = CppConstructor(
                name = self.message.id,
                arguments = arguments,
                initialization_list = initialization_list
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
        s = s + "\tstatic std::shared_ptr<AbstractMessage> create("
        s = s + "Cake::Networking::Protocol::IReadBuffer & buffer)\n"
        s = s + "\t{\n"
        s = s + "\t\tCake::Networking::Protocol::BinaryDecoder decoder(buffer);\n"
        s = s + "\t\tint id = Id::InvalidMessage;\n"
        s = s + "\t\tdecoder >> id;\n"
        s = s + "\t\tstd::shared_ptr<AbstractMessage> ret;\n"
        s = s + "\n"
        s = s + "\t\tswitch(id)\n"
        s = s + "\t\t{\n"

        for message in self.messages:
            s = s + self.__generate_message_case(message)

        s = s + "\t\tdefault:\n"
        s = s + "\t\t\tthrow std::runtime_error(\"unknown message id\");\n"
        s = s + "\t\t} // switch\n"
        s = s + "\n"
        s = s + "\t\tret->unserialize(buffer);\n"
        s = s + "\n"
        s = s + "\t\treturn ret;\n"
        s = s + "\t} // create\n"
        s = s + "\n"
        return s

    def __generate_message_case(self, message):
        s = ""
        s = s + "\t\tcase Id::" + message.id + ":\n"
        s = s + "\t\t\tret.reset(new " + message.id + "());\n"
        s = s + "\t\t\tbreak;\n"
        s = s + "\n"
        return s

    def __generate_create_from_string_method(self):
        s = (""
             "    static std::shared_ptr<AbstractMessage> create(const std::string & s)\n"
             "    {\n"
             "        std::shared_ptr<AbstractMessage> ret;\n"
             "        Cake::Serialization::Fc fc(s);\n")

        for message in self.messages:
            s = s + ("        if (fc.getName() == \"" + message.id + "\")\n"
                     "        {\n"
                     "            ret.reset(new " + message.id + "());\n"
                     "        }\n")

        s = s + ("   if (!ret)\n"
                 "   {\n"
                 "       throw std::runtime_error(\"no such message\");\n"
                 "   }\n"
                 "   ret->unserialize(fc);\n"
                 "   return ret;\n"
                 "}\n")
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
        s = s + "\tvoid call(AbstractMessage & message"

        for i in range(parameter_count):
            s = s + ", T" + str(i) + " p" + str(i)

        s = s + ")\n"
        s = s + "\t{\n"
        s = s + "\t\tswitch(message.getId())\n"
        s = s + "\t\t{\n"
        return s

    def __generate_message_case(self, parameter_count, message):
        s = ""
        s = s + "\t\tcase Id::" + message.id + ":\n"
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

msgs = []
for message_xml_node in messages.childNodes[0].getElementsByTagName("message"):
    message = Struct(message_xml_node) 
    msgs.append(message)

hpp = Generator(structs, msgs, options.output, options.namespace)
hpp.generate()


