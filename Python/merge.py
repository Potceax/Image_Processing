from PIL import Image

def blend_images(image_path1, image_path2, output_path, alpha=0.5):
    # Open the images
    image1 = Image.open(image_path1)
    image2 = Image.open(image_path2)

    # Ensure images are the same size
    if image1.size != image2.size:
        print("Images are not the same size. Resizing the second image to match the first.")
        image2 = image2.resize(image1.size)

    # Blend the images
    blended_image = Image.blend(image1, image2, alpha)

    # Save the new image
    blended_image.save(output_path)

# Example usage
blend_images('chrupiaca.jpg', 'mam.jpg', 'out.jpg')
