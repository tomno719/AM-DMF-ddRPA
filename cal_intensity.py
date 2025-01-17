import cv2
import numpy as np
import tkinter as tk
from tkinter import filedialog
import matplotlib.pyplot as plt

def open_image_file():
    # 弹出文件选择对话框
    file_path = filedialog.askopenfilename(title="Select an image file",
                                           filetypes=[("Image files", "*.jpg;*.png;*.bmp;*.gif")])
    return file_path

def process_image(image_path):
    # 读取图像
    image = cv2.imread(image_path)
    if image is None:
        print("Error: Unable to load image.")
        return

        # 转换颜色空间从BGR到HSV
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    # 定义HSV范围
    lower_green = np.array([10, 0, 10]) #TAMRA
    upper_green = np.array([60, 255, 100]) #TAMRA
   # lower_green = np.array([40, 0, 40])  # FITC
     # upper_green = np.array([80, 255, 255])  # FITC

    # 创建颜色掩膜
    mask = cv2.inRange(hsv, lower_green, upper_green)


    # 对掩膜进行形态学操作，以消除噪点
    kernel = np.ones((5, 5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

    # 查找轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    average_brightness_per_blob = []
    # 遍历每个轮廓
    for contour in contours:
        # 获取轮廓的边界框
        x, y, w, h = cv2.boundingRect(contour)

        # 从图像中提取出边界框内的区域
        blob_image = image[y:y + h, x:x + w]

        # 提取亮度通道（在BGR中，亮度对应于V通道，但在这里我们直接使用原始图像）
        blob_brightness = blob_image[:, :, 2]  # 假设image是BGR格式的，第三通道是亮度

        # 计算亮度平均值
        average_brightness = np.mean(blob_brightness)

        # 将平均亮度添加到列表中
        average_brightness_per_blob.append(average_brightness)

        # 打印每个色块的平均亮度

    for i, avg_brightness in enumerate(average_brightness_per_blob):
        print(f"Average brightness of blob {i + 1}: {avg_brightness}")

    # 绘制荧光强度图
    draw_intensity(average_brightness_per_blob)

    # 计算色块数量
    num_blobs = len(contours)
    print(f"Number of blobs: {num_blobs}")

    # 复制原始图像用于绘制轮廓
    colored_image = image.copy()

    # 用红色线条绘制轮廓
    cv2.drawContours(colored_image, contours, -1, (0, 0, 255), 1)  # (0, 0, 255) 是BGR中的红色

    # 显示标记后的图像
    cv2.imshow('Colored Blobs', colored_image)
    cv2.waitKey(0)  # 等待按键
    cv2.destroyAllWindows()  # 销毁所有OpenCV窗口

    # 导出标记后的图片
    save_path = filedialog.asksaveasfilename(defaultextension=".jpg",
                                             filetypes=[("JPEG files", "*.jpg"), ("All files", "*.*")])
    if save_path:
        cv2.imwrite(save_path, colored_image)
        print(f"Marked image saved as '{save_path}'")

    # 创建主窗口
def draw_intensity(average_brightness_per_blob):
    plt.figure()

    # 绘制点图，x轴是色块的索引，y轴是亮度值
    plt.scatter(range(len(average_brightness_per_blob)), average_brightness_per_blob)

    # 设置x轴和y轴的标签
    plt.xlabel('Blob Index')
    plt.ylabel('Average Brightness')

    # 设置标题
    plt.title('Average Brightness of Each Blob')

    # 显示图形
    plt.show()

root = tk.Tk()
root.withdraw()  # 隐藏主窗口

# 获取用户选择的图片路径
image_path = open_image_file()
if image_path:
    process_image(image_path)
