#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            int avg =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Update pixel values
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute sepia values
            int sepiaRed, sepiaGreen, sepiaBlue;
            sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            // 當數值超過 255，回傳 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // Update pixel with sepia values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixels
            RGBTRIPLE tmp;
            tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 確認中間像素
            if (0 < i && i < (height - 1) && 0 < j && j < (width - 1))
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i - 1][j - 1].rgbtRed + copy[i][j - 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j + 1].rgbtRed) / 9.0);
                image[i][j].rgbtGreen = round((copy[i - 1][j - 1].rgbtGreen + copy[i][j - 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i][j].rgbtGreen + copy[i + 1][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j + 1].rgbtGreen) / 9.0);
                image[i][j].rgbtBlue = round((copy[i - 1][j - 1].rgbtBlue + copy[i][j - 1].rgbtBlue + copy[i + 1][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i][j].rgbtBlue + copy[i + 1][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j + 1].rgbtBlue) / 9.0);
            }
            // 確認最上邊像素
            else if (i == 0 && 0 < j && j < (width - 1))
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i][j - 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + + copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j + 1].rgbtRed) / 6.0);
                image[i][j].rgbtGreen = round((copy[i][j - 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen + + copy[i][j].rgbtGreen + copy[i + 1][j].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j + 1].rgbtGreen) / 6.0);
                image[i][j].rgbtBlue = round((copy[i][j - 1].rgbtBlue + copy[i + 1][j - 1].rgbtBlue + + copy[i][j].rgbtBlue + copy[i + 1][j].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j + 1].rgbtBlue) / 6.0);
            }
            //確認最下邊像素
            else if (i == height && 0 < j && j < (width - 1))
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i - 1][j - 1].rgbtRed + copy[i][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j + 1].rgbtRed) / 6.0);
                image[i][j].rgbtGreen = round((copy[i - 1][j - 1].rgbtGreen + copy[i][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j + 1].rgbtGreen) / 6.0);
                image[i][j].rgbtBlue = round((copy[i - 1][j - 1].rgbtBlue + copy[i][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j + 1].rgbtBlue) / 6.0);
            }
            // 確認最左邊像素
            else if (0 < i && i < (height - 1) && j == 0)
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i - 1][j].rgbtRed + copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j + 1].rgbtRed) / 6.0);
                image[i][j].rgbtGreen = round((copy[i - 1][j].rgbtGreen + copy[i][j].rgbtGreen + copy[i + 1][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j + 1].rgbtGreen) / 6.0);
                image[i][j].rgbtBlue = round((copy[i - 1][j].rgbtBlue + copy[i][j].rgbtBlue + copy[i + 1][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j + 1].rgbtBlue) / 6.0);
            }
            // 確認最右邊像素
            else if (0 < i && i < (height - 1) && j == width)
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i - 1][j - 1].rgbtRed + copy[i][j - 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed) / 6.0);
                image[i][j].rgbtGreen = round((copy[i - 1][j - 1].rgbtGreen + copy[i][j - 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i][j].rgbtGreen + copy[i + 1][j].rgbtGreen) / 6.0);
                image[i][j].rgbtBlue = round((copy[i - 1][j - 1].rgbtRed + copy[i][j - 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed) / 6.0);
            }
            // 確認左上角像素
            else if (i == 0 && j == 0)
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed + + copy[i][j + 1].rgbtRed + copy[i + 1][j + 1].rgbtRed) / 4.0);
                image[i][j].rgbtGreen = round((copy[i][j].rgbtGreen + copy[i + 1][j].rgbtGreen + + copy[i][j + 1].rgbtGreen + copy[i + 1][j + 1].rgbtGreen) / 4.0);
                image[i][j].rgbtBlue = round((copy[i][j].rgbtBlue + copy[i + 1][j].rgbtBlue + + copy[i][j + 1].rgbtBlue + copy[i + 1][j + 1].rgbtBlue) / 4.0);
            }
            // 確認右上角像素
            else if (i == 0 && j == width)
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i][j - 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i][j].rgbtRed + copy[i + 1][j].rgbtRed) / 4.0);
                image[i][j].rgbtGreen = round((copy[i][j - 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen + copy[i][j].rgbtGreen + copy[i + 1][j].rgbtGreen) / 4.0);
                image[i][j].rgbtBlue = round((copy[i][j - 1].rgbtBlue + copy[i + 1][j - 1].rgbtBlue + copy[i][j].rgbtBlue + copy[i + 1][j].rgbtBlue) / 4.0);
            }
            // 確認左下角像素
            else if (i == height && j == 0)
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i - 1][j].rgbtRed + copy[i][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j + 1].rgbtRed) / 4.0);
                image[i][j].rgbtGreen = round((copy[i - 1][j].rgbtGreen + copy[i][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j + 1].rgbtGreen) / 4.0);
                image[i][j].rgbtBlue = round((copy[i - 1][j].rgbtBlue + copy[i][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j + 1].rgbtBlue) / 4.0);
            }
            // 確認右下角像素
            else if (i == height && j == width)
            {
                //取平均
                image[i][j].rgbtRed = round((copy[i - 1][j - 1].rgbtRed + copy[i][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i][j].rgbtRed) / 4.0);
                image[i][j].rgbtGreen = round((copy[i - 1][j - 1].rgbtGreen + copy[i][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i][j].rgbtGreen) / 4.0);
                image[i][j].rgbtBlue = round((copy[i - 1][j - 1].rgbtBlue + copy[i][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i][j].rgbtBlue) / 4.0);
            }
        }
    }
    return;
}
