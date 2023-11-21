from PIL import Image, ImageFilter
from typing import Callable
import time

# blur image by pixels
def gaussian_blur(image_path, radius):
    image = Image.open(image_path)
    width, height = image.size
    blurred_image = Image.new("RGB", (width, height))

    for x in range(width):
        for y in range(height):
            r, g, b = 0, 0, 0
            count = 0
            for dx in range(-radius, radius + 1):
                for dy in range(-radius, radius + 1):
                    nx, ny = x + dx, y + dy
                    if 0 <= nx < width and 0 <= ny < height:
                        pixel = image.getpixel((nx, ny))
                        r += pixel[0]
                        g += pixel[1]
                        b += pixel[2]
                        count += 1

            r //= count
            g //= count
            b //= count
            blurred_image.putpixel((x, y), (r, g, b))

    return blurred_image

def optimized_gaussian_blur(image_path, radius):
    image = Image.open(image_path)
    blurred_image = image.filter(ImageFilter.GaussianBlur(radius))
    return blurred_image

def measure_time(function: Callable, *args, n_repetitions: int) -> float:
    """Function mesuring argument "function" time

    Args:
        function (Callable): function being timed
        n_repetitions (int): number of calls to function argument

    Returns:
        float: mesured time
    """
    times = []
    for _ in range(n_repetitions):
        t_0 = time.perf_counter()
        function(*args)
        t_1 = time.perf_counter()
        times.append(t_1 - t_0)
    mean_time = sum(times) / n_repetitions
    return mean_time

N_REPS = 5
PREP_FILE_PATH = "file.txt"


if __name__ == "__main__":
    # output in text variable should be formated this way:
    # "image_path (string), output_path (string), radius (int)"
    with open(PREP_FILE_PATH, "rt", encoding="utf8") as file:
        text = file.read()

    # dictionary containing initial values
    text_list = [name.strip() for name in text.split(',')]
    text_dir = { "image_path": text_list[0], "output_path": text_list[1], "radius": text_list[2] }
    

    input_image_path = text_dir["image_path"]
    gaussian_blur_path = text_dir["output_path"]
    radius = int(text_dir["radius"])  # Adjust the radius as needed
    
    # mesure time of gaussian_blur
    t_time = measure_time(optimized_gaussian_blur, input_image_path, radius, n_repetitions=N_REPS)
    print(f'gaussian blur: {t_time}')

    # create blurred image
    simple_blur_image = optimized_gaussian_blur(input_image_path, radius)
    simple_blur_image.save(gaussian_blur_path)
    print(f"Image blurred successfuly")

