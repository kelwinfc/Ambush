import cv2
import numpy as np
from sys import argv
from scipy.spatial import Delaunay
import matplotlib.pyplot as plt

import matplotlib.delaunay as triang
import pylab
import numpy
import heapq
from math import sqrt

def draw_corners(img, corners, v=0.5):
    dst = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
    dilated_corners = cv2.dilate(corners, None)
    dilated_corners = cv2.dilate(dilated_corners, None)
    
    dst[dilated_corners >= v * dilated_corners.max()] = [0,0,255]
    
    cv2.imshow('corners',dst)

def get_points(corners, v=0.5):
    def to_grid(p):
        xs = list(set([x for (x, _) in p]))
        ys = list(set([y for (_, y) in p]))
        xs.sort()
        ys.sort()
        
        print xs
        print ys
        
        p = []
        for idx, x in enumerate(xs):
            if idx > 0 and abs(xs[idx] - xs[idx-1]) < 5:
                continue
            
            for idy, y in enumerate(ys):
                if idy > 0 and abs(ys[idy] - ys[idy-1]) < 5:
                    continue
                
                p.append([y, x])
        return p
    
    max_v = corners.max()
    rows, cols = corners.shape
    ret = [[0, 0], [rows - 1,0], [0, cols - 1], [rows - 1, cols - 1]]
    
    for idx, x in enumerate(corners):
        for idy, y in enumerate(x):
            if y >= v * max_v:
                ret.append([idx, idy])
    
    return to_grid(ret)

def flip_points(points, rows, cols):
    return [ [rows - x[0], cols - x[1]] for x in points]

def draw_triangles(img, points, tri):

    dst = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
    
    for t in tri:
        t = np.array([points[t[0]], points[t[1]], points[t[2]]])
        cv2.polylines(dst, [t], True, (255,0,0))
    cv2.imshow("triangles", dst)
    
def dist_transform(img):
    def dist((x0,y0),(x1,y1)):
        dx = x1 - x0
        dy = y1 - y0
        
        return sqrt(dx * dx + dy * dy)
    
    ret = np.zeros(img.shape,np.float32)
    v = np.zeros(img.shape,np.uint8)
    q = []

    rows, cols = img.shape
    
    for idx, x in enumerate(img):
        for idy, y in enumerate(x):
            if y == 0:
                heapq.heappush(q, (0, (idx, idy), (idx, idy)))
                v[idx][idy] = 255
    
    for x in xrange(rows):
        heapq.heappush(q, (0, (x, 0), (x, 0)))
        heapq.heappush(q, (0, (x, cols - 1), (x, cols - 1)))
        v[x][0] = 255
        v[x][cols - 1] = 255
    
    for y in xrange(cols):
        heapq.heappush(q, (0, (0, y), (0, y)))
        heapq.heappush(q, (0, (rows - 1, y), (rows - 1, y)))
        v[0][y] = 255
        v[rows - 1][y] = 255
    
    while len(q) > 0:
        (d, (x, y), (px, py)) = heapq.heappop(q)
        
        ret[x][y] = d
        
        for dx in range(-1,2):
            for dy in range(-1,2):
                nx = x + dx
                ny = y + dy

                if 0 <= nx and nx < rows and 0 <= ny and ny < cols and\
                       v[nx][ny] == 0 and img[nx][ny] != 0:
                   v[nx][ny] = 255
                   heapq.heappush(q, (dist((nx, ny),(px, py)), (nx, ny),
                                      (px, py)))
    
    cv2.imshow("v", v)
    
    return cv2.normalize(ret,0,255)

filename = argv[1]
img = cv2.imread(filename)
cv2.imshow('src', img)

gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

_, gray = cv2.threshold(gray, 1, 255, cv2.THRESH_BINARY)
cv2.imshow('binarized', gray)

size = np.size(gray)
skel = np.zeros(gray.shape,np.uint8)

element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3))
done = False

d = dist_transform(gray)

cv2.imshow("skel",d)
cv2.waitKey(0)

#cv2.imshow("distance", distance)

gray = np.float32(gray)

corners = cv2.cornerHarris(gray,2,3,0.04)

print corners.shape
v = 0.01
draw_corners(gray, corners, v)
points = get_points(corners, v)

print "Num points:", len(points)
    
x = np.array([x for (x, _) in points])
y = np.array([y for (_, y) in points])

cens, edg, tri, neig = triang.delaunay(x,y)
draw_triangles(gray, points, tri)


#for t in tri:
  ## t[0], t[1], t[2] are the points indexes of the triangle
  #t_i = [t[0], t[1], t[2], t[0]]
  #pylab.plot(x[t_i],y[t_i])
#pylab.plot(x,y,'o')
cv2.waitKey(0)
#pylab.show()

