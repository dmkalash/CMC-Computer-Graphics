#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


Image::Image(const std::string &a_path)
{
  if ((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr)
  {
    size = width * height * channels;
  } else {
      std::cout << "Image(path) failed. Name: " << a_path << " " <<
                width << " " << height << " " << channels << std::endl;
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
    if (a_width <= 0 || a_height <= 0) {
        std::cout << "Bad shape" << std::endl;
    }

    data = new Pixel[a_width * a_height] {};
    if(data != nullptr)
    {
        width = a_width;
        height = a_height;
        size = a_width * a_height * a_channels;
        channels = a_channels;
        self_allocated = true;
    }
}


void Image::FillImage(const Pixel &color = MainWindowColor)
{
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            PutPixel(x, height - y - 1, color );
        }
    }
}


void Image::FillImage(Image &bg_image)
{
    int y_up = std::min(bg_image.Height(), height);
    int x_up = std::min(bg_image.Width(), width);
    for(int y = 0; y < y_up; ++y) {
        for(int x = 0; x < x_up; ++x) {
            PutPixel(x, y_up - y - 1, bg_image.GetPixel(x, y_up - y - 1) );
        }
    }
}


int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

Image::~Image()
{
    if(self_allocated)
        delete [] data;
    else
    {
        stbi_image_free(data);
    }
}


Image *Image::conv2D(double *filter, int filter_size, int margin) // TODO: использовать margin
{
    Image *new_img = new Image(width, height, channels);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int new_ind = y * width + x;
            double rsum = 0, gsum = 0, bsum = 0;
            for (int fy = 0; fy < filter_size; fy++) {
                for (int fx = 0; fx < filter_size; fx++) {
                    int img_x = x + (fx - filter_size / 2);
                    int img_y = y + (fy - filter_size / 2);
                    if (img_x < 0 || img_x >= Width() || img_y < 0 || img_y >= Height()) {
                        continue;
                    }

                    int old_ind = img_y * width + img_x;
                    int filter_ind = fy * filter_size + fx;

                    rsum += data[old_ind].r * filter[filter_ind];
                    gsum += data[old_ind].g * filter[filter_ind];
                    bsum += data[old_ind].b * filter[filter_ind];
                }
            }
            new_img->Data()[new_ind].r = rsum;
            new_img->Data()[new_ind].g = gsum;
            new_img->Data()[new_ind].b = bsum;
            new_img->Data()[new_ind].a = data[new_ind].a;
        }
    }
    return new_img;
}


std::unique_ptr<double> Image::getGaussian(int r, double sigma)
{
    double sum = 0;
    std::unique_ptr<double> filter(new double[(2 * r + 1) * (2 * r + 1)]);

    for (int i = 0; i <= 2 * r; i++) {
        for (int j = 0; j <= 2 * r; j++) {
            double val = exp( -((i - r) * (i - r) + (j - r) * (j - r)) / (2 * sigma * sigma)) /
                         (2 * 3.14 * sigma * sigma);
            filter.get()[i * (2 * r + 1) + j] = val;
            sum += filter.get()[i * (2 * r + 1) + j];
        }
    }

    for (int i = 0; i <= 2 * r; i++) {
        for (int j = 0; j <= 2 * r; j++) {
            filter.get()[i * (2 * r + 1) + j] /= sum;
        }
    }

    return filter;
}


std::unique_ptr<double> Image::get_box_filter(int r)
{
    double val = 1.0 / (r * r);
    std::unique_ptr<double> filter(new double[(2 * r + 1) * (2 * r + 1)]);

    for (int i = 0; i <= 2 * r; i++) {
        for (int j = 0; j <= 2 * r; j++) {
            filter.get()[i * (2 * r + 1) + j] = val;
        }
    }

    return filter;
}


std::unique_ptr<double> Image::get_bright_filter(int r, double sigma)
{
    auto filter = getGaussian(r, sigma);
    for (int i = 0; i < (2 * r + 1) * (2 * r + 1); i++) {
        filter.get()[i] = 1 + filter.get()[i];
    }
    return filter;
}


void Image::apply_matrix(int cx, int cy, int r)
{
    for (int y = cy - r, fy = 0; y <= cy + r; y++, fy++) {
        if (y < 0 || y >= height) {
            continue;
        }
        for (int x = cx - r, fx = 0; x <= cx + r; x++, fx++) {
            if (x < 0 || x >= width) {
                continue;
            }

            double sigma = 64.0;
            double coef = 1.0 + exp( -((fx - r) * (fx - r) + (fy - r) * (fy - r)) / (2 * sigma * sigma));


            uint8_t r = std::min(255, int(GetPixel(x, y).r * coef));
            uint8_t g = std::min(255, int(GetPixel(x, y).g * coef));
            uint8_t b = std::min(255, int(GetPixel(x, y).b * coef));
            PutPixel(x, y, {
                    r, g, b, GetPixel(x, y).a
            });
        }
    }
}

