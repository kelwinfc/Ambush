from sys import argv
import errno
import os

import matplotlib.delaunay as triang
from skimage import morphology
import numpy as np
import cv2

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
    
def medial_axis(img):
    rows, cols = img.shape
    for x in xrange(rows):
        img[x][0] = 0
        img[x][cols-1] = 0
    for x in xrange(cols):
        img[0][x] = 0
        img[rows-1][x] = 0


    skel, distance = morphology.medial_axis(img, return_distance=True)
    skel = [ [ 255 if y else 0 for y in x] for x in skel]
    skel = np.array(skel)

    return skel, distance

if len(argv) < 2:
    sys.exit(-1)

path = "results/medial_axis/" + argv[1].split("/")[-1].split(".")[0] + "/"
print "Output path:", path

try:
    os.mkdir(path)
except OSError as exc:
    if exc.errno == errno.EEXIST and os.path.isdir(path):
        pass

filename = argv[1]
img = cv2.imread(filename)
cv2.imwrite(path + 'src.jpg', img)

gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
gray = cv2.threshold(gray, 20, 255, cv2.THRESH_BINARY)[1]

kernel = np.ones((3,3), np.uint8)
gray = cv2.erode(gray, kernel, iterations = 1)

cv2.imwrite(path + 'binarized.jpg', gray)
print "Image binarized"

ma, distance = medial_axis(gray)

cv2.imwrite(path + 'medial_axis.jpg',  ma)
cv2.imwrite(path + 'distance_transform.jpg',  distance)

print "Medial axis computed"
