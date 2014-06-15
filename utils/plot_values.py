import matplotlib.pyplot as plt
from os import sys

lines = open(sys.argv[1]).readlines()[1:]
lines = [ x.split(' ') for x in lines ]

shift = 0
try:
    shift = int(sys.argv[2])
except:
    pass

lines = [ [ float(y.replace("\n", "")) for y in x] for x in lines ]

xx = [x[1] for x in lines]
astar = [x[2 + 0 + shift] for x in lines]
crow = [x[2 + 3 + shift] for x in lines]
ambush = [x[2 + 6 + shift] for x in lines]

plt.plot(xx, astar, 'r--', xx, crow, 'b-.', xx, ambush, 'g-')
plt.show()
