# -*- coding: utf-8 -*-
"""
Description: there is describe the script function
Environment: the script need the Environment
Example:     how to use the script
             $ python example_google.py
Author:      syf
Date:        2020.4.1
"""

import cv2
import numpy as np
import math
from collections import defaultdict
from PIL import Image,ImageDraw
from matplotlib.pyplot import imshow
import matplotlib.pyplot as plt
import face_recognition


def visualize_landmark(image_array, landmarks):
    """plot landmarks on image .
    brief:
    parameter:
        image_array: numpy array of a single image
        landmarks: dict of landmarks for facial parts as keys and tuple of coordinates as values
    return:
        plots of images with landmarks on
    example:
        visualize_landmark(image_array=cropped_face,landmarks=transferred_landmarks)
    """
    origin_img = Image.fromarray(image_array)
    draw = ImageDraw.Draw(origin_img)
    for facial_feature in landmarks.keys():
        draw.point(landmarks[facial_feature])
    imshow(origin_img)


def align_face(image_array, landmarks):
    """ align faces according to eyes position .
    brief:
    parameter:
        image_array: numpy array of a single image
        landmarks: dict of landmarks for facial parts as keys and tuple of coordinates as values
    return:
        rotated_img:  numpy array of aligned image
        eye_center: tuple of coordinates for eye center
        angle: degrees of rotation
    example:
        visualize_landmark(image_array=cropped_face,landmarks=transferred_landmarks)
    """
    # get list landmarks of left and right eye
    left_eye = landmarks['left_eye']
    right_eye = landmarks['right_eye']
    print(left_eye)
    # calculate the mean point of landmarks of left and right eye
    left_eye_center = np.mean(left_eye, axis=0).astype("int")
    right_eye_center = np.mean(right_eye, axis=0).astype("int")
    print(left_eye_center)
    # compute the angle between the eye centroids
    dy = right_eye_center[1] - left_eye_center[1]
    dx = right_eye_center[0] - left_eye_center[0]
    # compute angle between the line of 2 centeroids and the horizontal line
    angle = math.atan2(dy, dx) * 180. / math.pi
    # calculate the center of 2 eyes
    eye_center = ((left_eye_center[0] + right_eye_center[0]) // 2,
                  (left_eye_center[1] + right_eye_center[1]) // 2)
    # at the eye_center, rotate the image by the angle
    rotate_matrix = cv2.getRotationMatrix2D(eye_center, angle, scale=1)
    rotated_img = cv2.warpAffine(image_array, rotate_matrix, (image_array.shape[1], image_array.shape[0]))
    return rotated_img, eye_center, angle


def rotate(origin, point, angle, row):
    """ rotate coordinates in image coordinate system.
    brief:
    parameter:
        origin: tuple of coordinates,the rotation center
        point: tuple of coordinates, points to rotate
        angle: degrees of rotation
        row: row size of the image
    return:
        rotated coordinates of point
    example:
        visualize_landmark(image_array=cropped_face,landmarks=transferred_landmarks)
    """
    x1, y1 = point
    x2, y2 = origin
    y1 = row - y1
    y2 = row - y2
    angle = math.radians(angle)
    x = x2 + math.cos(angle) * (x1 - x2) - math.sin(angle) * (y1 - y2)
    y = y2 + math.sin(angle) * (x1 - x2) + math.cos(angle) * (y1 - y2)
    y = row - y
    return int(x), int(y)


def rotate_landmarks(landmarks, eye_center, angle, row):
    """ rotate landmarks to fit the aligned face.
     brief:
     parameter:
         landmarks: dict of landmarks for facial parts as keys and tuple of coordinates as values
         eye_center: tuple of coordinates for eye center
         angle: degrees of rotation
         row: row size of the image
     return:
         rotated_landmarks with the same structure with landmarks, but different values
     example:
        visualize_landmark(image_array=cropped_face,landmarks=transferred_landmarks)
     """
    rotated_landmarks = defaultdict(list)
    for facial_feature in landmarks.keys():
        for landmark in landmarks[facial_feature]:
            rotated_landmark = rotate(origin=eye_center, point=landmark, angle=angle, row=row)
            rotated_landmarks[facial_feature].append(rotated_landmark)
    return rotated_landmarks


def corp_face(image_array, size, landmarks):
    """  crop face according to eye,mouth and chin position.
     brief:
     parameter:
         image_array: numpy array of a single image
         size: single int value, size for w and h after crop
         landmarks: dict of landmarks for facial parts as keys and tuple of coordinates as values
     return:
         cropped_img: numpy array of cropped image
         left, top: left and top coordinates of cropping
     example:
        visualize_landmark(image_array=cropped_face,landmarks=transferred_landmarks)
     """
    x_min = np.min(landmarks['chin'], axis=0)[0]
    x_max = np.max(landmarks['chin'], axis=0)[0]
    x_center = (x_max - x_min) / 2 + x_min
    left, right = (x_center - size / 2, x_center + size / 2)

    eye_landmark = landmarks['left_eye'] + landmarks['right_eye']
    eye_center = np.mean(eye_landmark, axis=0).astype("int")
    lip_landmark = landmarks['top_lip'] + landmarks['bottom+lip']
    lip_center = np.mean(lip_landmark, axis=0).astype("int")
    mid_part = lip_center[1] - eye_center[1]
    top, bottom = eye_center[1] - (size - mid_part) / 2, lip_center[1] + (size - mid_part) / 2

    pil_img = Image.fromarray(image_array)
    left, top, right, bottom = [int(i) for i in [left, top, right, bottom]]
    cropped_img = pil_img.crop((left, top, right, bottom))
    cropped_img = np.array(cropped_img)
    return cropped_img, left, top


def transfer_landmark(landmarks, left, top):
    """ transfer landmarks to fit the cropped face.
     brief:
     parameter:
         landmarks: dict of landmarks for facial parts as keys and tuple of coordinates as values
         left: left coordinates of cropping
         top: top coordinates of cropping
     return:
        transferred_landmarks with the same structure with landmarks, but different values
     example:
        visualize_landmark(image_array=cropped_face,landmarks=transferred_landmarks)
     """
    transferred_landmarks = defaultdict(list)
    for facial_feature in landmarks.keys():
        for landmark in landmarks[facial_feature]:
            transferred_landmark = (landmark[0] - left, landmark[1] - top)
            transferred_landmarks[facial_feature].append(transferred_landmark)
    return transferred_landmarks


# Description: load image
# Attention:
image_array = cv2.imread(r'C:\Users\gzsyf\Pictures\Saved Pictures\girl2.png')

# Description: get frist man feature
# Attention:
face_landmarks_list = face_recognition.face_landmarks(image_array, model="large")
face_landmarks_dict = face_landmarks_list[0]

# Description: rotated face
# Attention:
aligned_face, eye_center, angle = align_face(image_array=image_array, landmarks=face_landmarks_dict)
rotated_landmarks = rotate_landmarks(landmarks=face_landmarks_dict, \
                                     eye_center=eye_center, angle=angle, row=image_array.shape[0])

# Description: cropped face
# Attention:
cropped_face, left, top = corp_face(image_array=aligned_face, size=140, landmarks=rotated_landmarks)
transferred_landmarks = transfer_landmark(landmarks=rotated_landmarks, left=left, top=top)

# Description: show pic
# Attention:
plt.subplot(122)
visualize_landmark(image_array=cropped_face,landmarks=transferred_landmarks)
plt.subplot(121)
imshow(Image.fromarray(image_array))
plt.show()

"""Reference
[1] code come from
https://zhuanlan.zhihu.com/p/55479744
"""