import cv2
import os

# 忽略 OpenCV 对不支持的摄像头控制的警告
os.environ["OPENCV_VIDEOIO_V4L2_WARN_NON_DEFAULT"] = "0"

# 打开设备（使用 V4L2 后端）
cap = cv2.VideoCapture("/dev/video1", cv2.CAP_V4L2)

# 强制设置摄像头支持的 UYVY 格式和分辨率
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('U','Y','V','Y'))  # UYVY 格式
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1536)

# 检查是否成功打开
if not cap.isOpened():
    print("Error: Could not open video device.")
    exit()

# 打印实际生效的参数（调试用）
print(f"实际分辨率: {cap.get(cv2.CAP_PROP_FRAME_WIDTH)}x{cap.get(cv2.CAP_PROP_FRAME_HEIGHT)}")
print(f"实际帧率: {cap.get(cv2.CAP_PROP_FPS)}")

# 读取并显示画面
while True:
    ret, frame = cap.read()
    if ret:
        cv2.imshow("Video Feed", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
