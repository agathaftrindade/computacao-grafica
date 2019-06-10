# from imutils import face_utils
import numpy as np
import dlib
import cv2
from datetime import datetime
from imutils import face_utils

from collections import OrderedDict

from face_processing import process_face_image

from skimage.measure import compare_ssim


cap = cv2.VideoCapture(0)

# detector = dlib.get_frontal_face_detector()
# predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('./models/shape_predictor_68_face_landmarks.dat')

last_detection = {}

while True:
    _, image = cap.read()

    # image = np.flip(image, axis=1)

    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    gray_comp = gray.copy()
    last_gray_comp = last_detection.get('gray_image', [])

    should_reuse = False

    if len(last_gray_comp) > 0:

        upper_left = last_detection.get('coords_ul')
        lower_right = last_detection.get('coords_lr')
        cv2.rectangle(gray_comp, upper_left, lower_right, (0, 0, 0), -1)

        # cv2.imshow("gray", last_gray_comp)
        (score, diff) = compare_ssim(gray_comp, last_gray_comp, full=True)
        diff = (diff * 255).astype("uint8")
        should_reuse = score > 0.6
        print(score)

    shape = last_detection.get("shape", []) if should_reuse else []

    rects = detector(gray, 0)
    rect = rects[0] if len(rects) > 0 else None

    if rect:
        upper_left, lower_right = (rect.left(), rect.top()), (rect.right(), rect.bottom())
        cv2.rectangle(image, upper_left, lower_right, (0, 255, 0), 1)

        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        cv2.rectangle(gray_comp, upper_left, lower_right, (0, 0, 0), -1)

        last_detection = {
            "time": datetime.now(),
            "shape": shape,
            "gray_image": gray_comp,
            "coords_ul": upper_left,
            "coords_lr": lower_right
        }

    for (x, y) in shape:
        cv2.circle(image, (x, y), 1, (0, 0, 255), -1)

    if len(shape) > 0:
        process_face_image(image, shape)

    cv2.imshow("Output", image)

    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()
