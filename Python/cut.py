import cv2
import numpy as np

# tutaj dam klasy ksztaltow
# class Shape(abstract):


def cut_and_leave_shape_opencv(image, shape, position, size):
    """
    Cuts a white shape out of the given image using OpenCV, leaving the rest of the image intact.

    :param image: Image array loaded by OpenCV
    :param shape: String, type of shape ('circle', 'square', 'triangle')
    :param position: Tuple (x, y), the center for circle/square or the first vertex of triangle
    :param size: Integer, diameter for circle, width/height for square, or size for triangle
    :return: Image with the shape in white
    """
    # Create a mask with the same size as the image
    mask = np.full(image.shape[:2], 0, dtype=np.uint8)  # Black mask

    if shape == 'circle':
        # Create a white circle on the mask
        cv2.circle(mask, position, size // 2, (255, 255, 255), -1)
    elif shape == 'square':
        # Create a white square on the mask
        top_left = (position[0] - size // 2, position[1] - size // 2)
        bottom_right = (position[0] + size // 2, position[1] + size // 2)
        cv2.rectangle(mask, top_left, bottom_right, (255, 255, 255), -1)
    elif shape == 'triangle':
        # Create a white triangle on the mask
        vertices = np.array([[
            position,
            (position[0] + size, position[1]),
            (position[0], position[1] + size)
        ]], dtype=np.int32)
        cv2.fillPoly(mask, vertices, (255, 255, 255))

    # Invert the mask to get a mask with a white shape on a black background
    mask_inv = cv2.bitwise_not(mask)

    # Cut the shape from the image and add a white shape in its place
    image_white_shape = cv2.bitwise_or(image, image, mask=mask_inv)
    image_white_shape[mask == 255] = [255, 255, 255]  # Set the shape color to white

    return image_white_shape

if __name__ == "__main__":    
    image_path = 'Maklowicz.jpg'
    image = cv2.imread(image_path)

    # Cutting a white circle shape out of the image at a given position
    cut_image_with_white_circle = cut_and_leave_shape_opencv(image, 'triangle', (100, 100), 200)

    # Save the modified image
    output_path_with_white_circle = 'Out.jpg'
    cv2.imwrite(output_path_with_white_circle, cut_image_with_white_circle)


