#ifndef _TGA_H_
#define _TGA_H_

namespace X
{
    typedef unsigned char byte;

    #pragma pack(push, 1)
    struct tga_hdr
    {
        byte  identsize;          // size of ID field that follows 18 byte header (0 usually)
        byte  colourmaptype;      // type of colour map 0=none, 1=has palette
        byte  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

        short colourmapstart;     // first colour map entry in palette
        short colourmaplength;    // number of colours in palette
        byte  colourmapbits;      // number of bits per palette entry 15,16,24,32

        short xstart;             // image x origin
        short ystart;             // image y origin
        short width;              // image width in pixels
        short height;             // image height in pixels
        byte  bits;               // image bits per pixel 8,16,24,32
        byte  descriptor;         // image descriptor bits (vh flip bits)
        
        // pixel data follows header  
    };
    #pragma pack(pop)

    struct tga_file
    {
        tga_hdr hdr;    // the header
        byte * pdata;   // the pixeldata
    };

    bool load_tga(const char *, tga_file *);    // loads a tga file (only 24 bit RGA files supported for the moment).
    void free_tga(tga_file *);
};

#endif