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
    # lower_green = np.array([10, 0, 10]) #TAMRA
    # upper_green = np.array([60, 255, 100]) #TAMRA
    lower_green = np.array([40, 40, 40])  # FITC
    upper_green = np.array([80, 255, 255])  # FITC

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
    test = [np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(3.9454545454545453), np.float64(3.93), np.float64(3.950617283950617), np.float64(3.933333333333333), np.float64(5.777089783281734), np.float64(3.98), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(8.35204081632653), np.float64(6.85), np.float64(4.0), np.float64(7.238095238095238), np.float64(6.684444444444445), np.float64(4.0), np.float64(6.790816326530612), np.float64(5.852941176470588), np.float64(5.220588235294118), np.float64(4.0), np.float64(4.0), np.float64(6.704761904761905), np.float64(5.435555555555555), np.float64(3.9761904761904763), np.float64(3.982142857142857), np.float64(3.9761904761904763), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(7.108333333333333), np.float64(3.95959595959596), np.float64(3.919191919191919), np.float64(4.0), np.float64(3.9857142857142858), np.float64(3.963636363636364), np.float64(7.702222222222222), np.float64(3.9338842975206614), np.float64(3.9761904761904763), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(6.84), np.float64(3.87603305785124), np.float64(3.963636363636364), np.float64(3.9363636363636365), np.float64(4.0), np.float64(6.68), np.float64(5.329166666666667), np.float64(6.168888888888889), np.float64(4.152380952380953), np.float64(6.695238095238095), np.float64(5.438095238095238), np.float64(4.0), np.float64(4.0), np.float64(7.690476190476191), np.float64(4.0), np.float64(3.95), np.float64(6.115555555555556), np.float64(4.0), np.float64(3.988888888888889), np.float64(4.0), np.float64(3.909090909090909), np.float64(4.0), np.float64(4.0), np.float64(3.977777777777778), np.float64(4.0), np.float64(3.9714285714285715), np.float64(3.9454545454545453), np.float64(6.35), np.float64(3.97), np.float64(4.0), np.float64(3.909090909090909), np.float64(4.0), np.float64(4.0), np.float64(3.9909090909090907), np.float64(3.9454545454545453), np.float64(3.9363636363636365), np.float64(3.9494949494949494), np.float64(3.9555555555555557), np.float64(7.685714285714286), np.float64(3.988888888888889), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(3.977777777777778), np.float64(7.571428571428571), np.float64(6.2625), np.float64(7.466666666666667), np.float64(3.965909090909091), np.float64(4.0), np.float64(4.0), np.float64(3.911111111111111), np.float64(3.918181818181818), np.float64(4.0), np.float64(4.0), np.float64(6.671428571428572), np.float64(5.493827160493828), np.float64(3.93), np.float64(3.98), np.float64(4.0), np.float64(3.986111111111111), np.float64(3.96), np.float64(3.93), np.float64(3.96), np.float64(3.9454545454545453), np.float64(3.95959595959596), np.float64(4.0), np.float64(4.0), np.float64(3.9363636363636365), np.float64(3.925619834710744), np.float64(3.963636363636364), np.float64(3.909090909090909), np.float64(3.9727272727272727), np.float64(7.511111111111111), np.float64(3.97), np.float64(4.0), np.float64(4.0), np.float64(3.96), np.float64(8.404761904761905), np.float64(4.0), np.float64(4.0), np.float64(3.9791666666666665), np.float64(3.9797979797979797), np.float64(3.9454545454545453), np.float64(4.0), np.float64(3.9454545454545453), np.float64(3.9454545454545453), np.float64(3.9722222222222223), np.float64(3.94), np.float64(3.9272727272727272), np.float64(4.0), np.float64(3.9338842975206614), np.float64(6.847058823529411), np.float64(4.0), np.float64(6.884444444444444), np.float64(4.672916666666667), np.float64(3.933333333333333), np.float64(3.9), np.float64(6.488888888888889), np.float64(6.14), np.float64(6.714285714285714), np.float64(7.035555555555556), np.float64(6.257777777777778), np.float64(7.591111111111111), np.float64(4.0), np.float64(3.9583333333333335), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(3.9363636363636365), np.float64(3.99), np.float64(4.0), np.float64(4.0), np.float64(6.915555555555556), np.float64(7.426666666666667), np.float64(3.9797979797979797), np.float64(4.0), np.float64(7.480952380952381), np.float64(6.075555555555556), np.float64(5.5323529411764705), np.float64(6.938095238095238), np.float64(5.635555555555555), np.float64(6.302222222222222), np.float64(6.377777777777778), np.float64(6.738095238095238), np.float64(5.315555555555555), np.float64(3.96), np.float64(7.266666666666667), np.float64(5.937777777777778), np.float64(5.816964285714286), np.float64(3.9696969696969697), np.float64(3.9722222222222223), np.float64(6.657777777777778), np.float64(4.171052631578948), np.float64(6.08), np.float64(6.542222222222223), np.float64(6.428571428571429), np.float64(5.119047619047619), np.float64(4.0), np.float64(6.296052631578948), np.float64(3.977777777777778), np.float64(3.9272727272727272), np.float64(4.0), np.float64(3.96), np.float64(3.984126984126984), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(3.9454545454545453), np.float64(3.9797979797979797), np.float64(3.909090909090909), np.float64(3.9753086419753085), np.float64(3.87603305785124), np.float64(4.0), np.float64(3.9523809523809526), np.float64(4.0), np.float64(3.97), np.float64(3.9555555555555557), np.float64(5.874509803921568), np.float64(6.982222222222222), np.float64(4.0), np.float64(3.986111111111111), np.float64(3.97), np.float64(3.9722222222222223), np.float64(4.0), np.float64(3.9761904761904763), np.float64(5.90625), np.float64(3.988888888888889), np.float64(3.9363636363636365), np.float64(3.94), np.float64(3.9696969696969697), np.float64(3.9876543209876543), np.float64(4.0), np.float64(3.9272727272727272), np.float64(3.9857142857142858), np.float64(4.0), np.float64(4.0), np.float64(3.9875), np.float64(3.988888888888889), np.float64(4.0), np.float64(3.9242424242424243), np.float64(4.0), np.float64(7.266666666666667), np.float64(6.152380952380953), np.float64(6.874509803921568), np.float64(4.0), np.float64(3.9545454545454546), np.float64(3.918181818181818), np.float64(4.0), np.float64(3.925619834710744), np.float64(3.9173553719008263), np.float64(3.925619834710744), np.float64(4.0), np.float64(3.933333333333333), np.float64(3.925619834710744), np.float64(3.9272727272727272), np.float64(3.933333333333333), np.float64(3.94), np.float64(3.9166666666666665), np.float64(3.9454545454545453), np.float64(4.0), np.float64(3.9454545454545453), np.float64(3.96), np.float64(3.95), np.float64(3.9454545454545453), np.float64(3.9393939393939394), np.float64(3.99), np.float64(3.94), np.float64(4.0), np.float64(3.977272727272727), np.float64(4.0), np.float64(3.925619834710744), np.float64(7.0476190476190474), np.float64(3.96), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(3.92), np.float64(7.035555555555556), np.float64(4.0), np.float64(4.0), np.float64(5.84453781512605), np.float64(6.1466666666666665), np.float64(7.261904761904762), np.float64(4.88265306122449), np.float64(4.796875), np.float64(6.777777777777778), np.float64(5.604444444444445), np.float64(3.977272727272727), np.float64(6.053333333333334), np.float64(4.0), np.float64(6.68), np.float64(5.3011695906432745), np.float64(4.0), np.float64(4.0), np.float64(4.04), np.float64(6.488888888888889), np.float64(6.371428571428571), np.float64(5.8), np.float64(5.75), np.float64(4.0), np.float64(4.0), np.float64(5.870833333333334), np.float64(3.9814814814814814), np.float64(6.942222222222222), np.float64(6.693333333333333), np.float64(4.0), np.float64(6.995238095238095), np.float64(3.977777777777778), np.float64(3.982142857142857), np.float64(4.0), np.float64(4.0), np.float64(4.0), np.float64(6.120535714285714), np.float64(3.9814814814814814), np.float64(6.942857142857143), np.float64(4.0), np.float64(6.75), np.float64(4.0), np.float64(4.0), np.float64(6.631111111111111), np.float64(4.0), np.float64(3.9444444444444446), np.float64(3.9292929292929295), np.float64(6.182662538699691), np.float64(4.0)]
    print( test )
    plt.scatter(range(len(test )), test )

    # 设置x轴和y轴的标签
    #plt.xlabel('Blob Index')
    #plt.ylabel('Average Brightness')

    # 设置标题
   # plt.title('Average Brightness of Each Blob')

    # 显示图形
    plt.show()

root = tk.Tk()
root.withdraw()  # 隐藏主窗口

# 获取用户选择的图片路径
image_path = open_image_file()
if image_path:
    process_image(image_path)
