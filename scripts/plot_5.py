import matplotlib.pyplot as plt
f = plt.figure()
plt.xlabel('log2(table size)')
plt.ylabel('miss prediction per k instructions')
i = 0
colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
plt.title('counter size 5')
plt.legend(loc='upper right')
plt.subplot().set_yscale('log', nonposy='clip')
plt.legend(loc='upper right')
f.savefig('graph_5', bbox_inches='tight')
