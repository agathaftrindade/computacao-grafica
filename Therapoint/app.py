# from imutils import face_utils
import numpy as np
import dlib
import cv2
import time
from imutils import face_utils

from collections import OrderedDict

from face_processing import process_face_image

cap = cv2.VideoCapture(0)

# detector = dlib.get_frontal_face_detector()
# predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('./models/shape_predictor_68_face_landmarks.dat')

while True:
    _, image = cap.read()

    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    rects = detector(gray, 0)

    for (i, rect) in enumerate(rects):
        upper_left, lower_right = (rect.left(), rect.top()), (rect.right(), rect.bottom())
        cv2.rectangle(image, upper_left, lower_right, (0, 255, 0), 1)

        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        for (name, (i, j)) in face_utils.FACIAL_LANDMARKS_IDXS.items():

            # clone the original image so we can draw on it, then
            # display the name of the face part on the image
            print(f'found {name} at {(i, j) }')

            for (x, y) in shape[i:j]:
                cv2.circle(image, (x, y), 1, (0, 0, 255), -1)

        process_face_image(image, shape)

    cv2.imshow("Output", np.flip(image, axis=1))

    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()
