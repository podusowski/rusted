#! /usr/bin/env python3

import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-o", "--output", required=True)
options = parser.parse_args()

f = open(options.output, "w")

f.write(
    'BEGIN;\n'
    'INSERT INTO users VALUES(1, "user1", "5baa61e4c9b93f3f0682250b6cf8331b7ee68fd8");\n'
)

for i in range(10000):
    #                               id              type        class  model             x     y     z      owner integrity carbon helium
    f.write(
        'INSERT INTO objects VALUES('+ str(i+1) +', "Ship",     1,     NULL,             1,    1,    1,     1,    100,      10,    10);\n'
    )

f.write(
    '/*                              id   speed   max_integrity  name             model                 max_capacity  required_carbon  required_helium */\n'
    'INSERT INTO ship_classes VALUES(1,   1,      100,           "Ship1",         "Ship1.model",        1000,         10,              5);\n'
    'COMMIT;\n'
)

