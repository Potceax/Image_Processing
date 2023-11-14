import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.awt.image.BufferedImageOp;
import java.awt.image.ConvolveOp;
import java.awt.image.Kernel;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws FileNotFoundException {
        String inputImagePath = "input.jpg";
        String outputImagePath = "blurred.jpg";
        int blurIntensity = 5;
        int tests = 10;
        long start = System.currentTimeMillis();
        long end = 0;

        File file = new File("file.txt");
        Scanner scanner = new Scanner(file);
        inputImagePath = scanner.next();
        outputImagePath = scanner.next();
        blurIntensity = scanner.nextInt();

        for (int i = 0; i < tests; i++) {
            try {
                BufferedImage image = ImageIO.read(new File(inputImagePath));

                // Apply a blur filter without black borders
                BufferedImage blurredImage = applyCustomBlur(image, blurIntensity);

                // Crop the image to remove black borders
                int width = blurredImage.getWidth() - 2 * blurIntensity;
                int height = blurredImage.getHeight() - 2 * blurIntensity;
                BufferedImage croppedImage = blurredImage.getSubimage(blurIntensity, blurIntensity, width, height);

                // Save the blurred and cropped image
                ImageIO.write(croppedImage, "JPEG", new File(outputImagePath));
                System.out.println("Image blurred and saved as " + outputImagePath);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        end = System.currentTimeMillis();
        long time = end - start;
        System.out.println("Time: " + time/tests + " miliseconds");
    }

    private static BufferedImage applyCustomBlur(BufferedImage image, int blurIntensity) {
        int kernelSize = blurIntensity * 2 + 1;
        float[] blurMatrix = new float[kernelSize * kernelSize];

        Arrays.fill(blurMatrix, 1.0f / (float) (kernelSize * kernelSize));

        BufferedImageOp op = new ConvolveOp(new Kernel(kernelSize, kernelSize, blurMatrix), ConvolveOp.EDGE_ZERO_FILL, null);
        return op.filter(image, null);
    }
}