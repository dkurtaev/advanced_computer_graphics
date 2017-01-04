import matplotlib.pyplot as plt

xs = []
ys = []
for lod in range(6):
    xs.append([])
    ys.append([])

with open('out.txt', 'rt') as f:
    for line in f:
        data = line.rstrip('\n').split()
        n_spheres = int(data[0])
        lod = int(data[1])
        time = float(data[2])
        xs[lod].append(n_spheres)
        ys[lod].append(time)

for lod in reversed(range(6)):
    plt.plot(xs[lod], ys[lod], label='LOD = %d' % lod)

plt.xlabel('Number of spheres')
plt.ylabel('avg. time of rendering, ms. (128x128)')
plt.legend(loc=2)
plt.grid(True)
plt.savefig('plot.png')
