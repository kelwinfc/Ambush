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

p1, = plt.plot(xx, astar, 'r--')
p2, = plt.plot(xx, crow, 'b-*')
p3, = plt.plot(xx, ambush, 'g-')

plt.ylabel('Ambush degree')
plt.xlabel('Number of agents (log)')

plt.xlim(min(xx), max(xx))
plt.ylim(0.0, 1.0)

plt.legend([p1, p2, p3], ["A*", "van Toll et al.", "A*mbush"], loc='upper center',
           bbox_to_anchor=(0.5, 1.1), ncol=3
          )

#plt.legend(, ,
          #, fancybox=True, shadow=True)

plt.show()
