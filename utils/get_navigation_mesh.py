from sys import argv
import errno
import os

import matplotlib.delaunay as triang
from skimage import morphology

import numpy as np
import cv2

from skimage import draw
import numpy as np
import matplotlib.pyplot as plt
from os import sys

def get_skeleton(img):
    
    rows, cols = img.shape
    
    for r in xrange(rows):
        img[r][0] = 0
        img[r][cols - 1] = 0
    for c in xrange(cols):
        img[0][c] = 0
        img[rows - 1][c] = 0
    
    dst = np.asarray(map(lambda row:
                         np.asarray(map(lambda x: 1 if x == 255 else 0,row)),
                     img))
    dst, _ = morphology.medial_axis(dst, return_distance=True)
    dst = np.asarray(map(lambda row:
                         np.asarray(map(lambda x: 255 if x else 0,row)),
                     dst))
    return dst

def get_neighbor(img, i, j, (di, dj)):
    rows, cols = img.shape
    ni = i + di
    nj = j + dj
    if 0 <= ni and ni < rows and 0 <= nj and nj < cols:
        return (ni,nj)
    return None

def belongs_to_skeleton(img, p):
    if p is None:
        return True

    (i,j) = p

    if img[i][j] == 255:
        return True
    
    return False

def count_neighbors(img, i, j):
    rows, cols = img.shape
    ret = 0
    
    """
    -1,-1 -1,+0, -1,+1
    +0,-1 +0,+0, +0,+1
    +1,-1 +1,+0, +1,+1
    """
    neighbors = [(-1,-1),(0,-1),(1,-1),(1,0),(1,1),(0,1),(-1,1),(-1,0)]
    
    for index, n in enumerate(neighbors):
        prev_n = get_neighbor(img, i, j, n)
        current_n = get_neighbor(img, i, j,
                                 neighbors[(index + 1) % len(neighbors)])
        
        if prev_n is None or current_n is None:
            continue
        
        if not belongs_to_skeleton(img, prev_n) and \
               belongs_to_skeleton(img, current_n):
            ret += 1
    
    return ret

def get_branching_points(img):    
    points = []
    
    rows, cols = img.shape
    
    for i in xrange(rows):
        for j in xrange(cols):
            if img[i][j] == 0:
                continue
            
            nn = count_neighbors(img, i, j)
            if nn > 2 or nn == 1:
                points.append((j,i))
    print len(points), "branching points"
    return points

def draw_branching_points(img, p):
    dst = np.zeros(img.shape, np.uint)
    cv2.merge([img, img, img], dst)
    dst = 255 - dst
    
    color = (0,0,255)
    for c in p:
        #dst[c[1]][c[0]] = color(0,0,255)
        cv2.circle(dst, c, 5, color, 2)
        
    return dst

if len(argv) < 2:
    sys.exit(-1)

path = argv[2]
if path[-1] != "/":
    path += "/"

try:
    os.mkdir(path)
except OSError as exc:
    if exc.errno == errno.EEXIST and os.path.isdir(path):
        pass

filename = argv[1]
ma = cv2.imread(filename, 0)
cv2.imwrite(path + "src.jpg", ma)

_, ma = cv2.threshold(ma, 100, 255, cv2.THRESH_BINARY)
cv2.imwrite(path + "dst.jpg", ma)

skeleton = get_skeleton(ma)

cv2.imwrite(path + "skeleton.jpg", skeleton)
