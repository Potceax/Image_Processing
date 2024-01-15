import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class ImageBlender {

    static {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
    }

    public static void main(String[] args) throws FileNotFoundException {
        long start, stop, time;
        start = System.currentTimeMillis();
        Mat blended = new Mat();
        int tests = 10;

        File file = new File("file.txt");
        Scanner scanner = new Scanner(file);
        String image1Path = scanner.next();
        String image2Path = scanner.next();
        String outputPath = scanner.next();

        // Read the images
        Mat image1 = Imgcodecs.imread(image1Path);
        Mat image2 = Imgcodecs.imread(image2Path);

        // Check if the images are loaded properly
        if (image1.empty() || image2.empty()) {
            System.out.println("Error: Could not open or find the images!");
            return;
        }

        for(int i=0; i<tests; i++) {
            // Resize the second image to match the first
            if (image1.size() != image2.size()) {
                Imgproc.resize(image2, image2, image1.size());
            }

            // Blend the images
            double alpha = 0.5;
            double beta = 1 - alpha;
            Core.addWeighted(image1, alpha, image2, beta, 0.0, blended);
        }
        stop = System.currentTimeMillis();
        time = stop - start;
        System.out.println("Time after 10 tests: " + time + " milliseconds");

        // Save the result
        Imgcodecs.imwrite(outputPath, blended);
    }
}
