import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class ImageBlender {

    public static void main(String[] args) throws IOException {
        // Load the images
        BufferedImage image1 = ImageIO.read(new File("input1.png"));
        BufferedImage image2 = ImageIO.read(new File("input.png"));

        // Create a new image for the blended result
        BufferedImage blendedImage = new BufferedImage(image1.getWidth(), image1.getHeight(), BufferedImage.TYPE_INT_ARGB);

        // Blend the images
        for (int y = 0; y < image1.getHeight(); y++) {
            for (int x = 0; x < image1.getWidth(); x++) {
                int rgb1 = image1.getRGB(x, y);
                int rgb2 = image2.getRGB(x, y);

                int alpha1 = (rgb1 >> 24) & 0xff;
                int red1 =   (rgb1 >> 16) & 0xff;
                int green1 = (rgb1 >>  8) & 0xff;
                int blue1 =  (rgb1      ) & 0xff;

                int alpha2 = (rgb2 >> 24) & 0xff;
                int red2 =   (rgb2 >> 16) & 0xff;
                int green2 = (rgb2 >>  8) & 0xff;
                int blue2 =  (rgb2      ) & 0xff;

                // Blend the pixels
                float alphaBlend = 0.5f; // 50% alpha blend
                int alpha = (int) ((alpha1 * alphaBlend) + (alpha2 * (1 - alphaBlend)));
                int red = (int) ((red1 * alphaBlend) + (red2 * (1 - alphaBlend)));
                int green = (int) ((green1 * alphaBlend) + (green2 * (1 - alphaBlend)));
                int blue = (int) ((blue1 * alphaBlend) + (blue2 * (1 - alphaBlend)));

                int blendedRGB = (alpha << 24) | (red << 16) | (green << 8) | blue;
                blendedImage.setRGB(x, y, blendedRGB);
            }
        }

        // Save the blended image
        ImageIO.write(blendedImage, "PNG", new File("output.png"));
    }
}