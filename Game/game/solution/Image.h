#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

constexpr int tileSize = 32;
constexpr int BaseBlockSize = 32;


struct Pixel
{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t a = 0;
};

constexpr Pixel MainWindowColor{143, 163, 160, 0 };
constexpr Pixel EmptyColor{0, 0, 0, 0};


static int clip(int val, int min_val = 0, int max_val = 255)
{
    return std::max( min_val, std::min(max_val, val) );
}

static Pixel blend(Pixel oldPixel, Pixel newPixel)
{
    newPixel.r = clip( newPixel.a / 255.0 * (newPixel.r - oldPixel.r) + oldPixel.r );
    newPixel.g = clip( newPixel.a / 255.0 * (newPixel.g - oldPixel.g) + oldPixel.g );
    newPixel.b = clip( newPixel.a / 255.0 * (newPixel.b - oldPixel.b) + oldPixel.b );

    return newPixel;
}


struct Image
{
    static constexpr double blur_sigma = 1.0;

    Image(const std::string &a_path);
    Image(int a_width, int a_height, int a_channels);

    int Save(const std::string &a_path);

    void FillImage(const Pixel &MainWindowColor);
    void FillImage(Image &bg_image);

    static std::unique_ptr<double> getGaussian(int r, double sigma);
    static std::unique_ptr<double> get_box_filter(int r);
    static std::unique_ptr<double> get_bright_filter(int r, double sigma);

    Image *conv2D(double *filter, int filter_size, int margin);
    void apply_matrix(int cx, int cy, int r);

    int Width()    const { return width; }
    int Height()   const { return height; }
    int Channels() const { return channels; }
    size_t Size()  const { return size; }
    Pixel* Data()        { return  data; }

    Pixel GetPixel(int x, int y) { return data[width * y + x]; }
    Pixel GetPixel(int x, int y, double alpha_coef) {
        Pixel pix = data[width * y + x];
        pix.r *= alpha_coef;
        pix.g *= alpha_coef;
        pix.b *= alpha_coef;
        pix.a *= alpha_coef;
        return pix;
    }
    void  PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }

    ~Image();

private:
    int width = -1;
    int height = -1;
    int channels = 3;
    size_t size = 0;
    Pixel *data = nullptr;
    bool self_allocated = false;
};



#endif //MAIN_IMAGE_H
