import face_recognition
import cv2
import numpy as np


# 打开摄像头
video_capture = cv2.VideoCapture(0)

# 加载一张人脸图片  jpg格式
syf_image = face_recognition.load_image_file(r"/home/pi/Downloads/face_recognition-1.2.3/tests/test_images/obama.jpg")

# 获取面部编码 维度为0 的数据
syf_face_encoding = face_recognition.face_encodings(syf_image)[0]


# 编辑编码
known_face_encodings = [
    syf_face_encoding,
]
known_face_names = [
    "syf"
]

# 初始化变量 
face_locations = []
face_encodings = []
face_names = []
process_this_frame = True

while True:
    # 打开摄像头
    ret, frame = video_capture.read()

    # 将图片缩小为0.25 
    small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)

    # 把RGB变成BGR 因为cv只适应BGR
    rgb_small_frame = small_frame[:, :, ::-1]

    # Only process every other frame of video to save time
    if process_this_frame:
        # Find all the faces and face encodings in the current frame of video
		# locations是定位所以人脸 
        face_locations = face_recognition.face_locations(rgb_small_frame)
		# 得到当前帧face的编码
        face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)

		
        face_names = []
        for face_encoding in face_encodings:
            # See if the face is a match for the known face(s)
			# 比较两者人脸编码
            matches = face_recognition.compare_faces(known_face_encodings, face_encoding, tolerance=0.6)
            name = "Unknown"

            # # If a match was found in known_face_encodings, just use the first one.
            # if True in matches:
            #     first_match_index = matches.index(True)
            #     name = known_face_names[first_match_index]

            # Or instead, use the known face with the smallest distance to the new face
            face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
            best_match_index = np.argmin(face_distances)
            if matches[best_match_index]:
                name = known_face_names[best_match_index]

            face_names.append(name)

    process_this_frame = not process_this_frame


    # Display the results
    for (top, right, bottom, left), name in zip(face_locations, face_names):
        # Scale back up face locations since the frame we detected in was scaled to 1/4 size
        top *= 4
        right *= 4
        bottom *= 4
        left *= 4

        # Draw a box around the face
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

        # Draw a label with a name below the face
        cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
        font = cv2.FONT_HERSHEY_DUPLEX
        cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)

    # Display the resulting image
    cv2.imshow('Video', frame)

    # Hit 'q' on the keyboard to quit!
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release handle to the webcam
video_capture.release()
cv2.destroyAllWindows()
