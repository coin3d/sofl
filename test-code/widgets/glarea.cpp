
#include "Inventor/Fl/widgets/SoFlGLArea.h"

extern bool ConvertWXAttrsWxGLFormat(const int *wxattrs,
                                     SoFlGLArea::GLFormat  &format);

int main() {

    {
        int gl_attr[] = {0};
        SoFlGLArea::GLFormat format;
        ConvertWXAttrsWxGLFormat(gl_attr, format);
    }
    {
        int gl_attr[] = {WX_GL_RGBA, 0};
        SoFlGLArea::GLFormat format;
        ConvertWXAttrsWxGLFormat(gl_attr, format);
    }

    {
        // set DEPTH BUFFER
        int gl_attr[] = {WX_GL_DEPTH_SIZE, 16, 0};
        SoFlGLArea::GLFormat format;
        ConvertWXAttrsWxGLFormat(gl_attr, format);
        (void)(format);
    }

}