// Adapt from http://www.cnblogs.com/shengansong/archive/2011/09/23/2186409.html

#include <expected.hpp>

#include "kbmplib.hpp"
#include "console.hpp"
#include "vfs/vfs.hpp"
#include "vesa.hpp"

std::expected<size_t> read_to_mem(size_t fd, char* buffer, size_t size, size_t offset);

std::expected<size_t> read_to_mem(size_t fd, char* buffer, size_t size, size_t offset) {
    int64_t code = vfs::read(fd, buffer, size, offset);
    if(code < 0){
        return std::make_expected_from_error<size_t, size_t>(-code);
    } else {
        return std::make_expected<size_t>(code);
    }
}

void display_bmp(size_t fd, size_t size, const char* file)
{
    BITMAPFILE bitmap_file;

    uint64_t offset = 0;

    auto res0 = read_to_mem(fd, reinterpret_cast<char*>(&(bitmap_file.bfHeader)), sizeof(BITMAPFILEHEADER), offset);
    offset += sizeof(BITMAPFILEHEADER);
    if(!res0.valid()){
        k_print_line("ERROR");
        return;
    }

    auto res1 = read_to_mem(fd, reinterpret_cast<char*>(&(bitmap_file.biInfo.bmiHeader)), sizeof(BITMAPINFOHEADER), offset);
    offset += sizeof(BITMAPINFOHEADER);
    if(!res1.valid()){
        k_print_line("ERROR");
        return;
    }

    uint8_t bitCountPerPix = bitmap_file.biInfo.bmiHeader.biBitCount;
    uint32_t width = bitmap_file.biInfo.bmiHeader.biWidth;
    uint32_t height = bitmap_file.biInfo.bmiHeader.biHeight;

    if (width > 1024 || height > 768) {
        k_print_line("ERROR: TOO LARGE");
        return;
    }

    uint32_t bmppicth = ((width * bitCountPerPix + 31) >> 5) << 2;
    uint8_t BytePerPix = bitCountPerPix >> 3;
    uint32_t pitch = width * BytePerPix;

    auto start_x = (1024 - width) / 2;
    auto start_y = (768 - height) / 2;

    if (size - offset < height * bmppicth) {
        k_print_line("SIZE ERROR");
        return;
    }

    char* read_data = new char[height * bmppicth];
    int64_t code = vfs::direct_read(file, read_data, height * bmppicth, offset);
    if(code < 0){
        k_print_line("DIRECT READ ERROR");
        return;
    }

    if (read_data)
    {
        int w, h;
        for (h = 0; h < height; ++h)
        {
            for(w = 0; w < width; ++w)
            {
                vesa::draw_pixel(
                  start_x + w,
                  start_y + (height - 1 - h),
                  vesa::make_color(
                    read_data[h * width * BytePerPix + w * BytePerPix + 2],
                    read_data[h * width * BytePerPix + w * BytePerPix + 1],
                    read_data[h * width * BytePerPix + w * BytePerPix + 0]
                  )
                );
            }
        }
    }
    delete read_data;
}
