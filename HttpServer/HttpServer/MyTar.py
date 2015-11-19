import tarfile
import os
import tarfile
import time
# dabao
def tar(fname):
    t = tarfile.open(fname+".tar", "w")
    for root, dir, files in os.walk(fname):
        print root, dir, files
        for file in files:
            fullpath = os.path.join(root, file)
            t.add(fullpath)
    t.close()

def tar_one(fname):
    l = len(fname)
    fileName = ''
    for v in xrange(l):
        t = l-v-1
        nPos = fname[t]
        if '\\' == nPos or '/' == nPos:
            fileName = fname[t+1:]
    t = tarfile.open(fname[:-4]+".tar", "w")
    #if os.walk(""):
    t.add(fileName)
    print fileName
    t.close()
    return True

# jiebao
def utar(fname, toPath=''):
    start = time.time()
    t = tarfile.open(fname, "r:")
    t.extractall(path = toPath)
    t.close()
    return time.time()-start
#tar("EBDT_R")
#tar_one("XML/EBDB_100000000001.xml")
#print utar("xx.tar")
#print "OK"




