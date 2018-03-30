#include "tga.h"
#include <iostream>

using namespace std;
namespace X
{
    /*
     *
     */
    bool load_tga(const char * filename, tga_file * tga)
    {
        if(strstr(filename, ".tga") == 0) {
            cerr << "XServer::CreatePixmapFromFile: Only TGA files are supported." << endl;
            return false;
        }
        FILE * file = fopen(filename, "rb");
        if(!file) {
            cerr << "XServer::CreatePixmapFromFile: Failed to open the specified TGA file" << endl;
            return false;
        }
        if(fread(&tga->hdr, sizeof(tga_hdr),1,file) != 1)
        {
            cerr << "XServer::CreatePixmapFromFile: Failed to read the TGA header from the specified file" << endl;
            fclose(file);
            return false;
        }

        tga_hdr & hdr = tga->hdr;
        if(hdr.colourmaptype != 0 || hdr.imagetype != 2 || hdr.bits != 24)
        {
            cerr << "XServer::CreatePixmapFromFile: only 24 bit RGB files without a palette is supported" << endl;
            fclose(file);
            return false;
        }

        // now allocate a buffer for the image data
        tga->pdata = new (std::nothrow) unsigned char[hdr.width*hdr.height*3];
        if(!tga->pdata) {
            cerr << "XServer::CreatePixmapFromFile: Failed to allocate memory for the texture data" << endl;
            fclose(file);
            return false;
        }

        memset(tga->pdata, 0, hdr.width*hdr.height*3);
        // now read the data from the file
        if(fread(tga->pdata, hdr.width*hdr.height*3, 1, file) != 1)
        {
            cerr << "XServer::CreatePixmapFromFile: Failed to read the image data from the file" << endl;
            delete [] tga->pdata; tga->pdata = NULL;
            fclose(file);
            return false;
        }

        fclose(file);
        return true;
    }

    /*
     *
     */
    void free_tga(tga_file * tga)
    {
        delete tga->pdata;
        tga->pdata = NULL;
    }
};