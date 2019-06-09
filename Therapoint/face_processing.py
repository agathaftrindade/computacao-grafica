import cv2

from enum import Enum

X = 0
Y = 1

class InstructionType(Enum):
    PRESS = 0,
    DIRECTED = 1

def p(i):
    return i - 1

def side_of_nose(img, face_points):
    # Press into side of nose
    points = face_points[p(36)][X], face_points[p(30)][Y]
    cv2.circle(img, points, 5, (255,0, 0), -1)

    points = face_points[p(32)][X], face_points[p(30)][Y]
    cv2.circle(img, points, 5, (255,0, 0), -1)

def process_face_image(img, face_points):
    side_of_nose(img, face_points)
