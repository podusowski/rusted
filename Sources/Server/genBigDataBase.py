#! /usr/bin/env python

import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-o", "--output", required=True)
options = parser.parse_args()

f = open(options.output, "w")

f.write(
    "<root>\n"
    "   <users>\n"
    "       <user id=\"1\" login=\"user1\" password=\"passwordhash\" />\n"
    "   </users>\n"
    "   <objects>\n")

for i in xrange(10000):
    f.write(
        "       <object id=\"" + str(i+1) + "\" type=\"Ship\" class=\"1\" x=\"1\" y=\"1\" z=\"1\" owner=\"1\" />\n"
    )

f.write(
    "   </objects>\n"
    "   <ship_classes>\n"
    '       <ship_class id="1" speed="20" integrity="200" name="Ship2" model="Ship2.mesh" capacity="1000" required_carbon="10" required_helium="5" />\n'
    '   </ship_classes>\n'
    "</root>\n")
