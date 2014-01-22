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

for i in xrange(10000):
    radius = 10000
    x = random.randrange(-radius, radius)
    y = random.randrange(-radius, radius)
    z = random.randrange(-radius, radius)
    f.write(
        'INSERT INTO objects(type, model, x, y, z, integrity, carbon, helium) VALUES("Asteroid", "Asteroid.model", ' + str(x) + ', ' + str(y) + ', ' + str(z) + '100, 10, 10);\n'
    )

f.write(
    'COMMIT;\n'
)

