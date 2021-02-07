#include "helpers.h"
#include <math.h>
#include <stdio.h>

#define max(x, y) ((x > y) ? x : y)
#define min(x, y) ((x > y) ? y : x)
#define cap(x) (x > 255 ? 255: x)

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int buf;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buf = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = buf;
            image[i][j].rgbtGreen = buf;
            image[i][j].rgbtRed = buf;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < (width / 2); i++)
    {
        for (int j = 0; j < height; j++)
        {
            RGBTRIPLE buf;
            buf = image[j][i];
            image[j][i] = image[j][width - 1 - i];
            image[j][width - 1 - i] = buf;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float blue = 0;
            float green = 0;
            float red = 0;
            int pixel_cnt = 0;
            for (int k = max(0, i - 1); k < min(height, i + 2); k++)
            {
                for (int l = max(0, j - 1); l < min(width, j + 2); l++)
                {
                    blue += image[k][l].rgbtBlue;
                    green += image[k][l].rgbtGreen;
                    red += image[k][l].rgbtRed;
                    pixel_cnt++;
                }
            }
            blurred[i][j].rgbtBlue = round(blue / pixel_cnt);
            blurred[i][j].rgbtGreen = round(green / pixel_cnt);
            blurred[i][j].rgbtRed = round(red / pixel_cnt);
        }
    }
    //image = blurred;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edged[height][width];
    int vkernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int hkernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float Gx_b = 0, Gx_g = 0, Gx_r = 0, Gy_b = 0, Gy_g = 0, Gy_r = 0;
            RGBTRIPLE buf;

            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    if ((i - 1 + k < 0) | (i - 1 + k >= height) | (j - 1 + l < 0) | (j - 1 + l >= width))
                    {
                        buf = (RGBTRIPLE)
                        {
                            0, 0, 0
                        };
                    }
                    else
                    {
                        buf = image[i - 1 + k][j - 1 + l];
                    }

                    Gx_b += vkernel[k][l] * buf.rgbtBlue;
                    Gx_g += vkernel[k][l] * buf.rgbtGreen;
                    Gx_r += vkernel[k][l] * buf.rgbtRed;
                    Gy_b += hkernel[k][l] * buf.rgbtBlue;
                    Gy_g += hkernel[k][l] * buf.rgbtGreen;
                    Gy_r += hkernel[k][l] * buf.rgbtRed;
                }
            }
            edged[i][j].rgbtBlue = cap(round(sqrt((Gx_b * Gx_b) + (Gy_b * Gy_b))));
            edged[i][j].rgbtGreen = cap(round(sqrt((Gx_g * Gx_g) + (Gy_g * Gy_g))));
            edged[i][j].rgbtRed = cap(round(sqrt((Gx_r * Gx_r) + (Gy_r * Gy_r))));
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edged[i][j];
        }
    }
    return;
}
