#include <iostream>
#include <libheif/heif.h>
#include <jpeglib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
    #define PATH_SEPARATOR "\\"
#else
    #include <sys/stat.h>
    #define PATH_SEPARATOR "/"
#endif

void extractImagesFromHeic(const std::string &heicFilePath, const std::string &outputFolder)
{
    heif_context *ctx = heif_context_alloc();

    if (heif_context_read_from_file(ctx, heicFilePath.c_str(), nullptr).code == 0)
    {
        heif_image_handle *handle;
        heif_item_id *hid = new heif_item_id[16];
        int numImages = heif_context_get_list_of_top_level_image_IDs(ctx, hid, 16);

        std::string wallsFolderPath = outputFolder + PATH_SEPARATOR + "walls";
        mkdir(wallsFolderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        for (int i = 0; i < numImages; ++i)
        {
            if (heif_context_get_image_handle(ctx, hid[i], &handle).code == 0)
            {
                heif_image *img;
                if (heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr).code == 0)
                {
                    int width = heif_image_get_width(img, heif_channel_interleaved);
                    int height = heif_image_get_height(img, heif_channel_interleaved);
                    std::vector<uint8_t> imageBuffer(width * height * 3);

                    const uint8_t *planeData;
                    int stride;
                    planeData = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);

                    for (int row = 0; row < height; ++row)
                    {
                        std::memcpy(imageBuffer.data() + row * width * 3, planeData + row * stride, width * 3);
                    }

                    std::ostringstream ss;
                    ss << wallsFolderPath << "/wall_" << i << ".jpg";
                    std::string imagePath = ss.str();

                    struct jpeg_compress_struct cinfo;
                    struct jpeg_error_mgr jerr;

                    cinfo.err = jpeg_std_error(&jerr);
                    jpeg_create_compress(&cinfo);

                    FILE *outfile;
                    if ((outfile = fopen(imagePath.c_str(), "wb")) == NULL)
                    {
                        std::cerr << "Cannot open " << imagePath << std::endl;
                        exit(1);
                    }

                    jpeg_stdio_dest(&cinfo, outfile);

                    cinfo.image_width = width;
                    cinfo.image_height = height;
                    cinfo.input_components = 3;
                    cinfo.in_color_space = JCS_RGB;

                    jpeg_set_defaults(&cinfo);
                    jpeg_start_compress(&cinfo, TRUE);

                    JSAMPROW row_pointer[1];
                    int row_stride = width * 3;

                    while (cinfo.next_scanline < cinfo.image_height)
                    {
                        row_pointer[0] = &imageBuffer[cinfo.next_scanline * row_stride];
                        jpeg_write_scanlines(&cinfo, row_pointer, 1);
                    }

                    jpeg_finish_compress(&cinfo);
                    fclose(outfile);

                    jpeg_destroy_compress(&cinfo);

                    heif_image_release(img);
                }
                heif_image_handle_release(handle);
            }
        }
        delete[] hid;
    }

    heif_context_free(ctx);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input.heic> <output_folder>" << std::endl;
        return 1;
    }

    std::string heicFilePath = argv[1];
    std::string outputFolder = argv[2];

    extractImagesFromHeic(heicFilePath, outputFolder);

    std::cout << "Images extracted to " << outputFolder << "/walls" << std::endl;

    return 0;
}