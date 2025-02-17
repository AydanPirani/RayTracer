#!/usr/bin/env python3
from PIL import Image
import numpy as np
import sys

def generate_diff_image(image_path1, image_path2, output_path):
    # Open both images
    img1 = Image.open(image_path1)
    img2 = Image.open(image_path2)

    # Ensure both images are in RGB mode (PPM images usually are)
    if img1.mode != 'RGB':
        img1 = img1.convert('RGB')
    if img2.mode != 'RGB':
        img2 = img2.convert('RGB')

    # Check if the images have the same dimensions
    if img1.size != img2.size:
        print("Error: Images have different dimensions!")
        sys.exit(1)

    # Convert images to NumPy arrays. Using int16 to avoid negative wrap-around.
    arr1 = np.array(img1, dtype=np.int16)
    arr2 = np.array(img2, dtype=np.int16)

    # Compute the absolute difference for each pixel (per channel)
    diff_arr = np.abs(arr1 - arr2).astype(np.uint8)

    # Convert the difference array back to an image
    diff_img = Image.fromarray(diff_arr, mode='RGB')

    # Save the difference image in PPM format
    diff_img.save(output_path, format='PPM')
    print(f"Difference image saved as {output_path}")

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print("Usage: python diff_image.py <image1.ppm> <image2.ppm> <output.ppm>")
        sys.exit(1)
    
    image1_path = sys.argv[1]
    image2_path = sys.argv[2]
    output_path = sys.argv[3]
    
    generate_diff_image(image1_path, image2_path, output_path)
