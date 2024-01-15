import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class ImageProcessor {

    static {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
    }

    abstract static class Shape {
        Point position;
        Scalar color;
        int size;

        Shape(Point position, Scalar color, int size) {
            this.position = position;
            this.color = color;
            this.size = size;
        }

        abstract void createShape(Mat mask);
    }

    static class Circle extends Shape {
        int radius;

        Circle(Point position, Scalar color, int size) {
            super(position, color, size);
            this.radius = size / 2;
        }

        @Override
        void createShape(Mat mask) {
            Imgproc.circle(mask, position, radius, color, size);
        }
    }

    static class Square extends Shape {
        Point pt1, pt2;

        Square(Point position, Scalar color, int size) {
            super(position, color, size);
            this.pt1 = new Point(position.x - (double) size / 2, position.y - (double) size / 2);
            this.pt2 = new Point(position.x + (double) size / 2, position.y + (double) size / 2);
        }

        @Override
        void createShape(Mat mask) {
            Imgproc.rectangle(mask, pt1, pt2, color, size);
        }
    }

    static class Triangle extends Shape {
        Point[] vertices = new Point[3];

        Triangle(Point position, Scalar color, int size) {
            super(position, color, size);
            vertices[0] = position;
            vertices[1] = new Point(position.x + size, position.y);
            vertices[2] = new Point(position.x, position.y + size);
        }

        @Override
        void createShape(Mat mask) {
            MatOfPoint matOfPoint = new MatOfPoint(vertices);
            List<MatOfPoint> points = new ArrayList<>();
            points.add(matOfPoint);
            Imgproc.fillPoly(mask, points, color);
        }
    }

    private static Mat cutShape(String imagePath, Shape shape) {
        Mat image = Imgcodecs.imread(imagePath);
        Mat mask = new Mat(image.size(), CvType.CV_8UC1, new Scalar(0));

        shape.createShape(mask);

        Mat maskInv = new Mat();
        Core.bitwise_not(mask, maskInv);

        image.setTo(new Scalar(255, 255, 255), mask);
        return image;
    }

    public static void main(String[] args) throws FileNotFoundException {
        String imagePath;
        String outputPath;
        int tests = 10;

        File file = new File("cut_file.txt");
        Scanner scanner = new Scanner(file);
        imagePath = scanner.next();
        outputPath = scanner.next();
        String shape = scanner.next();
        int x = scanner.nextInt();
        int y = scanner.nextInt();
        int size = scanner.nextInt();

        Point position = new Point(x, y);
        Scalar white = new Scalar(255);

        Shape circle = new Circle(position, white, size);
        Shape square = new Square(position, white, size);
        Shape triangle = new Triangle(position, white, size);

        Shape currentShape = switch (shape) {
            case "Circle" -> circle;
            case "Square" -> square;
            default -> triangle;
        };

        long start = System.currentTimeMillis();
        long end = 0;
        for (int i=0; i<tests-1; i++) {
            Mat result = cutShape(imagePath, currentShape);
        }
        Mat result = cutShape(imagePath, currentShape);
        Imgcodecs.imwrite(outputPath, result);
        end = System.currentTimeMillis();
        long time = end - start;
        System.out.println("Time: " + time/tests + " milliseconds");
    }
}
