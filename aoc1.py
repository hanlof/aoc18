#!/usr/bin/env python
f = open("input-1")
n=[]
for l in f.readlines():
    n.append(int(l))
    c=0

print len(n)
print sum(n)

size=len(n)

fr=dict()
i=0
f=0
while True:
    f = f + n[i]
    if not fr.has_key(f):
        fr[f] = 0
    fr[f] = fr[f] + 1
    if fr[f] > 1:
        print "This is it!! %d" % f
        break
    i = i + 1
    i = i % size
