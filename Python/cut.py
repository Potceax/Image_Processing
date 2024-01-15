import cv2
import numpy as np
import os

class Shape:
    def __init__(self, position, color, size=1):
        self.position = position
        self.color = color
        self.size = size

    def create_shape(self, mask):
        raise NotImplementedError("Subclasses should implement this!")

class Circle(Shape):
    def __init__(self, position, color, size=1):
        super().__init__(position, color, size)
        self.radius = size // 2

    def create_shape(self, mask):
        cv2.circle(mask, self.position, self.radius, self.color, self.size)

class Square(Shape):
    def __init__(self, position, color, size=1):
        super().__init__(position, color, size)
        half_size = size // 2
        self.pt1 = (position[0] - half_size, position[1] - half_size)
        self.pt2 = (position[0] + half_size, position[1] + half_size)

    def create_shape(self, mask):
        cv2.rectangle(mask, self.pt1, self.pt2, self.color, self.size)

class Triangle(Shape):
    def __init__(self, position, color, size=1):
        super().__init__(position, color, size)
        self.vertices = [position, (position[0] + size, position[1]), (position[0], position[1] + size)]

    def create_shape(self, mask):
        cv2.fillPoly(mask, [np.array(self.vertices, dtype=np.int32)], self.color)

def cut_shape(image_path, shape):
    image = cv2.imread(image_path)
    mask = np.zeros(image.shape[:2], dtype=np.uint8)

    shape.create_shape(mask)

    mask_inv = cv2.bitwise_not(mask)
    image[mask_inv == 0] = [255, 255, 255]
    return image

def test_time(input_path, shape, reps=10):
    if shape is None:
        return 0.0

    times = []
    for _ in range(reps):
        start = cv2.getTickCount()
        cut_shape(input_path, shape)
        end = cv2.getTickCount()
        time = (end - start) / cv2.getTickFrequency()
        times.append(time)

    return sum(times) / len(times)

PREP_FILE_PATH = "cut_file.txt"

def main():
    # output in text variable should be formated this way:
    # "image_path (string), output_path (string), radius (int)"
    with open(PREP_FILE_PATH, "rt", encoding="utf8") as file:
        text = file.read()

    # dictionary containing initial values
    text_list = text.split(",")
    text_dir = { "image_path": text_list[0], "output_path": text_list[1], "Figure": text_list[2],
                 "position_x": text_list[3], "position_y": text_list[4], "size": text_list[5] }
    

    input_image_path = text_dir["image_path"]
    cut_path = text_dir["output_path"]


    image_path = "Maklowicz1.png"
    output_path = "Mk1.jpg"

    position = (int(text_dir["position_x"]), int(text_dir["position_y"]))
    white = (255, 255, 255)
    size = int(text_dir["size"])

    # circle = Circle(position, white, size)
    # square = Square(position, white, size)
    # triangle = Triangle(position, white, size)

    if text_dir["Figure"] == "Circle":
        current_shape = Circle(position, white, size)
    elif text_dir["Figure"] == "Square":
        current_shape = Square(position, white, size)
    elif text_dir["Figure"] == "Triangle":
        current_shape = Triangle(position, white, size)

    print(test_time(input_image_path, current_shape))
    cv2.imwrite(cut_path, cut_shape(input_image_path, current_shape))

if __name__ == "__main__":
    main()
