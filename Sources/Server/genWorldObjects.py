#! /usr/bin/env python

import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-o", "--output", required=True)
options = parser.parse_args()

f = open(options.output, "w")

f.write(
    'BEGIN;\n'
)

# INSERT INTO objects VALUES(100, "Asteroid", NULL,  "Asteroid.model", 2000, 1,    1,     NULL, 100,      1000,  1000);
for i in xrange(10000):
    radius = 10000
    x = random.randrange(-radius, radius)
    y = random.randrange(-radius, radius)
    z = random.randrange(-radius, radius)
    #                               id              type        class  model             x     y     z      owner integrity carbon helium
    f.write(
        'INSERT INTO objects VALUES('+ str(i+1) +', "Ship",     1,     NULL,             1,    1,    1,     1,    100,      10,    10);\n'
    )

f.write(
    '/*                              id   speed   max_integrity  name             model                 max_capacity  required_carbon  required_helium */\n'
    'INSERT INTO ship_classes VALUES(1,   1,      100,           "Ship1",         "Ship1.model",        1000,         10,              5);\n'
    )

f.write(
    'COMMIT;\n'
)

