//
// nvImage.h - Image support class
//
// The nvImage class implements an interface for a multipurpose image
// object. This class is useful for loading and formating images
// for use as textures. The class supports dds, png, and hdr formats.
//
// Author: Evan Hart
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#ifndef NV_IMAGE_H
#define NV_IMAGE_H

//#ifdef WIN32
//#ifdef NVIMAGE_EXPORTS
//#define NVSDKENTRY __declspec(dllexport)
//#else
//#define NVSDKENTRY __declspec(dllimport)
//#endif
//#endif

#include <vector>
#include <assert.h>

namespace nv {

    class Image {
    public:

        /*NVSDKENTRY*/ Image();
        /*NVSDKENTRY*/ virtual ~Image();

        // return the width of the image
        /*NVSDKENTRY*/ int getWidth() const { return _width; }

        //return the height of the image
        /*NVSDKENTRY*/ int getHeight() const { return _height; }

        //return the dpeth of the image (0 for images with no depth)
        /*NVSDKENTRY*/ int getDepth() const { return _depth; }

        //return the number of mipmap levels available for the image
        /*NVSDKENTRY*/ int getMipLevels() const { return _levelCount; }

        //return the number of cubemap faces available for the image (0 for non-cubemap images)
        /*NVSDKENTRY*/ int getFaces() const { return _faces; }

        //return the format of the image data (GL_RGB, GL_BGR, etc)
        /*NVSDKENTRY*/ GLenum getFormat() const { return _format; }

        //return the suggested internal format for the data
        /*NVSDKENTRY*/ GLenum getInternalFormat() const { return _internalFormat; }

        //return the type of the image data
        /*NVSDKENTRY*/ GLenum getType() const { return _type; }

        //return the Size in bytes of a level of the image 
        /*NVSDKENTRY*/ int getImageSize(int level = 0) const;

        //return whether the data is a crompressed format
        /*NVSDKENTRY*/ bool isCompressed() const {
            switch(_format) {
                case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
                case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
                case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
                case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
				case GL_COMPRESSED_LUMINANCE_LATC1_EXT:
				case GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT:
				case GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT:
				case GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT:
                    return true;
            }
            return false;
        }

        //return whether the image represents a cubemap
        /*NVSDKENTRY*/ bool isCubeMap() const { return _faces > 0; }

        //return whether the image represents a volume
        /*NVSDKENTRY*/ bool isVolume() const { return _depth > 0; }

        //get a pointer to level data
        /*NVSDKENTRY*/ const void* getLevel( int level, GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X) const;
        /*NVSDKENTRY*/ void* getLevel( int level, GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X);

        //initialize an image from a file
        /*NVSDKENTRY*/ int loadImageFromFile( const char* file);

        //convert a suitable image from a cubemap cross to a cubemap (returns false for unsuitable images)
        /*NVSDKENTRY*/ bool convertCrossToCubemap();

        //load an image from memory, for the purposes of saving
        /*NVSDKENTRY*/ bool setImage( int width, int height, GLenum format, GLenum type, const void* data);

        //save an image to a file
        /*NVSDKENTRY*/ bool saveImageToFile( const char* file);

    protected:
        int _width;
        int _height;
        int _depth;
        int _levelCount;
        int _faces;
        GLenum _format;
        GLenum _internalFormat;
        GLenum _type;
        int _elementSize;

        //pointers to the levels
        std::vector<GLubyte*> _data;

        /*NVSDKENTRY*/ void freeData();
        /*NVSDKENTRY*/ void flipSurface(GLubyte *surf, int width, int height, int depth);


        //
        // Static elements used to dispatch to proper sub-readers
        //
        //////////////////////////////////////////////////////////////
        struct FormatInfo {
            const char* extension;
            int (*reader)( const char* file, Image& i);
            bool (*writer)( const char* file, Image& i);
        };

        static FormatInfo formatTable[]; 

        //NVSDKENTRY static int readPng( const char *file, Image& i);
        static int readDDS( const char *file, Image& i);
        //NVSDKENTRY static int readHdr( const char *file, Image& i);

        //NVSDKENTRY static bool writePng( const char *file, Image& i);
        //NVSDKENTRY static bool writeDDS( const char *file, Image& i);
        //NVSDKENTRY static bool writeHdr( const char *file, Image& i);

        /*NVSDKENTRY*/ static void flip_blocks_dxtc1(GLubyte *ptr, unsigned int numBlocks);
        /*NVSDKENTRY*/ static void flip_blocks_dxtc3(GLubyte *ptr, unsigned int numBlocks);
        /*NVSDKENTRY*/ static void flip_blocks_dxtc5(GLubyte *ptr, unsigned int numBlocks);
    };
};

#endif //NV_IMAGE_H