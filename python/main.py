import numpy as np
from PIL import Image, ImageOps
import os
#import cv2
directory = "./photo/"
widh_screen, height_screen = 800, 480
colorpalet7color = np.array([[0, 0, 0], [255, 255, 255], [0, 255, 0],
                             [0, 0, 255], [255, 0, 0], [255, 255, 0],
                             [255, 165, 0]])


def resize_imag(img, height_screen, widh_screen):
    widh, height, = img.size
    if height / widh < height_screen / widh_screen:
        img = scale_h_crop_w_imaq(img, height_screen, widh_screen)
    else:
        img = scale_w_crop_h_imaq(img, height_screen, widh_screen)
    return img


def scale_h_crop_w_imaq(img, new_height, new_widh):
    widh, height, = img.size
    new = int(widh * new_height / height)
    img = img.resize((new, new_height))
    left, right = int((new - new_widh) / 2), new_widh + int(
        (new - new_widh) / 2)
    top, bottom = 0, new_height
    img = img.crop((left, top, right, bottom))
    return img


def scale_w_crop_h_imaq(img, new_height, new_widh):
    widh, height, = img.size
    new = int(height * new_widh / widh)
    img = img.resize((new_widh, new))
    left, right = 0, new_widh
    top, bottom = int((new - new_height) / 2), new_height + int(
        (new - new_height) / 2)
    img = img.crop((left, top, right, bottom))
    return img


def Floyd_Steinberg_dithering(img, colorpalet):
    widh, height, = img.size
    num = np.copy(np.asarray(img)) / 255.
    out = np.zeros((height, widh), dtype=np.uint8)

    for y in range(widh):
        if y % int(widh / 5) == 1:
            print(int(y / widh * 100), "%")

        for x in range(height):

            oldpixel = np.copy(num[x, y])
            newpixel, idx = closest_palet_color(oldpixel,
                                                colorpalet7color / 255.)
            num[x, y] = np.copy(newpixel)
            out[x, y] = np.uint8(idx)
            error = (oldpixel - newpixel)
            if x < height - 1:
                num[x + 1, y] = num[x + 1, y] + error * 7 / 16.
            if y < widh - 1:
                num[x - 1, y + 1] = num[x - 1, y + 1] + error * 3 / 16.
                num[x, y + 1] = num[x, y + 1] + error * 5 / 16.
                if x < height - 1:
                    num[x + 1, y + 1] = num[x + 1, y + 1] + error * 1 / 16.
    img = Image.fromarray(np.uint8(num * 255), "RGB")
    return img, out


def closest_palet_color(pixel, colorpalet):
    idx = (np.linalg.norm((colorpalet - pixel), axis=1)).argmin()
    return colorpalet[idx], idx


def shrink(In):
    x, y = In.shape
    out = np.zeros((x, int(y / 2)), dtype=np.uint8)
    for i in range(x):
        for j in range(int(y / 2)):
            out[i, j] = In[i, 2 * j] << 4 | In[i, 2 * j + 1]
    return bytes(out)


def deshrink(In):
    In = np.reshape(np.asarray(bytearray(In)),
                    (height_screen, int(widh_screen / 2)))
    x, y = In.shape
    out = np.zeros((x, int(y * 2), 3), dtype=np.uint8)
    for i in range(x):
        for j in range(int(y * 2)):
            temp = (In[i, int(j / 2)] & 0b11110000) >> 4
            temp1 = (In[i, int(j / 2)] & 0b1111)
            if j % 2 == 0:
                out[i, j] = np.copy(colorpalet7color[temp])
            else:
                out[i, j] = np.copy(colorpalet7color[temp1])
    return out


try:
    os.mkdir("out")
    os.mkdir("outdata")
except:
    pass

for file in os.listdir(directory):
    if file.endswith(".JPG") or file.endswith(".jpg"):
        print(file)
        img = Image.open(directory + file)
        img = ImageOps.exif_transpose(img)
        img = resize_imag(img, height_screen, widh_screen)
        img, out = Floyd_Steinberg_dithering(img, colorpalet7color)
        out = shrink(out)
        with open("./outdata/" + file[0:-3] + "dat", "wb") as binary_file:
            binary_file.write(out)
        img.save("./out/" + file)
