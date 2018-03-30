#ifndef _XTEXTURE_H_
#define _XTEXTURE_H_

namespace X
{
    /* Clas:            XTexture
     * Description:     A Texture.
     */
    class XTexture
    {
    public:
        XTexture();
        ~XTexture();

        bool LoadTexture(const char *);     // loads a texture from a file.
        void Bind();                        // Binds the texture
        void Unbind();                      // Unbinds the texture
        void GetSize(unsigned int & w, unsigned int & h) const {w = m_w; h = m_h;}
    protected:
        unsigned int m_id,                  // the texture id,
            m_w,                            // the width
            m_h;                            // the height
    };
};

#endif