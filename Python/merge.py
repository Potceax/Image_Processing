from PIL import Image
from typing import Callable
import time

N_REPS = 10
PREP_FILE_PATH = "mergeFile.txt"

def blend_images(image_path1, image_path2, alpha=0.5):
    # Open the images
    image1 = Image.open(image_path1)
    image2 = Image.open(image_path2)

    # Ensure images are the same size
    if image1.size != image2.size:
        image2 = image2.resize(image1.size)

    # Blend the images
    blended_image = Image.blend(image1, image2, alpha)
    return blended_image
    

def measure_time(function: Callable, *args, n_repetitions: int = N_REPS) -> float:
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


if __name__== "__main__":
    # output in text variable should be formated this way:
    # "image_path (string), output_path (string), radius (int)"
    with open(PREP_FILE_PATH, "rt", encoding="utf8") as file:
        text = file.read()

    # dictionary containing initial values
    text_list = [name.strip() for name in text.split(',')]
    text_dir = { "image_path1": text_list[0], "image_path2": text_list[1], "output_path": text_list[2] }
    

    input_image_path1 = text_dir["image_path1"]
    input_image_path2 = text_dir["image_path2"]
    blend_output_path = text_dir["output_path"]

    print(f'total time: {measure_time(blend_images, input_image_path1, input_image_path2)} s')

    # Example usage
    blended_image = blend_images(input_image_path1, input_image_path2)
    blended_image.save(blend_output_path)
